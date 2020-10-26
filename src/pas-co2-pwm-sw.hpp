/** 
 * @file        pas-co2-pwm-sw.hpp
 * @brief       PAS CO2 PWM Input Software
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PWM_SW_HPP_
#define PAS_CO2_PWM_SW_HPP_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_PWM_SW)

#include "pas-co2-pal-gpio.hpp"
#include "pas-co2-pal-timer.hpp"
#include "pas-co2-pal-pwm.hpp"

namespace pasco2
{

/**
 * @addtogroup co2api
 * @{
 */

class PWMSW : public PWM
{
    public:

                PWMSW   (GPIO  * const pwmIn, 
                         Timer * const timer);
                                 
               ~PWMSW   ();
    Error_t     init    ();
    Error_t     deinit  ();
    Error_t     getPMW  (); 

    private:

        GPIO  const * pwmIn;
        Timer const * timer;
};

/** @} */

}
#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_I2C_H_ **/