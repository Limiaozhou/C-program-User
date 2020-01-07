#include "mb_rtu_master.h"
#include "check.h"

//modbus rtu 协议读取保持寄存器，reg_adr和reg_len高位在前
void mb_rtu_read_holdreg(uint8_t * pbuf, uint8_t dev_adr, uint8_t func, uint16_t reg_adr, uint16_t reg_num)
{
    uint16_t crc = 0;
    
    *(pbuf + 0) = dev_adr;
    *(pbuf + 1) = func;
    *(pbuf + 2) = (reg_adr >> 8) & 0xFF;
    *(pbuf + 3) = reg_adr & 0xFF;
    *(pbuf + 4) = (reg_num >> 8) & 0xFF;
    *(pbuf + 5) = reg_num & 0xFF;
    
    crc = check_crc16_modbus_calc(pbuf, 6);
    
    *(pbuf + 6) = crc & 0xFF;
    *(pbuf + 7) = (crc >> 8) & 0xFF;
}
