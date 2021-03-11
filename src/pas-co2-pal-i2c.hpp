/** 
 * @file        pas-co2-pal-i2c.hpp
 * @brief       PAS CO2 I2C Platform Abstraction Layer
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_I2CPAL_HPP_
#define PAS_CO2_I2CPAL_HPP_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_I2C)

#include <stdint.h>
#include "pas-co2-types.hpp"

namespace pasco2
{

/**
 * @addtogroup co2pal
 * @{
 */

class I2CPAL
{
    public:          

        /**
         * @brief       Initializes the I2C interface
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t    init             ()  = 0;
        
        /**
         * @brief       Deinitializes the I2C interface
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t    deinit           ()  = 0;
        
        /**
         * @brief       Sets the I2C bus frequency 
         * @param[in]   clockHz  Clock frequency in Hertzs
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t    setClockFreq     (const uint32_t clockHz) = 0;        
        
        /**
         * @brief           Reads from the I2C interface the device memory registers
         *                  
         *  The memory addresses length is 8 bits, which point to 8 bits word memory registers.
         *  In bulk read mode (length > 1 byte), the register address passed by argument will be the 
         *  start read address, and the subsequent registers will be read up until the requested 
         *  length is satisfied.
         *  
         * @param[in]       slave7BAddr Slave address 7 bits format. Without write/read bit (8 bit address right shifted 1 bit)
         * @param[in]       memAddress  Device memory register start read address 
         * @param[out]      *data       Pointer to the array that will store the read data
         * @param[in,out]   length      Number of bytes to be read from the bus. Outputs the actual number of bytes read.
         * @return          PAS CO2 error code
         * @retval          OK if success
         * @retval          INTF_ERROR if error
         */
        virtual Error_t    read             (const uint8_t slave7BAddr, const uint8_t memAddr,       uint8_t * data, uint16_t & length)  = 0;
        
        /**
         * @brief           Writes in the I2C interface the device memory registers
         * 
         *  The memory addresses length is 8 bits, which point to 8 bits word memory registers.
         *  In bulk write mode (length > 1 byte), the register address passed by argument will be the 
         *  start write address, and the subsequent registers will be written up until the requested length 
         *  is satisfied. 
         * 
         * @param[in]       slave7BAddr Slave address 7 bits format. Without write/read bit (8 bit address right shifted 1 bit)
         * @param[in]       memAddress  Device memory register start write address
         * @param[in]       *data       Pointer to the data array to be written  
         * @param[in,out]   length      Number of bytes to be written in the bus. Outputs the actual number of bytes written.
         * @return          PAS CO2 error code
         * @retval          OK if success
         * @retval          INTF_ERROR if  error
         */
        virtual Error_t    write            (const uint8_t slave7BAddr, const uint8_t memAddr, const uint8_t * data, uint16_t & length)  = 0;
};

/** @} */

}

#endif /** PAS_CO2_I2C_INTF **/
#endif /** PAS_CO2_I2CPAL_HPP_ **/