/** 
 * @file        pas-co2-uart.cpp
 * @brief       PAS CO2 UART Protocol
 * @date        August 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-uart.hpp"

#if IS_INTF(PAS_CO2_INTF_UART)

using namespace pasco2;

UART::UART(UARTPAL const * uartpal) 
: uartpal(uartpal)
{
    /* Coming soon! */
}

UART::~UART(void)
{
    /* Coming soon! */
}

Error_t UART::init(void)
{
    /* Coming soon! */
    return INTF_ERROR;
}

Error_t UART::deinit(void)
{
    /* Coming soon! */
    return INTF_ERROR;
}

Error_t UART::write(const uint8_t   memAddr,
                    const uint8_t * data,
                          uint16_t  length)
{
    /* Coming soon! */
    return INTF_ERROR;
}

Error_t UART::read(const uint8_t   memAddr,
                         uint8_t * data,
                         uint16_t  length)
{
      /* Coming soon! */
    return INTF_ERROR;
}

#endif /** PAS_CO2_INTF **/