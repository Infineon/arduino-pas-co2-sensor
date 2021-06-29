/** 
 * @file        pas-co2-pulse-ino.hpp
 * @brief       PAS CO2 Pulse Arduino API
 * @date        January 2021
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PULSE_INO_HPP_
#define PAS_CO2_PULSE_INO_HPP_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_PULSE)

#include <Arduino.h>
#include "pas-co2-pulse.hpp"
#include "pas-co2-pal-pwm-ino.hpp"
#include "pas-co2-pal-gpio-ino.hpp"
#include "pas-co2-pal-timer-ino.hpp"
#include "pas-co2-platf-ino.hpp"

/**
 * @addtogroup co2pinoapi
 * @{
 */

class PASCO2PulseIno : public PASCO2Pulse
{
    public:

        PASCO2PulseIno(uint8_t   pwmPin, 
                       uint8_t   intPin         = GPIOIno::unusedPin, 
                       uint8_t   power3V3Pin    = GPIOIno::unusedPin,
                       uint8_t   power12VPin    = GPIOIno::unusedPin,
                       uint8_t   pwmSelPin      = GPIOIno::unusedPin);

        ~PASCO2PulseIno();

        /**
         * @brief   Begins the sensor
         * 
         * @note    Optional call. The sensor initializes and
         *          enables the sensor directly in the rest of
         *          API calls.
         * @return  PAS CO2 error code
         * @retval  OK if success
         * @retval  INTF_ERROR if interface error
         * @retval  IC_POWERON_ERROR if power-on error
         */
        Error_t begin   () { return enable(); };
        
        /**
         * @brief   Ends the sensor
         * 
         * @note    Optional call. The sensor initializes and
         *          enables the sensor directly in the rest of
         *          API calls.
         * @return  PAS CO2 error code
         * @retval  OK if success
         * @retval  INTF_ERROR if interface error
         */
        Error_t end     () { return disable(); };

    private:

        GPIOIno   * pwmGPIO;
        TimerIno  * pwmTimer;
        PWMIno    * pwm;
        TimerIno  * timer;
        GPIOIno   * interrupt;
        GPIOIno   * power3V3;
        GPIOIno   * power12V;
        GPIOIno   * pwmSelect;
};

/** @} */

#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_PULSE_INO_HPP_ **/