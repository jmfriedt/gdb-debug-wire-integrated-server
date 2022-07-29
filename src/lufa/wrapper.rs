use crate::lufa::bindings::{CDC_LineEncoding_t, LUFA_Endpoint_ConfigureEndpoint, LUFA_get_last_request_header, LUFA_get_line_encoding, USB_Request_Header_t};

pub unsafe fn endpoint_configure(address: u8, type_: u8, size: u16, banks: u8) -> Result<(), ()> {
    if LUFA_Endpoint_ConfigureEndpoint(address, type_, size, banks) {
        return Ok(())
    }
    Err(())
}

pub unsafe fn get_line_encoding() -> CDC_LineEncoding_t{
    return * get_line_encoding_ptr()
}

pub unsafe fn get_last_request_header() -> USB_Request_Header_t{
    return * get_last_request_header_ptr()
}

pub unsafe fn get_line_encoding_ptr() -> *mut CDC_LineEncoding_t{
    return LUFA_get_line_encoding()
}

pub unsafe fn get_last_request_header_ptr() -> *mut USB_Request_Header_t{
    return LUFA_get_last_request_header()
}