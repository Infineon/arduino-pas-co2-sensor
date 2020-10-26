/** 
 * @file        pas-co2-pal-pwm.hpp
 * @brief       PAS CO2 PWM Platform Abstraction Layer
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PAL_PWM_HPP_
#define PAS_CO2_PAL_PWM_HPP_

#include "pas-co2-types.hpp"

namespace pasco2
{

/**
 * @addtogroup co2pal
 * @{
 */

class PWM 
{    
    public:  

        /**
         * @brief       Initializes the PWM
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t     init        () = 0;
        
        /**
          * @brief       Deinitializes the PWM
          * @return      PAS CO2 error code
          * @retval      OK if success
          * @retval      INTF_ERROR if error
          */
        virtual Error_t    deinit      () = 0;
        
        /**
          * @brief       Gets the average CO2 concretation in ppm of a PWM pulse train
          * 
          * @note        The read out of PWM pulses needs to be synched with the incomming 
          *              PWM signals. This synchronization needs to be handled in the PAL.
          * 
          * @param[out]  co2ppm Average CO2 ppm concentration value from PWM pulse train (0 ppm to 10000 ppm)
          * @return      PAS CO2 error code
          * @retval      OK if success
          * @retval      INTF_ERROR if error
          */
        virtual Error_t    getPWM      (int16_t & co2ppm) = 0;

};

/** @} */

}

#endif /** PASCO2_PAL_PWM_HPP_ **/