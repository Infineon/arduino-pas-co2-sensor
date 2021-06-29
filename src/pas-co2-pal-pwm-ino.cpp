/** 
 * @file        pas-co2-pal-pwm-ino.cpp
 * @brief       PAS CO2 PWM Arduino PAL
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-pal-pwm-ino.hpp"

#if IS_INTF(PAS_CO2_INTF_PULSE)

/**
 * @brief       PWM Ino constructor
 * 
 * @details     Implemented based on the PAS CO2 PWM software class
 * 
 * @param[in]   *pwmIn          PWM input instance
 * @param[in]   *timer          Timer instance         
 * @param[in]   mode            PWM input mode. Hardware interrupt or polling 
 *                              mode
 * @pre         None
 */
PWMIno::PWMIno(GPIOIno  * const pwmIn, 
               TimerIno * const timer,
               Mode_t           mode)
:PWMSW(pwmIn, timer, mode)
{

}

/**
 * @brief       PWM Ino destructor
 */
PWMIno::~PWMIno()
{

}

#endif /** PAS_CO2_INTF **/