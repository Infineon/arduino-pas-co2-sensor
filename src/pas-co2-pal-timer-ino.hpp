/** 
 * @file        pas-co2-pal-timer-ino.hpp
 * @brief       PAS CO2 Timer Arduino PAL 
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PAL_TIMER_INO_HPP_
#define PAS_CO2_PAL_TIMER_INO_HPP_

#include "pas-co2-pal-timer.hpp"

using namespace pasco2;

/**
 * @addtogroup co2inopal
 * @{
 */

class TimerIno: virtual public Timer
{
    public:
                        TimerIno        ();
                        ~TimerIno       ();
        Error_t         init            ();
        Error_t         deinit          ();
        Error_t         start           ();
        Error_t         elapsed         (uint32_t & elapsed);
        Error_t         elapsedMicro    (uint32_t & elapsed);
        Error_t         stop            ();
        Error_t         delay           (const uint32_t timeout);

    private:
        uint32_t curTime;   /**< Current time */
};

/** @} */

#endif /** PAS_CO2_PAL_TIMER_INO_HPP_ **/