/** 
 * @file        pas-co2-serial.cpp
 * @brief       PAS CO2 Serial API 
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-serial.hpp"

#if IS_INTF(PAS_CO2_INTF_SERIAL)

#include "pas-co2-logger.hpp"

using namespace pasco2;

/**
 * @brief      PAS CO2 Serial Constructor
 * 
 *   Mandatory arguments: 
 *      - The SBus needs to be provided
 *      - A timer instance (currently default as nullptr.TODO: Needs to be changed!)
 * 
 *   Optional  arguments: 
 *      - All GPIO instances.
 * 
 *      Most of the GPIO will be fixed by hardware. In particular those enabling 
 *      the sensor main power and the emitter power. 
 *      They are part of the sensor library to provide potential absolute control 
 *      by software.
 *      The hardware interface and its features, are also provided in the library.
 *      But it can be as well handled externally 
 * 
 * @param[in]   *sbus           Serial bus instance
 * @param[in]   *timer          Timer instance
 * @param[in]   *interrupt      Interrupt GPIO instance. Default is nullptr         
 * @param[in]   *protoSelect    Protocol select GPIO instance. Default is nullptr
 * @param[in]   *power3V3       Power 3V3 control GPIO instance. Default is nullptr
 * @param[in]   *power12V       Power 12V control GPIO instance. Default is nullptr
 * @pre         None
 */
PASCO2Serial::PASCO2Serial(pasco2::SBus  * const sbus,
                           pasco2::Timer * const timer,
                           pasco2::GPIO  * const interrupt, 
                           pasco2::GPIO  * const protoSelect,
                           pasco2::GPIO  * const power3V3,
                           pasco2::GPIO  * const power12V)
:PASCO2(sbus, nullptr, timer, interrupt, protoSelect, power3V3, power12V, nullptr)
{

}

/**
 * @brief       PAS CO2 Serial Destructor 
 * @pre         None
 */
PASCO2Serial::~PASCO2Serial()
{

}

/**
 * @brief       Enables the sensor
 * 
 * @details     Initializes the controller peripheral interfaces and enable 
 *              VDD and IR emitter power of the sensor.
 * 
 * @note        Optional call. Each API function will set internally 
 *              the required status for its operation
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      IC_POWERON_ERROR if power-on error
 * @pre         None
 */
