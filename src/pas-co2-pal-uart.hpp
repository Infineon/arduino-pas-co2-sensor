/** 
 * @file        pas-co2-pal-uart.hpp
 * @brief       PAS CO2 UART Platform Abstraction Layer
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PAL_UART_HPP_
#define PAS_CO2_PAL_UART_HPP_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_UART)

#include <stdint.h>
#include "pas-co2-types.hpp"

#undef UART_PARITY_NONE
#undef UART_PARITY_ODD
#undef UART_PARITY_EVEN

namespace pasco2
{

/**
 * @addtogroup co2pal
 * @{
 */

class UARTPAL
{
    public:

        /**
         * @name   Data bits
         * @{
         */
        enum DataBits_t
        {         
            UART_5BITS = 5,     /**< 5 Data bits */
            UART_6BITS,         /**< 6 Data bits */
            UART_7BITS,         /**< 7 Data bits */
            UART_8BITS,         /**< 8 Data bits */
            UART_9BITS          /**< 9 Data bits */
        };
        /** @} */

        /**
         * @name   Parity
         * @{
         */
        enum Parity_t
        {
            UART_PARITY_NONE,   /**< Parity None */
            UART_PARITY_ODD,    /**< Parity Odd */
            UART_PARITY_EVEN    /**< Parity Even */
        };
        /** @} */

        /**
         * @name   Stop bits
         * @{
         */
        enum StopBits_t
        {
            UART_1STOPBIT = 1,  /**< 1 Stop bit */
            UART_2STOPBITS      /**< 2 Stop bits */
        };
        /** @} */

        /**
         * @brief       Initializes the UART interface
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t     init        () = 0;
        
        /**
         * @brief       Deinitializes the UART interface
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t     deinit      () = 0;

        /**
         * @brief       Configures the UART interface settings
         * @param[in]   baudrate    Baudrate in bits per second
         * @param[in]   dataBits    Number of data bits transmitted between
         *                          start and stop conditions
         * @param[in]   parity      Parity bit configuration
         * @param[in]   stopBits    Number of stop bits
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t     config      (const uint32_t   baudrate, 
                                         const DataBits_t dataBits, 
                                         const Parity_t   parity, 
                                         const StopBits_t stopBits) = 0;

        /**
         * @brief           Writes in the UART interface 
         * @param[in]       *data       Pointer to the data array to be written
         * @param[in,out]   length      Data length in bytes. Outputs the actual
         *                              number of written bytes
         * @return          PAS CO2 error code
         * @retval          OK if success
         * @retval          INTF_ERROR if error
         */
        virtual Error_t     write       (const uint8_t  * data,      
                                               uint16_t & length) = 0;

        /**
         * @brief           Reads from the UART interface
         * @param[in]       *data       Pointer to the array that will store the
         *                              read data
         * @param[in,out]   length      Data length in bytes. Outputs the actual
         *                              number of read bytes
         * @return          PAS CO2 error code
         * @retval          OK if success
         * @retval          INTF_ERROR if error
         */
        virtual Error_t     read        (       uint8_t  * data,     
                                                uint16_t & length) = 0;
        /**
         * @brief           Waits for a maximum timeout for the specified amount
         *                  of bytes to be received
         * @param[in]       bytesRcvd   Expected number of bytes to be received
         * @param[in]       timeoutMs   Maximum time to wait in milliseconds
         * @return          PAS CO2 error code
         * @retval          OK if success
         * @retval          INTF_ERROR if error or timeout expires
         */
        virtual Error_t     waitReceive (       uint16_t  bytesRcvd, 
                                                uint32_t  timeoutMs) = 0;

};

/** @} */

}

#endif /* PAS_CO2_UART_ENABLED */
#endif /**< PAS_CO2_PAL_UART_HPP_ */