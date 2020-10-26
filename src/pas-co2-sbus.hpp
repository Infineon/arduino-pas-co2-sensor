/** 
 * @file        pas-co2-sbus.hpp
 * @brief       PAS CO2 Serial Interface
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_SBUS_HPP_
#define PAS_CO2_SBUS_HPP_

#include "pas-co2-types.hpp"

namespace pasco2
{

/**
 * @addtogroup co2api
 * @{
 */

class SBus
{
    public:

       /**
        * @name   Serial protocol 
        * @{
        */
        enum Proto_t
        {
            BUS_PROTO_I2C   = 0,    /**< I2C protocol */
            BUS_PROTO_UART  = 1     /**< UART protocol */
        };
        /** @} */

        /**
         * @brief       Initializes the serial interface
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual  Error_t    init        (void)  = 0;

        /**
         * @brief       Deinitializes the serial interface
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual  Error_t    deinit      (void)  = 0;

        /**
         * @brief       Writes data to a register or set of consecutive registers
         * 
         * @param[in]   regAddr     Register start write address
         * @param[in]   *data       Pointer to the data array to be written  
         * @param[in]   length      Number of registers to be written in the bus
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual  Error_t    write       (const uint8_t    regAddr, 
                                         const uint8_t  * data, 
                                               uint16_t   length = 1)  = 0;

        /**
         * @brief       Reads data from a register or set of consecutive registers
         * 
         * @param[in]   regAddr     Register start read address
         * @param[out]  *data       Pointer to the array that will store the read data   
         * @param[in]   length      Number of registers to be read from the bus
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual  Error_t    read        (const uint8_t    regAddr,
                                               uint8_t  * data, 
                                               uint16_t   length = 1)  = 0;

        /**
         * @brief       Gets the specific serial protocol
         * @return      PAS CO2 serial protocol
         * @retval      BUS_PROTO_I2C
         * @retval      BUS_PROTO_UART
         */
        virtual  Proto_t    getProtocol (void)  = 0;
};

/** @} */

}


#endif /** PAS_CO2_SBUS_HPP_ **/