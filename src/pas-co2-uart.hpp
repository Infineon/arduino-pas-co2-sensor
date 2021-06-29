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
#include "pas-co2-pal-uart.hpp"

#undef UART

namespace pasco2
{

/**
 * @addtogroup co2api
 * @{
 */

class UART : public SBus
{
    public:

        static const uint32_t dfltBaudrateBps = 9600;        /**< Default baudrate in bps */

                 UART       (UARTPAL  * const uartpal,
                             uint32_t         baudrateBps = dfltBaudrateBps);
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

        UARTPAL * const uartpal;  
        
        uint32_t                         baudrateBps;
        static const UARTPAL::DataBits_t dataBits            = UARTPAL::UART_8BITS;
        static const UARTPAL::Parity_t   parity              = UARTPAL::UART_PARITY_NONE;
        static const UARTPAL::StopBits_t stopBits            = UARTPAL::UART_1STOPBIT;

    protected:

                char    uint4ToHexChar             (const uint8_t   uint4);
        
        virtual void    assembleWriteRequestFrame  (const uint8_t   regAddr,
                                                    const uint8_t   regVal,
                                                          char    * frame);

        virtual void    assembleReadRequestFrame   (const uint8_t   regAddr,
                                                          char    * frame);

        virtual uint8_t disassembleReadReplyFrame  (const char    * frame);

        virtual bool    isAckFrame                 (const char    * frame,
                                                    const uint8_t   len);

        virtual bool    isNackFrame                (const char    * frame,
                                                    const uint8_t   len);

};

/** @} */

/**
 * @brief 
 */
// extern UART pasco2_uart;

}

#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_UART_H_ **/