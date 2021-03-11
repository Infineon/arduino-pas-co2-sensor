/** 
 * @file        pas-co2.hpp
 * @brief       PAS CO2 Core API
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_HPP_
#define PAS_CO2_HPP_

#include <stdint.h>
#include "pas-co2-sbus.hpp"
#include "pas-co2-pal-pwm.hpp"
#include "pas-co2-pal-timer.hpp"
#include "pas-co2-pal-gpio.hpp"
#include "pas-co2-reg.hpp"

#undef PWM

using namespace pasco2;

/**
 * @addtogroup co2api
 * @{
 */

class PASCO2
{
    public:

                   PASCO2(pasco2::SBus  * const sbus,
                          pasco2::PWM   * const pwm,
                          pasco2::Timer * const timer       = nullptr,
                          pasco2::GPIO  * const interrupt   = nullptr,
                          pasco2::GPIO  * const protoSelect = nullptr,
                          pasco2::GPIO  * const power3V3    = nullptr, 
                          pasco2::GPIO  * const power12V    = nullptr,
                          pasco2::GPIO  * const pwmSelect   = nullptr);

       virtual     ~PASCO2                ();

       Error_t     init                   ();
       Error_t     deinit                 ();

       Error_t     enableLogic            ();
       Error_t     disableLogic           ();

       Error_t     enableIREmitter        ();
       Error_t     disableIREmitter       ();

       Error_t     softReset              ();
       Error_t     hardReset              ();

       Error_t     setOperationMode       (OpMode_t mode);

       Error_t     enablePWM              ();
       Error_t     disablePWM             ();
       Error_t     setPWMMode             (PWMMode_t mode);

       Error_t     setMeasPeriod          (int16_t periodInSec);   

       Error_t     getCO2                 (int16_t & CO2PMM);
       
       Error_t     getDiagnosis           (Diag_t & diagnosis);

       Error_t     getMeasStatus          (MeasStatus_t & measSt);

       Error_t     enableInterrupt        (void (*cback) (void *), Int_t intType, IntIOConf_t ioConf);
       Error_t     disableInterrupt       ();

       Error_t     enableABOCompensation  (ABOC_t aboc, int16_t abocRef);
       Error_t     disableABOCompensation ();

       Error_t     setPressureCompensation(uint16_t pressRef);
       
       Error_t     enableAlarm            (int16_t thres, Alarm_t alarmType);
       Error_t     disableAlarm           ();  

       Error_t     getDeviceID            (uint8_t & prodID, uint8_t & revID);
       
       Status_t    getStatus              () { return status; }

    protected:

       pasco2::SBus   * const sbus;                /**< SBus interface  */ 
       pasco2::PWM    * const pwm;                 /**< PWM interface */ 
       pasco2::Timer  * const timer;               /**< Timer */ 
       pasco2::GPIO   * const interrupt;           /**< Interrupt */ 
       pasco2::GPIO   * const protoSelect;         /**< Serial protocol select */ 
       pasco2::GPIO   * const power3V3;            /**< 3V3 power */ 
       pasco2::GPIO   * const power12V;            /**< 12V power */ 
       pasco2::GPIO   * const pwmSelect;           /**< PWM select */ 

       pasco2::Reg  reg;                           /**< Register map */
       Status_t     status;                        /**< Sensor host mcu state machine status */ 

       virtual Error_t       setStatus                  (const Status_t newStatus, 
                                                         bool           enoughIfImplicit = false);
       virtual Error_t       preResetSaveConfig         (void);
       virtual Error_t       postResetRestoreConfig     (void);

       static const int16_t periodMeasMin = 0x0005;   /**< Minimum measurement period in Seconds */  
       static const int16_t periodMeasMax = 0x0FFF;   /**< Maximum measurement period in Seconds */
       static const uint16_t pressRefMin  = 0x258;    /**< Minimum pressure reference value */
       static const uint16_t pressRefMax  = 0x640;    /**< Maximum pressure reference value */

};

/** @} */

#endif /** PAS_CO2_HPP_ **/