Error_t PASCO2Serial::enable()
{
    Error_t err = OK;

    PAS_CO2_SERIAL_LOG_MSG(__FUNCTION__);
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

    PAS_CO2_SERIAL_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Disables the sensor
 * 
 * @details     Initializes the controller peripheral interfaces and enable 
 *              VDD and IR emitter power of the sensor
 * 
 * @note        Optional call. Each API function will set internally 
 *              the required status for its operation
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2Serial::disable()
{
    Error_t err = OK;
    
    PAS_CO2_SERIAL_LOG_MSG(__FUNCTION__);
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

    PAS_CO2_SERIAL_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Triggers the internal measuring of the sensor 
 * 
 * @details     The function start the measurement controlling the different
 *              sensor modes and features depending on the configured arguments.
 *              
 *              Single shot
 *              ---------------------------------------------------------------
 *              If the function is called with no arguments, the sensor
 *              will be trigger to perform a single shot measurement. 
 *              The users needs to poll with getCO2() until the CO2 value is 
 *              available and has been readed from the sensor.
 *              The CO2 concentration value read will be zero as long as 
 *              no value is avaiable, of -1 if any error occurred in the 
 *              readout attempt. 
 *              Polling example:
 * 
 *              @code
 *              PASCO2Serial cotwo(sbus, timer); 
 *              int16_t   co2ppm;
 * 
 *              cotwo.startMeasure();
 * 
 *              do{ cotwo.getCO2(co2ppm); } while (co2ppm <= 0);  
 *              @endcode
 * 
 *              Periodic measurement
 *              ---------------------------------------------------------------
 *              Periodic measurements (periodInSec) will configure the sensor
 *              to perform a measurements every desired period. Between 5 and
 *              4095 seconds.
 *              Without further arguments, the user has to poll with getCO2()
 *              until the value is available. Any super loop or thread 
 *              routine, can just consist on reading the CO2 (getCO2()). 
 *              For example, measure every 5 minutes:
 * 
 *              @code
 *              PASCO2Serial cotwo(sbus, timer);
 *              int16_t   co2ppm;
 * 
 *              cotwo.startMeasure(300);
 * 
 *              while(1)
 *              {
 *                  delay(300);
 *                  do{ cotwo.getCO2(co2ppm); } while (co2ppm <= 0);  
 *                  // ... do something with the co2 value ... 
 *              }
 *              @endcode
 *
 *              Synching readouts with the hardware interrupt
 *              ---------------------------------------------------------------
 *              In order not to saturate the sensor with constant serial 
 *              requests, especially in periodic mode, it is recommended 
 *              to synch the readout with a timer. Or even better using 
 *              the hardware GPIO hardware interrupt.
 *              If the interrupt GPIO interface has been provided, passing
 *              a callback function will enable the interrupt mode. The 
 *              type of interrupt is decided depending on the value of the
 *              rest of the arguments and operations modes.
 *              Some example:
 * 
 *              @code
 *              volatile bool intFlag = false;
 *              void cback(void *)
 *              {
 *                  intFlag = true;
 *              }
 * 
 *              PASCO2Serial cotwo(sbus, timer, interrupt);
 *              int16_t   co2ppm;
 * 
 *              cotwo.startMeasure(300,0,cback);
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
 *              Alarm mode
 *              ---------------------------------------------------------------
 *              If the alarm threshold argument is non-zero, the alarm mode 
 *              is activated, and an the sensor internal flag will be eanbled 
 *              if the concentration of CO2 goes above the specified value.
 *              This option is better combined with the interupt mode. Thus,
 *              if the interrupt mode is available and a callback function
 *              is passed, the interrupt will occurr only when the co2 
 *              concentration goes above the threshold. 
 *              This makes mostly sense for periodic measurement configuration. 
 *              But it can be used as well for a single shot configuration
 *                 
 * @param[in]   periodInSec Enables periodic measurement with the specified period. 
 *                          The default, value is 0, meaning single shot operation. 
 *                          The valid period range goes between 5 and 4095 seconds
 * @param[in]   alarmTh     Enables upper alarm threshold mode for the specified
 *                          value. 
 *                          The default value is 0, meaning no alarm mode. 
 *                          For any non-zero value, the sensor will internally set 
 *                          the alarm flag. If an interrupt callback function is 
 *                          provided, then the interrupt will occurr only when the 
 *                          defined threshold has been tresspassed
 * @param[in]   *cback      Pointer to the callback function to be called upon
 *                          interrupt
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      CONFIG_ERROR if invalid configuration parameter
 * @pre         None
 */
Error_t PASCO2Serial::startMeasure(int16_t periodInSec, int16_t alarmTh, void (*cback) (void *))
{
    Error_t err = OK;
    
    PAS_CO2_SERIAL_LOG_MSG(__FUNCTION__);
    do
    {   
        OpMode_t mode = MODE_SINGLE_SHOT;
        Int_t intType = INT_DATA_READY;

        if( periodInSec > 0 )
        {
            err = setMeasPeriod(periodInSec);
            if (OK != err)
                break;
            
            mode = MODE_CONTINUOUS_MEAS;
        }

        if( alarmTh >  0 )
        {
            err = enableAlarm(alarmTh, ALARM_TH_CROSS_UP);
            if (OK != err)
                break;
            
            intType = INT_ALARM_ACTIVE;
        }
        else
        {
            err = disableAlarm();
            if (OK != err)
                break;
        }

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

        err = setOperationMode(mode);
        if (OK != err)
                break;

    } while (0);

    PAS_CO2_SERIAL_LOG_RETURN(err);
        
    return err;
}

/**
 * @brief       Stops the internal measuring of the sensor 
 * 
 * @details     Sets operation mode to idle
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2Serial::stopMeasure()
{
    Error_t err = OK;
    
    PAS_CO2_SERIAL_LOG_MSG(__FUNCTION__);
    do
    {   
        err = setOperationMode(MODE_IDLE);
        if (OK != err)
                break;

    } while (0);

    PAS_CO2_SERIAL_LOG_RETURN(err);
       
    return err;
}

/**
 * @brief       Gets the CO2 concentration measured
 *               
 * 
 * @details     The value read is zero when no measurement is 
 *              yet available.
 *              In case of error, the read value is set to -1.
 *              Before reading the co2 value, the measurement 
 *              status register is read to know if a new 
 *              measurement data is available.
 * 
 * @param[out]  co2ppm  CO2 concentration read (in ppm)
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2Serial::getCO2(int16_t & co2ppm)
{
    Error_t err = OK;
    
    PAS_CO2_SERIAL_LOG_MSG(__FUNCTION__);
    do
    {   
        co2ppm = -1;
        /* Check if data is ready */
        MeasStatus_t measSt; 
        err = getMeasStatus(measSt);
        if (OK != err)
            break;

        /* If data was not ready, it returns with CO2 pmm = 0*/
        if(!measSt.dataRdy)
        {   
            co2ppm = 0;
            break;  
        }

        /* Read the data */
        err = PASCO2::getCO2(co2ppm);
        if (OK != err)
            break;

    } while (0);

    PAS_CO2_SERIAL_LOG_RETURN(err);
      
    return err;
}

