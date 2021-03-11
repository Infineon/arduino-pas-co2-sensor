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
 
        /**
         * @brief   Measuring Mode 
         */
        enum Mode_t
        {
            MODE_POLLING,               /**< PWM Polling Mode */       
            MODE_INTERRUPT              /**< PWM Interrupt Mode */
        };

                    PWMSW   (GPIO  * const pwmIn, 
                             Timer * const timer,
                             Mode_t        mode);
                                    
                   ~PWMSW   ();
        Error_t     init    ();
        Error_t     deinit  ();
        Error_t     getDuty (double & duty); 

    private:

        GPIO  * const pwmIn;
        Timer * const timer;
        Mode_t        mode;

        volatile bool risingEdgeEvent;                  
        volatile bool fallingEdgeEvent;

        static constexpr uint8_t pulseTrainLen = 160; /**< Pulse train length */

        void poll      ();


        typedef void (* cback_t)(void *);

        static constexpr uint8_t   maxGPIOObjs = 4;             /**< Maximum number of instances which can register hardware interrupt */
        static           uint8_t   idxNext;                     /**< Interrupt array allocation index*/
        static           PWMSW   * objPtrVector[maxGPIOObjs];   /**< PWM object pointers vector */
        static void              * fnPtrVector [maxGPIOObjs];   /**< PWM interrupt function handlers vector */

    protected:

               void         callback    ();

        static void         int0Handler ();
        static void         int1Handler ();
        static void         int2Handler ();
        static void         int3Handler ();

        static void       * isrRegister (PWMSW *objPtr);
};

/** @} */

}
#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_PWM_SW_HPP_ **/