/** 
 * @file        pas-co2-pulse.cpp
 * @brief       PAS CO2 Pulse API
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-pulse.hpp"

#if IS_INTF(PAS_CO2_INTF_PULSE)

#include "pas-co2-logger.hpp"
/**
 * @brief       PAS CO2 Pulse Constructor
 *
 * @details     Mandatory arguments: 
 *              - The PWM needs to be provided 
 *              - A timer instance (currently default as nullptr.TODO: Needs to 
 *                be changed!)
 *
 *              Optional  arguments:
 *              - All GPIO instances.
 *             
 *              Most of the GPIO will be fixed by hardware. In particular those
 *              enabling the sensor main power and the emitter power. 
 *              They are part of the sensor library to provide potential 
 *              absolute control by software.
 *              The hardware interface and its features, are also provided in 
 *              the library.
 *              But it can be as well handled externally 
 *
 * @param[in]   *PWM            PWM instance
 * @param[in]   *timer          Timer instance
 * @param[in]   *interrupt      Interrupt GPIO instance. Default is nullptr
 * @param[in]   *power3V3       Power 3V3 control GPIO instance. Default is
 *                              nullptr
 * @param[in]   *power12V       Power 12V control GPIO instance. Default is
 *                              nullptr
 * @param[in]   *pwmSelect      PWM select GPIO instance. Default is nullptr
 * @pre         None
 */
PASCO2Pulse::PASCO2Pulse(PWM   * const pwm,
                         Timer * const timer,
                         GPIO  * const interrupt,
                         GPIO  * const power3V3, 
                         GPIO  * const power12V,
                         GPIO  * const pwmSelect)
:PASCO2(nullptr, pwm, timer, interrupt, nullptr, power3V3, power12V, pwmSelect)
{
    
}

/**
 * @brief       PAS CO2 Pulse Destructor 
 * @pre         None
 */
PASCO2Pulse::~PASCO2Pulse()
{

}

/**
 * @brief       Enables the sensor
 *
 * @details     Initializes the controller peripheral interfaces and enable VDD
 *              and IR emitter power of the sensor
 *
 * @note        Optional call. Each API function will set internally the
 *              required status for its operation
 *
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      IC_POWERON_ERROR if power-on error
 * @pre         None
 */
Error_t PASCO2Pulse::enable()
{
    Error_t err = OK;

    PAS_CO2_PULSE_LOG_MSG(__FUNCTION__);
    do
    {   
        err = init();
        if (OK != err)
            break;

        err = enableIREmitter();
        if (OK != err)
            break;

        err = enableLogic();
        if (OK != err)
            break;
        
    } while (0);

    PAS_CO2_PULSE_LOG_RETURN(err);

    return err;
}  

/**
 * @brief       Disables the sensor
 *
 * @details     Initializes the controller peripheral interfaces and enable VDD
 *              and IR emitter power of the sensor
 *
 * @note        Optional call. Each API function will set internally the
 *              required status for its operation
 *
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2Pulse::disable()
{
    Error_t err = OK;
    
    PAS_CO2_PULSE_LOG_MSG(__FUNCTION__);
    do
    {   
        err = disableIREmitter();
        if (OK != err)
            break;

        err = disableLogic();
        if (OK != err)
            break;

        err = deinit();
        if (OK != err)
            break;

    } while (0);

    PAS_CO2_PULSE_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Triggers the internal measuring of the sensor 
 *
 * @details     The PWM select signal will be enabled if it is available. If
 *              this signal is fixed on hardware, the sensor will already output
 *              the PWM signal without the need of calling startMeasure().
 * 
 *              By default, if no other setup has been configured through the
 *              serial interface, a pulse train will be provided by the sensor 
 *              every 1 min.
 * 
 *              Optionally, if a callback is passed by argument, the interrupt 
 *              event will occur approximately 1 second before the measurement
 *              starts. This can be used for synching the pwm redouts on the 
 *              microcontroller. Some example:
 
  *             @code
 *              volatile bool intFlag = false;
 *              void cback(void *)
 *              {
 *                  intFlag = true;
 *              }
 * 
 *              PASCO2Pulse cotwo(pwm, timer, interrupt);
 *              int16_t   co2ppm;
 * 
 *              cotwo.startMeasure(cback);
 * 
 *              while(1)
 *              {
 *                  while(!intFlag) { // block or yield() };
 *                  cotwo.getCO2(co2ppm);   
 *                  // ... do something with the co2 value ... 
 *                  intFlag = false;
 *              }
 *              @endcode
 * 
* @warning      If no PWM select GPIO instance is available and no interrupt 
                GPIO instance, the function does nothing and returns without 
 *              error (OK)
 *
 * @param[in]   *cback      Pointer to the callback function to be called upon
 *                          interrupt. Default is nullptr
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      CONFIG_ERROR if invalid configuration parameter
 * @pre         None
 */
Error_t  PASCO2Pulse::startMeasure(void (*cback) (void *))
{
    Error_t err = OK;
    
    PAS_CO2_PULSE_LOG_MSG(__FUNCTION__);
    do
    {   
        Int_t intType = INT_EARLY_MEAS;

        /* Enable the pin to measure */   
        err = enablePWM();
        if (OK != err)
            break;
        
        if(cback != nullptr)
        {
            err = enableInterrupt(cback, intType, INT_IO_PUSH_PULL_ACTIVE_HIGH);
            if (OK != err)
                break;
        }
        else
        {
            if(nullptr != interrupt)
            {
                err = disableInterrupt();
                if (OK != err)
                    break;
            }
        }

    } while (0);

    PAS_CO2_PULSE_LOG_RETURN(err);
        
    return err;
}

/**
 * @brief       Stops the internal measuring of the sensor 
 * 
 * @details     Disables the PWM select signal
 * 
 * @warning     PWM select GPIO interface required. If unavailable the function
 *              does nothing and returns without error (OK) 
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t  PASCO2Pulse::stopMeasure()
{
    Error_t err = OK;

    PAS_CO2_PULSE_LOG_MSG(__FUNCTION__);

    err = disablePWM();

    PAS_CO2_PULSE_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Gets the CO2 concentration measured
 *
 * @details     The value read is zero when no measurement is yet available.
 *              In case of error, the read value is set to -1
 *
 * @param[out]  co2ppm  CO2 concentration read (in ppm)
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2Pulse::getCO2(int16_t & co2ppm)
{
    Error_t err = OK;
    
    PAS_CO2_PULSE_LOG_MSG(__FUNCTION__);
    do
    {   
        co2ppm = 0;

        /* Read the data */
        err = PASCO2::getCO2(co2ppm);
        if (OK != err)
        {
            co2ppm = -1;
            break;
        }

    } while (0);

    PAS_CO2_PULSE_LOG_RETURN(err);
      
    return err;
}

/**
 * @brief       Resets the sensor
 *
 * @details     The hardware reset will disable 3V3 power output for 200 ms to
 *              perform a hardware reset
 *
 * @warning     Power 3V3 GPIO interface required.  If unavailable the function
 *              does nothing and returns without error (OK) 
 *
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      RESET_ERROR if the device reset error
 * @pre         None
 */
Error_t PASCO2Pulse::reset()
{
    Error_t err = OK;

    PAS_CO2_PULSE_LOG_MSG(__FUNCTION__);

    err = hardReset();

    PAS_CO2_PULSE_LOG_RETURN(err);
    
    return err;
}

#endif /** PAS_CO2_INTF **/