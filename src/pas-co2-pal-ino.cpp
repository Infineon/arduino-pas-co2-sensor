/** 
 * @file        pas-co2-pal-ino.cpp
 * @brief       XENSIV™ PAS CO2 Arduino PAL Implementation
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */


#include <Arduino.h>
#include <Wire.h>
#include "xensiv_pasco2.h"

#define XENSIV_PASCO2_UART_TIMEOUT_MS           (500U)

#define INO_ASSERT(x)   do {                \
                            if(!(x))        \
                            {               \
                                abort();    \
                            }               \
                        } while(false)

int32_t xensiv_pasco2_plat_i2c_transfer(void * ctx, uint16_t dev_addr, const uint8_t * tx_buffer, size_t tx_len, uint8_t * rx_buffer, size_t rx_len)
{
    INO_ASSERT(ctx != NULL);
    INO_ASSERT(tx_buffer != NULL);    

    TwoWire * wire = (TwoWire *)ctx;
    bool send_stop = (rx_buffer != NULL) ? false : true;
    
    wire->beginTransmission((uint8_t)dev_addr);

    uint8_t written = wire->write(tx_buffer, tx_len);

    if(written != tx_len)
    {
        return XENSIV_PASCO2_ERR_COMM;
    }

    if(0 != wire->endTransmission(send_stop))
    {
        return XENSIV_PASCO2_ERR_COMM;
    }

    if(NULL != rx_buffer)
    {
        uint8_t bytes_read = wire->requestFrom((uint8_t)dev_addr, (uint8_t)rx_len);

        if(bytes_read != rx_len)
        {
            return XENSIV_PASCO2_ERR_COMM;
        }

        for(uint16_t i = 0; (i < rx_len) && (wire->available() > 0) ; i++)
        {
            rx_buffer[i] = wire->read();
        }

        if(0 != wire->endTransmission(true))
        {
            return XENSIV_PASCO2_ERR_COMM;
        } 
    }

   return XENSIV_PASCO2_OK;
}

int32_t xensiv_pasco2_plat_uart_read(void * ctx, uint8_t * data, size_t len)
{
    INO_ASSERT(ctx != NULL);
    INO_ASSERT(data != NULL);

    HardwareSerial * uart = (HardwareSerial *)ctx;
    uint32_t timeout = XENSIV_PASCO2_UART_TIMEOUT_MS;
    size_t xfer_len = 0;

    while (((size_t)(uart->available()) < len) && (timeout > 0U))
    {
        delay(1);
        timeout--;
    }

    if (timeout != 0U)
    {
        xfer_len = uart->readBytes(data, len);
    }

    return (len == xfer_len) ? 
            XENSIV_PASCO2_OK : 
            XENSIV_PASCO2_ERR_COMM;
}

int32_t xensiv_pasco2_plat_uart_write(void * ctx, uint8_t * data, size_t len)
{
    INO_ASSERT(ctx != NULL);
    INO_ASSERT(data != NULL);
        
    HardwareSerial * uart = (HardwareSerial *)ctx;

    uart->flush();

    size_t xfer_len = uart->write(data, len);

    return (len == xfer_len) ? 
            XENSIV_PASCO2_OK : 
            XENSIV_PASCO2_ERR_COMM;
}

void xensiv_pasco2_plat_delay(uint32_t ms)
{
    delay(ms);
}

uint16_t xensiv_pasco2_plat_htons(uint16_t x)
{
    uint16_t rev_x = ((x & 0xFF) << 8) | ((x & 0xFF00) >> 8);

    return rev_x;
}

void xensiv_pasco2_plat_assert(int expr)
{
    INO_ASSERT(expr);
}