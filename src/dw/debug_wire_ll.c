//
// Created by stefano on 31/07/22.
//

#include <avr/io.h>
#include <util/delay.h>
#include <dw/debug_wire.h>
#include <dw/devices.h>
#include <panic.h>

debug_wire_t debug_wire_g;

static volatile uint8_t waiting_break;

uint8_t dw_init(uint32_t target_freq) {
    cli();
    debug_wire_g.target_frequency = target_freq;
    od_uart_clear();
    od_uart_init(target_freq >> 7);
    sei();

    if(debug_wire_g.device == NULL){
        MUST_SUCCEED(dw_cmd_halt(), 1);
        uint16_t signature = dw_cmd_get(DW_CMD_REG_SIGNATURE);

        uint16_t len = pgm_read_word(&devices.items);
        dw_device_definition_t * devs = (dw_device_definition_t *) &devices.devices;
        while(len--){
            if(pgm_read_word(&devs->signature) == signature){
                debug_wire_g.device = devs;
                return 1;
            }
            devs++;
        }
        return 0;
    }
    return 1;
}

//vi sono due tipi di break:
// nel caso di halt da stato runnning: il controllore risponde dopo la fine del break e ritorna 0x55
// nel casp di halt inatteso (già halted) il controllore asserisce oltre la durata causando un interrupt startbit
// (todo solve: è perchè uso irq low invece che falling edge)
uint8_t dw_cmd_halt(void){
    od_uart_clear();
    od_uart_break();

    uint8_t ret;
    od_uart_recv(&ret, 1);
    if(ret != 0x55){
        od_uart_recv(&ret, 1);
        PORTD ^= (1 << PIND4);
    }
    if(ret != 0x55) return 0;
    debug_wire_g.status = DW_STATUS_HALTED;

    return dw_cmd_set_speed(DW_DIVISOR_128);
}

uint16_t dw_cmd_get_16(uint8_t cmd){
    uint16_t ret;
    dw_cmd(cmd);
    od_uart_recv(&ret, 2);
    return ret;
}
uint8_t dw_cmd_get_8(uint8_t cmd){
    uint8_t ret;
    dw_cmd(cmd);
    od_uart_recv(&ret, 1);
    return ret;
}
uint8_t dw_cmd_set_speed(uint8_t divisor){
    debug_wire_g.cur_divisor = divisor;
    return dw_cmd_get_8(divisor) == 0x55;
}
void dw_cmd_send_multiple(uint8_t command, void * data, uint8_t len){
    dw_cmd(command);
    while(OD_UART_TX_FULL());
    od_uart_send(data, len);
}

void dw_cmd_reset(void){
    od_uart_clear();
    waiting_break = 1;
    dw_cmd(DW_CMD_RESET);
    while(waiting_break);
}

inline void od_uart_irq_rx(uint8_t data){}
inline void od_uart_irq_frame_error(void){

}
inline void od_uart_irq_break(void){
    dw_init(debug_wire_g.target_frequency);
    od_uart_discard(1);
    dw_cmd_set_speed(debug_wire_g.cur_divisor);
    waiting_break = 0;
}
