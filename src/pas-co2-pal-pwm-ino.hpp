/** 
 * @file        pas-co2-pal-pwm-ino.hpp
 * @brief       PAS CO2 PWM Arduino PAL
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PAL_PWM_INO_H_
#define PAS_CO2_PAL_PWM_INO_H_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_PULSE)

#include <Arduino.h>
#include "pas-co2-pwm-sw.hpp"
#include "pas-co2-pal-gpio-ino.hpp"
#include "pas-co2-pal-timer-ino.hpp"

using namespace pasco2;

/**
 * @addtogroup co2inopal
 * @{
 */

class PWMIno: virtual public PWMSW
{
    public:

        PWMIno  (GPIOIno  * const pwmIn, 
                 TimerIno * const timer,
                 Mode_t           mode);

       ~PWMIno  ();
};

/** @} */

#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_PAL_PWM_INO_HPP_ **/
