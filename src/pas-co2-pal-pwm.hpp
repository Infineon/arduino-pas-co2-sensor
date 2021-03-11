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
#undef PWM
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
          * @brief       Gets the PWM input duty cycle in percentage
          * 
          * @details     The duty will return 0.00 if no duty is read (e.g.
          *              polling mode). In case of error, the duty value will be
          *              -1.00, and the function will return the corresponding
          *              return code for diagnosis. 
          *
          * @note        The read out of PWM pulses needs to be synched with the
          *              incoming PWM signals. This synchronization needs to be
          *              handled in the PAL.
          *
          * @param[out]  duty PWM input duty cycle value from PWM
          *                   single or train pulse average (00.00 % to 
          *                   100.00 %)
          * @return      PAS CO2 error code
          * @retval      OK if success
          * @retval      INTF_ERROR if error
          */
        virtual Error_t    getDuty  (double & duty) = 0;

};

/** @} */

}

#endif /** PASCO2_PAL_PWM_HPP_ **/