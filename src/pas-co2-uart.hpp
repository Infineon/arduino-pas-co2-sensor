/** 
 * @file        pas-co2-uart.hpp
 * @brief       PAS CO2 UART Protocol
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_UART_HPP_
#define PAS_CO2_UART_HPP_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_UART)

#include <stdint.h>
#include "pas-co2-sbus.hpp"

namespace pasco2
{

/**
 * @addtogroup co2api
 * @{
 */

class UART : public SBus
{
    public:

        class UARTPAL;

                 UART       (UARTPAL const * uartpal);
                ~UART       (void);

        Error_t  init       (void);
        Error_t  deinit     (void);
  
        Error_t  write      (const uint8_t   regAddr,
                             const uint8_t * data,
                                   uint16_t  length);
        Error_t  read       (const uint8_t   regAddr,      
                                   uint8_t * data,
                                   uint16_t  length);  
        Proto_t  getProtocol(void)  { return BUS_PROTO_UART; };   

    private:

        UARTPAL const * uartpal;     
};

/** @} */

/**
 * @brief 
 */
extern UART pasco2_uart;

}

#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_UART_H_ **/