/** 
 * @file        pas-co2-pwm-sw.cpp
 * @brief       PAS CO2 PWM Input Software
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-pwm-sw.hpp"

#if IS_INTF(PAS_CO2_INTF_PWM_SW)

using namespace pasco2;

PWMSW::PWMSW(GPIO  * const pwmIn, 
             Timer * const timer)
: pwmIn(pwmIn), timer(timer)
{
    /* Coming soon! */
}
                                
PWMSW::~PWMSW  ()
{
    /* Coming soon! */
}

Error_t PWMSW::init()
{
    /* Coming soon! */

    return INTF_ERROR;
}

Error_t PWMSW::deinit()
{
    /* Coming soon! */
    return INTF_ERROR;
}

Error_t PWMSW::getPMW()
{
    /* Coming soon! */
    return INTF_ERROR;
}

#endif /** PAS_CO2_INTF **/

