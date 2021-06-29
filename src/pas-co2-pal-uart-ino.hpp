/** 
 * @file        pas-co2-pal-uart-ino.hpp
 * @brief       PAS CO2 UART Arduino PAL
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PAL_UART_INO_H_
#define PAS_CO2_PAL_UART_INO_H_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_UART)

#include <Arduino.h>
#include "pas-co2-pal-uart.hpp"

using namespace pasco2;

/**
 * @addtogroup co2inopal
 * @{
 */

class UARTPALIno : virtual public UARTPAL
{
    public:
                        UARTPALIno      (HardwareSerial * serial);
                       ~UARTPALIno      ();
        Error_t         init            ();
        Error_t         deinit          ();
        Error_t         config          (const uint32_t   baudrate, 
                                         const DataBits_t dataBits, 
                                         const Parity_t   parity, 
                                         const StopBits_t stopBits);
        Error_t         write           (const uint8_t  * data,      
                                               uint16_t & length);
        Error_t         read            (      uint8_t  * data,     
                                               uint16_t & length);
        Error_t         waitReceive     (      uint16_t   bytesRcvd, 
                                               uint32_t   timeoutMs);

    private:

        HardwareSerial * serial;


};

/** @} */

extern UARTPALIno uartpalino;

#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_PAL_UART_INO_HPP_ **/