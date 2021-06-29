/** 
 * @file        pas-co2-pal-logger-ino.cpp
 * @brief       PAS CO2 Logger Arduino PAL 
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-pal-logger-ino.hpp"

#if (PAS_CO2_LOGGER_ENABLED == 1)

#include <Arduino.h>
#include "pas-co2-logger.hpp"

using namespace pasco2;

/**
 * @brief Logger Arduino PAL constructor
 */
LoggerIno::LoggerIno()
{

}

/**
 * @brief Logger Arduino PAL destructor
 */
LoggerIno::~LoggerIno()
{

}

/**
 * @brief       Initializes the logger Arduino PAL 
 * @note        Serial is initialized by the Arduino core 
 *              and begun in setup().
 * @return      PAS CO2 error code
 * @retval      OK always
 */
Error_t  LoggerIno::init()
{
  return pasco2::OK;
}

/**
 * @brief       Deinitializes the logger Arduino PAL
 * @return      PAS CO2 error code
 * @retval      OK always
 */
Error_t   LoggerIno::deinit()
{
  return pasco2::OK;
} 

/**
 * @brief       Writes in the logger Arduino PAL
 * @param[in]   *logData    Pointer to the array holding the data to be logged
 * @param[in]   length      Length in bytes of the data to be written 
 * @return      PAS CO2 error code
 * @retval      OK always
 */
Error_t   LoggerIno::write(const uint8_t * log_data, uint32_t length)
{
  String str = (char*)log_data;

  for(uint32_t i=0; i< length; i++) {
    Serial.print(str.charAt(i));
  }
  
  return pasco2::OK;
}

/**
 * @brief Library Logger
 */
LoggerIno logpalino;

namespace pasco2
{
Logger    cotwo_log(static_cast<LoggerPAL*>(&logpalino));
}

#endif /* PAS_CO2_LOGGER_ENABLED */