/**
 * @brief       Gets diagnosis information
 *  
 * @details     The sensor status registers includes the following flags:
 *              - Sensor ready
 *              - PWM pin enabled
 *              - Temperature out of range error
 *              - IR emitter voltage out of range error
 *              - Communication error 
 *              which will be stored in the Diag_t struct varible passed by argument.
 *              After reading the flags, these are cleared in the device writing in 
 *              the corresponding clear flag bitfields.
 * 
 * @param[out]  diagnosis  Struct to store the diagnosis flags values   
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2Serial::getDiagnosis(Diag_t & diagnosis)
{
    Error_t err = OK;

    PAS_CO2_SERIAL_LOG_MSG(__FUNCTION__);

    err = PASCO2::getDiagnosis(diagnosis);

    PAS_CO2_SERIAL_LOG_RETURN(err);
    
    return err;
}

/**
 * @brief       Calibrates the sensor
 *               
 * 
 * @details     Configures the automatic baseline and the 
 *              reference pressure compensation
 * 
 * @param[in]   aboc        Automatic baseline compenstation mode  
 * @param[in]   abocRef     Automatic baseline compensation reference
 * @param[in]   pressRef    Pressure reference value. Min value is 600, and max 1600.
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      CONFIG_ERROR if pressure reference value is invalid
 * @pre         None
 */
Error_t PASCO2Serial::calibrate(ABOC_t aboc, int16_t abocRef, uint16_t pressRef)
{
    Error_t err = OK;
     
    PAS_CO2_SERIAL_LOG_MSG(__FUNCTION__);
    do
    {   
        err = enableABOCompensation(aboc, abocRef);
        if (OK != err)
            break;

        err = setPressureCompensation(pressRef);
        if (OK != err)
            break;

    } while (0);

    PAS_CO2_SERIAL_LOG_RETURN(err);
       
    return err;
}

/**
 * @brief       Resets the sensor
 * 
 * @details     The software reset performs the following operations:
 *              - Read the current sensor memory register map configuration
 *              - Send the corresponding software reset serial command
 *              - Rewrite the previous memory map configuration after reset 
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      RESET_ERROR if the device reset error
 * @pre         None
 */
Error_t PASCO2Serial::reset()
{
    Error_t err = OK;

    PAS_CO2_SERIAL_LOG_MSG(__FUNCTION__);

    err = softReset();

    PAS_CO2_SERIAL_LOG_RETURN(err);
    
    return err;
}

/**
 * @brief       Gets device product identifier 
 *  
 * @param[out]  prodID  Product identifier
 * @param[out]  revID   Version identifier
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2Serial::getDeviceID(uint8_t & prodID, uint8_t & revID)
{
    Error_t err = OK;
     
    PAS_CO2_SERIAL_LOG_MSG(__FUNCTION__);

    err = PASCO2::getDeviceID(prodID, revID);

    PAS_CO2_SERIAL_LOG_RETURN(err);
       
    return err;
}

#endif /** PAS_CO2_INTF **/