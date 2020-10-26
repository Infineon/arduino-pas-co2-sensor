/** 
 * @file        pas-co2-i2c.cpp
 * @brief       PAS CO2 I2C Protocol
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-i2c.hpp"

#if IS_INTF(PAS_CO2_INTF_I2C)

#include "pas-co2-logger.hpp"

using namespace pasco2;

/**
 * @brief         I2C interface constructor
 * @param[in]     *i2cpal I2C pal interface
 * @param[in]     freqHz  I2C bus frequency in Hz. Default is 400000. 
 * @pre           None       
 */
I2C::I2C(I2CPAL * i2cpal, uint32_t freqHz) 
: i2cpal(i2cpal), 
  clkFreq(freqHz)
{

}

/**
 * @brief         I2C interface destructor
 * @pre           None
 */
I2C::~I2C(void)
{
      
}

/**
 * @brief         Initializes the I2C interface   
 * 
 * @details       Initiliazes the host I2C peripheral and set its clock frequency
 * 
 * @return        PAS CO2 error code
 * @retval        OK if success
 * @retval        INTF_ERROR if hardware interface error
 * @pre           None
 */
Error_t I2C::init(void)
{
      Error_t err = OK;
      
      PAS_CO2_I2C_LOG_MSG(__FUNCTION__);
      do
      {
            err = i2cpal->init();
            if (OK != err)
                  break;

            err = i2cpal->setClockFreq(clkFreq);
            if (OK != err)
                  break;

      } while (0);
      PAS_CO2_I2C_LOG_RETURN(err);

      return err;
}

/**
 * @brief         Deinitializes the I2C interface   
 * 
 * @return        PAS CO2 error code
 * @retval        OK if success
 * @retval        INTF_ERROR if hardware interface error
 * @pre           None
 */
Error_t I2C::deinit(void)
{
      Error_t err = OK;
      
      PAS_CO2_I2C_LOG_MSG(__FUNCTION__);

      err = i2cpal->deinit();

      PAS_CO2_I2C_LOG_RETURN(err);

      return err;
}

/**
 * @brief       Writes data to a register or set of consecutive registers
 * 
 * @param[in]   regAddr     Register start write address
 * @param[in]   *data       Pointer to the data array to be written  
 * @param[in]   length      Number of registers to be written in the bus
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if error
 * @pre         init()
 */
Error_t I2C::write(const uint8_t   regAddr,
                   const uint8_t * data,
                         uint16_t  length)
{
      Error_t err = OK;

      PAS_CO2_I2C_LOG_MSG(__FUNCTION__);

      err = i2cpal->write(addr7bits, regAddr, data, length);

      PAS_CO2_I2C_LOG_RETURN(err);
      
      return err;
}

/**
 * @brief       Reads data from a register or set of consecutive registers
 * 
 * @param[in]   regAddr     Register start read address
 * @param[out]  *data       Pointer to the array that will store the read data   
 * @param[in]   length      Number of registers to be read from the bus
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if error
 * @pre         init()
 */
Error_t I2C::read(const uint8_t   regAddr,
                        uint8_t * data,
                        uint16_t  length)
{
      Error_t err = OK;

      PAS_CO2_I2C_LOG_MSG(__FUNCTION__);

      err = i2cpal->read(addr7bits, regAddr, data, length);

      PAS_CO2_I2C_LOG_RETURN(err);

      return err;
}

#endif /** PAS_CO2_INTF **/