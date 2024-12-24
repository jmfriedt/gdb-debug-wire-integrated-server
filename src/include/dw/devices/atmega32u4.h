//
// Created by jmfriedt on 24/12/24.
// https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf p.356

#ifndef ARDWINO_ATMEGA32U4_H
#define ARDWINO_ATMEGA32U4_H
#include <dw/devices.h>
//                                  sig     sram_base   sram_end    flash_page_end  flash_end   eeprom_end  spmcsr  dwdr  eearl eecr  eedr
#define DW_DEF_ATMEGA32U4 DW_DEV_DEF(0x8795, 0x100,      0x0AFF,     64,             0x3FFF,     0x03FF,     0x37,   0x31, 0x21, 0x1F, 0x20)

#endif //ARDWINO_ATMEGA32U4_H
