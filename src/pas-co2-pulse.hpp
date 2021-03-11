/** 
 * @file        pas-co2-pulse.hpp
 * @brief       PAS CO2 Pulse API
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PULSE_HPP_
#define PAS_CO2_PULSE_HPP_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_PULSE)

#include "pas-co2-types.hpp"
#include "pas-co2.hpp"

#undef GPIO
using namespace pasco2;

/**
 * @addtogroup co2papi
 * @{
 */

class PASCO2Pulse : private PASCO2
{
    public:

                PASCO2Pulse     (        PWM   * const pwm,
                                         Timer * const timer       = nullptr,
                                 pasco2::GPIO  * const interrupt   = nullptr,
                                 pasco2::GPIO  * const power3V3    = nullptr, 
                                 pasco2::GPIO  * const power12V    = nullptr,
                                 pasco2::GPIO  * const pwmSelect   = nullptr);

        virtual ~PASCO2Pulse    ();

        Error_t enable          ();  
        Error_t disable         ();


        Error_t startMeasure    (void (*cback) (void *) = nullptr);
        Error_t stopMeasure     ();
        Error_t getCO2          (int16_t & co2ppm);

        Error_t reset           ();
};

/** @} */

#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_PULSE_HPP_ **/