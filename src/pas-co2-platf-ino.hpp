/** 
 * @file        pas-co2-platf-ino.hpp
 * @brief       PAS CO2 Arduino Hardware Platforms
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PLATF_INO_HPP_
#define PAS_CO2_PLATF_INO_HPP_

#include <Arduino.h>
#include <Wire.h>

/**
 * @addtogroup co2inohw
 * @{
 */

typedef struct 
{   
    TwoWire         * i2c;          /**< I2C interface*/
    HardwareSerial  * uart;         /**< UART interface */
    uint8_t         power3V3;       /**< Power supply VDD 3V3 pin */
    uint8_t         power12V;       /**< Power supply VDD 12V pin */
    uint8_t         pwm;            /**< PWM output pin */
    uint8_t         pwmSelect;      /**< PWM disable pin */ 
    uint8_t         protoSelect;    /**< Serial interface select pin*/
    uint8_t         inte;           /**< Interrupt pin*/
}PlatformIno_t;

extern PlatformIno_t * pltf;                /**< Arduino Default platform */

extern PlatformIno_t PASCO2_S2Go_XMC2Go;    /**< PAS CO2 Shield2Go + XMC2Go stack */
extern PlatformIno_t PASCO2_S2Go_XMC4700;   /**< PAS CO2 Shield2Go + MyIOT adapter (Socket 1) + XMC4700 relax kit */

/** @} */

#endif /** PAS_CO2_PLATF_INO_HPP_ **/