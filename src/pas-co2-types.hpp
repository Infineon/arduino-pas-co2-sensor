/** 
 * @file        pas-co2-types.hpp
 * @brief       PAS CO2 Types
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_TYPES_HPP_
#define PAS_CO2_TYPES_HPP_

namespace pasco2
{
       /**
        * @addtogroup co2types
        * @{
        */

       /**
        * @name   Error codes
        * @{
        */
        enum Error_t
       {
              OK                     =  0,       /**< Success. No error */
              INTF_ERROR             = -1,       /**< Interface error */
              RESET_ERROR            = -2,       /**< Reset error */
              CONFIG_ERROR           = -3,       /**< Configuration error */
              IC_ERROR               = -14,      /**< Sensor internal error */
              IC_POWERON_ERROR       = -15,      /**< Power on internal error */
              IC_POWER12V_ERROR      = -16,      /**< IR power internal error */
              IC_TEMP_ERROR          = -17,      /**< Temperature excedded error */
              IC_COMM_ERROR          = -18       /**< Serial communication interface internal error */
              
       };
       /** @} */

       /**
        * @name   Power status flags
        * @{
        */
       enum StatusFlags_t
       {
              POWER3V3        = 0x02,     /**< VDD power 3V3 enabled mask */
              POWER12V        = 0x04      /**< IR emitter 12V enabled mask */
       };
       /** @} */

       /**
        * @name   Sensor status
        * @{
        */
       enum Status_t
       {
              UNINITED          =  0x00,                       /**< Controller peripherals not initialized */
              INITED            =  0x01,                       /**< Controller peripherals initialized. Sensor off */
              LOGIC_ONLY_ON     =  INITED   | POWER3V3,        /**< Sensor VDD power on */       
              IR_ONLY_ON        =  INITED   | POWER12V,        /**< Sensor IR emitter power on */ 
              ON                =  LOGIC_ONLY_ON | IR_ONLY_ON, /**< Sensor and IR emitter power on */        
        };
       /** @} */

       /**
        * @name   Operation modes
        * @{
        */
       enum OpMode_t
       {
              MODE_IDLE             = 0,  /**< Idle mode. No measurement is performed */
              MODE_SINGLE_SHOT      = 1,  /**< Single shot. Triggers one measurement and returns to idle */
              MODE_CONTINUOUS_MEAS  = 2,  /**< Periodic measurement mode */ 
       };
       /** @} */       
       
       /**
        * @name   PWM modes
        * @{
        */
       enum PWMMode_t
       {
              PWM_SINGLE_PULSE =  0,      /**< PWM Single pulse per measurement */
              PWM_TRAIN_PULSE  =  1       /**< PWM Train of 160 pulses per measurement */  
       };
       /** @} */

       /**
        * @name   Automatic baseline compensation modes
        * @{
        */
       enum ABOC_t
       {      
              ABOC_DISABLED           = 0,       /**< ABOC disabled */
              ABOC_ENABLED_PERIODIC   = 1,       /**< ABOC enabled (weekly period) */
              ABOC_ENABLED_FORCED     = 2        /**< ABOC forced for factory calibration. After 10 meas, the ABOC conf changes to periodic mode */
       };
       /** @} */

       /**
        * @name   Sensor interfaces 
        * @{
        */
       enum Intf_t 
       {
              INTF_PWM,     /**< PWM interface */
              INTF_UART,    /**< UART interface */
              INTF_I2C      /**< I2C interface */             
       };
       /** @} */

       /**
        * @name   Interrupt modes
        * @{
        */
       enum Int_t
       {
              INT_DISABLED          = 0,  /**< Interrupt disabled */
              INT_ALARM_ACTIVE      = 1,  /**< Interrupt on alarm activation */
              INT_DATA_READY        = 2,  /**< Interrupt on measurement data ready */
              INT_SERIAL_BUS_BUSY   = 3,  /**< Interrupt on serial bus busy */
              INT_EARLY_MEAS        = 4   /**< Interrupt before starting measurement */
       };
       /** @} */

       /**
        * @name   Interrupt output configuration
        * @{
        */
       enum IntIOConf_t
       {
              INT_IO_PUSH_PULL_ACTIVE_LOW  = 0,  /**< Interrupt voltage low enabled */
              INT_IO_PUSH_PULL_ACTIVE_HIGH = 1   /**< Interrupt voltage high enabled */
       };
       /** @} */

       /**
        * @name   Alarm threshold modes
        * @{
        */
       enum Alarm_t
       {
              ALARM_TH_CROSS_DOWN = 0,    /**< Cross down threshold mode */
              ALARM_TH_CROSS_UP   = 1     /**< Cross up threshold mode */
       };
       /** @} */

       /**
        * @name   Measurement status flags
        * @{
        */
       typedef struct 
       {
              bool dataRdy;        /**< Data ready flag. Enabled after measurement completion */
              bool intActive;      /**< Interrtupt active flag. Enabled after the configured interrupt event has ocurred */
              bool alarmActive;    /**<s Alarm active flag. Enabled if alarm CO2 concentration threshold has been trespassed */
       }MeasStatus_t;
       /** @} */

       /**
        * @name   Sensor status flags
        * @{
        */
       typedef struct 
       {
              bool sensorRdy;             /**< Sensor ready after power on flag */
              bool pwmPinEnabled;         /**< PWM select input pin enabled flag */
              bool outOfRangeTempError;   /**< Sensor temperature out of range error flag */
              bool outOfRange12VError;    /**< IR voltage out of range error flag */
              bool commError;             /**< Communication error flag */       
       }Diag_t;    
       /** @} */

       /** @} */
}



#endif /** PAS_CO2_TYPES_HPP_ **/