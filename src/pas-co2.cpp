/** 
 * @file        pas-co2.cpp
 * @brief       PAS CO2 Core API
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2.hpp"
#include "pas-co2-logger.hpp"

using namespace pasco2;

/**
 * @brief      PAS CO2 Constructor
 * 
 * @details    Mandatory arguments: 
 *              - Either the SBus or the PWM interface need to be provided. Both can be we as well provided, but the main CO2 reading interface will be the serial interface then
 *              - A timer instance (currently default as nullptr.TODO: Needs to be changed!)
 * 
 *              Optional  arguments: 
 *              - All GPIO instances
 * 
 *              Most of the GPIO will be fixed by hardware. In particular those enabling 
 *              the sensor main power and the emitter power. 
 *              They are part of the sensor library to provide potential absolute control 
 *              by software.
 *              The hardware interface and its features, are also provided in the library.
 *              But it can be as well handled externally 
 * 
 * @param[in]   *sbus           Serial bus instance
 * @param[in]   *pwm            Pwm instance
 * @param[in]   *timer          Timer instance
 * @param[in]   *interrupt      Interrupt GPIO instance. Default is nullptr         
 * @param[in]   *protoSelect    Protocol select GPIO instance. Default is nullptr
 * @param[in]   *power3V3       Power 3V3 control GPIO instance. Default is nullptr
 * @param[in]   *power12V       Power 12V control GPIO instance. Default is nullptr
 * @param[in]   *pwmSelect      Pwm selection GPIO instalce. Default is nullptr
 * @pre         None
 */
PASCO2::PASCO2(pasco2::SBus  * const sbus,
               pasco2::PWM   * const pwm,
               pasco2::Timer * const timer,
               pasco2::GPIO  * const interrupt,
               pasco2::GPIO  * const protoSelect,
               pasco2::GPIO  * const power3V3,
               pasco2::GPIO  * const power12V,
               pasco2::GPIO  * const pwmSelect)
: sbus(sbus),
  pwm(pwm),
  timer(timer), 
  interrupt(interrupt),  
  protoSelect(protoSelect),
  power3V3(power3V3),
  power12V(power12V),
  pwmSelect(pwmSelect),
  reg(),
  status(UNINITED)
{

}

/**
 * @brief       PAS CO2 Destructor
 * @warning     It calls deinit() without exception handling. In order to catch any
 *              potential exception, the user must explicitly call deinit() before the 
 *              destructor is called 
 * @pre         None
 */
PASCO2::~PASCO2()   
{
    deinit();
}

/**
 * @brief       Initializes the sensor hardware interfaces
 *
 * @note        The PAL provides init() and deinit() functions for each 
 *              platform abstraction layer interface. 
 *              Depending on the system design criteria, the init/deinitialization of these 
 *              interfaces might be done at OS level, and externally managed. 
 *              These functions will be then empty calls and always return successfully (OK).
 *              If the init/deinitialization is delegated to the PAS CO2 instance itself, 
 *              or handled externally is up to the user. The library supports both scenarios.                   
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::init()
{
    Error_t err = OK;

    PAS_CO2_LOG_INIT();
    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* If hardware peripheral interfaces are inited. Return */
        if(INITED == (status & INITED))
        {
            break;
        }

        /* Either SBus or PWM has to be available*/
        if( nullptr == sbus && nullptr == pwm)
        {
            err = INTF_ERROR;
            break;
        }

        /* Initialize sbus*/
        if (nullptr != sbus)
        {
            err = sbus->init();
            if (OK != err)
                break;

            /* Initialize protocol select*/
            if (nullptr != protoSelect)
            {
                err = protoSelect->init();
                if (OK != err)
                    break;
                    
                /* Select prototocol with PSEL Pin */ 
                SBus::Proto_t proto = sbus->getProtocol();

                if(SBus::Proto_t::BUS_PROTO_I2C == proto)
                {
                    err = protoSelect->write(GPIO::VLevel_t::GPIO_LOW);
                }
                else if(SBus::Proto_t::BUS_PROTO_UART == proto)
                {
                    err = protoSelect->write(GPIO::VLevel_t::GPIO_HIGH);
                }
                if (OK != err)
                    break;
            }
        }

        /* Initialize pwm */
        if (nullptr != pwm)
        {
            err = pwm->init();
            if (OK != err)
                break;
        }
        
        /* Initialize timer*/
        if (nullptr != timer)
        {
            err = timer->init();
            if (OK != err)
                break;
        }

        /* Initialize interrupt*/
        if (nullptr != interrupt)
        {
            err = interrupt->init();
            if (OK != err)
                break;
        }
        
        /* Initialize Power Supply Pin 3V3*/
        if (nullptr != power3V3)
        {
            err = power3V3->init();
            if (OK != err)
                break;
        }

        /* Initialize Power Supply Pin 12V */
        if (nullptr != power12V)
        {
            err = power12V->init();
            if (OK != err)
                break;
        }

        /* Initialize PWM Disable */
        if (nullptr != pwmSelect)
        {
            err = pwmSelect->init();
            if (OK != err)
                break;
        }

        status = INITED;

    } while (0);

    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Deinitializes the sensor hardware interfaces
 *      
 * @note        The PAL provides init() and deinit() functions for each 
 *              platfom abstraction layer interface. 
 *              Depending on the system design criteria, the init/deinitialization of these 
 *              interfaces might be done at OS level, and externally managed. 
 *              These functions will be then empty calls and always return successfully (OK).
 *              If the init/deinitialization is delegated to the PAS CO2 instance itself, 
 *              or handled externally is up to the user. The library supports both scenarios. 
 *
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::deinit()
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        if(UNINITED == status)
        {
            break;
        }

        /* Function requires sensor peripheral inited (or off) */
        err = setStatus(INITED);
        if(OK != err)
            break;

        /* Deinitialize sbus*/
        if (nullptr != sbus)
        {
            err = sbus->deinit();
            if (OK != err)
                break;
        }
        
        /* Deinitialize pwm */
        if (nullptr != pwm)
        {
            err = pwm->deinit();
            if (OK != err)
                break;
        }

        /* Deinitialize timer*/
        if (nullptr != timer)
        {
            err = timer->deinit();
            if (OK != err)
                break;
        }

        /* Deinitialize interrupt*/
        if (nullptr != interrupt)
        {
            err = interrupt->deinit();
            if (OK != err)
                break;
        }

        /* Deinitialize protocol select*/
        if (nullptr != protoSelect)
        {
            err = protoSelect->deinit();
            if (OK != err)
                break;
        }

        /* Deinitialize Power Supply Pin 3V3*/
        if (nullptr != power3V3)
        {
            err = power3V3->deinit();
            if (OK != err)
                break;
        }

        /* Deinitialize Power Supply Pin 12V */
        if (nullptr != power12V)
        {
            err = power12V->deinit();
            if (OK != err)
                break;
        }

        /* Deinitialize PWM Disable */
        if (nullptr != pwmSelect)
        {
            err = pwmSelect->deinit();
            if (OK != err)
                break;
        }

        status = Status_t(status & ~INITED);
    } while (0);
    PAS_CO2_LOG_RETURN(err);
    PAS_CO2_LOG_DEINIT();

    return err;
}

/**
 * @brief       Enables the sensor logic
 * 
 * @details     This function performs the following operations:
 *              - Initializes the hardware interfaces (if not yet initialized)
 *              - Enables the sensor 3V3 power (if the instance has been provided for software power enablement)
 *              - Reads the register map of the sensor (if the serial interface has been provided)
 *              The sensor status is updated (xored) with the LOGIC_ONLY_ON mask. 
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      IC_POWERON_ERROR if power-on error
 * @pre         None
 */
Error_t PASCO2::enableLogic()
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* If Power3V3 is already enabled */
        if(POWER3V3 == (status & POWER3V3))
        {
            break;
        }

        /* Function requires sensor peripheral 
           or with in Ir emitter enabled only status*/
        err = setStatus(INITED, true);
        if(OK != err)
            break;

        if(nullptr != power3V3)
        {
            /* Enable digital circuit power supply 3V3 */
            err = power3V3->enable();
            if (OK != err)
                break;
        }

        if(nullptr != sbus)
        {   
            /* TODO. Adjust Wait startup time.*/
            if(nullptr != timer)
            {
                err = timer->delay(200);
                if (OK != err)
                    break;
            }

            /* Read default startup/reset register values */
            err = sbus->read(Reg::REG_ADDR_PROD_ID, &(reg.regMap[Reg::REG_ADDR_PROD_ID]), reg.regCount);
            if (OK != err)
                break;

            if(!reg.isSensorReady())
            {
                  err = IC_POWERON_ERROR;
                  break;
            }
        }

        status = Status_t (status | POWER3V3);  

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Disables the sensor logic
 * 
 * @details     Disables the sensor 3V3 power if the instance has been provided for software power enablement.
 *              Otherwise, just updates the sensor status disabling POWER3V3 flag.
 *  
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::disableLogic()
{
   Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {        
        if(nullptr == power3V3)
        {
            status = (Status_t)(status & ~POWER3V3);
            break;
        }

        /* If power 3V3 is already disabled */
        if(0 == (status & POWER3V3))
        {
            break;
        }

        /* Disable digital circuit power supply 3V3 */
        err = power3V3->disable();
        if (OK != err)  
            break;

        status = (Status_t)(status & ~POWER3V3);   
   
    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Enables the IR emitter
 * 
 * @details     This function performs the following operations:
 *              - Initializes the hardware interfaces (if not yet initialized)
 *              - Enables the sensor 12V power (if the instance has been provided
 *                for software IR emitter power enablement)
 * 
 *              The sensor status is updated (xored) with the IR_ONLY_ON mask. 
 *  
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::enableIREmitter()
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* If Power12V is already enabled */
        if(POWER12V == (status & POWER12V))
        {
            break;
        }

        /* Function requires sensor logic on */
        err = setStatus(INITED, true);
        if(OK != err)
            break;
  
        if(nullptr != power12V)
        {
            /* Enable IR Emitter power supply 12V */
            err = power12V->enable();
            if (OK != err)
                break;
        }

        status = (Status_t)(status | POWER12V);  
    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Disables the IR emitter
 * 
 * @details     Disables the sensor 12V power if the instance has been provided for 
 *              software IR emitter power enablement.
 *              Otherwise, just updates the sensor status disabling POWER12V flag.
 *  
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::disableIREmitter()
{
   Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        if(nullptr == power12V)
        {
            status = (Status_t)(status & ~POWER12V); 
            break;
        }

        /* If emitter power is already disabled */
        if(0 == (status & POWER12V))
        {
            break;
        }
        
        /* Disable IR Emitter power supply 12V */
        err = power12V->disable();
        if (OK != err)
            break;

        status = (Status_t)(status & ~POWER12V); 
    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Resets the sensor via the serial interface
 * 
 * @details     The software reset performs the following operations:
 *              - Read the current sensor memory register map configuration
 *              - Send the corresponding software reset serial command
 *              - Rewrite the previous memory map configuration after reset 
 * 
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      RESET_ERROR if the device reset error
 * @pre         None
 */
Error_t PASCO2::softReset()
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        /* Save current configuration */
        err = preResetSaveConfig();
        if (OK != err)
            break;

        /* Perform soft reset */
        reg.setSoftReset();

        err = sbus->write(Reg::REG_ADDR_SENS_RST, &(reg.regMap[Reg::REG_ADDR_SENS_RST]));
        /* In case of UART interface, there is no ACK for this command.
           It will return error. This verification is omitted for UART.*/
        if(SBus::Proto_t::BUS_PROTO_I2C == sbus->getProtocol())
        {
            if (OK != err)
                break;
        }

       /* Delay TODO: set right timing. Is this required? */
       if(nullptr != timer)
       {
            err = timer->delay(2000);
            if (OK != err)
                break;
        }
        /* Verify reset and restore prior reset configuration */
        err = postResetRestoreConfig();
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Resets the sensor via the logic 3V3 power GPIO interface
 * 
 * @details     The software reset performs the following operations:
 *              - Read the current sensor memory register map configuration (if the serial interface has been provided)
 *              - Disables the power 3V3 GPIO interface
 *              - Time delay of 200 meas
 *              - Enables the power 3V3 GPIO interface
 *              - Rewrite the previous memory map configuration after reset (if the serial interface has been provided)
 * 
 * @warning     Power 3V3 GPIO interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      RESET_ERROR if the device reset error
 * @pre         None
 */
Error_t PASCO2::hardReset()
{
   Error_t err = OK;
    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        if(nullptr == power3V3)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        /* If the serial interface is available*/
        if(nullptr != sbus)
        { 
            /* Save current configuration */
            err = preResetSaveConfig();
            if (OK != err)
                break;
        }
        
        /* Perform hard reset */
        err = power3V3->disable();
        if (OK != err)
            break;

        /* Delay TODO: set right timing */
        if(nullptr != timer)
        {
            err = timer->delay(200);
            if (OK != err)
                break;
        }

        err = power3V3->enable();
        if (OK != err)
            break;

        /* If the serial interface is available*/
        if(nullptr != sbus)
        {

            /* Wait for startup TODO: set right timing */
            if(nullptr != timer)
            {
                err = timer->delay(200);
                if (OK != err)
                    break;
            }

            /* Verify reset and restore prior reset configuration */
            err = postResetRestoreConfig();
            if (OK != err)
                break;
        }

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Sets the sensor operation mode
 * 
 * @details     The sensor supports 3 operating modes:
 *              - Idle mode. Performs no measurement.
 *              - Single shot mode. Triggers a measurement and afterwards goes back to idle mode.
 *              - Continuous mode. Performs measurements periodically according to the register
 *              measurement configuration.
 *              The initial default mode is the idle mode. If only the PWM interface is used, then
 *              the default mode configuration is continuous (as there is no register configuration
 *              feasible)
 * 
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @param[in]   mode    Operation mode 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::setOperationMode(OpMode_t mode)
{
   Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the serial interface available*/
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        reg.setOperationMode(mode);

        err = sbus->write(Reg::REG_ADDR_MEAS_CFG, &(reg.regMap[Reg::REG_ADDR_MEAS_CFG]));
        if (OK != err)
            break;


    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Enables the PWM output
 * 
 * @details     PWM is enabled by default via the pwmSelect GPIO instance if provided. 
 *              Otherwise, the serial interface is used.
 *              
 * @warning     PWM interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @warning     If no interface for software enablement is provided, 
 *              the hardware configuration must ensure the PWM select sensor GPIO is 
 *              properly configured as PWM enabled.
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::enablePWM()
{
   Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the pwm interface available*/
        if(nullptr == pwm)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        if(nullptr != pwmSelect)
        {
            err = pwmSelect->enable();
            if (OK != err)
                break;
        }
        else if(nullptr != sbus)
        {
            reg.enablePWMOutput();

            err = sbus->write(Reg::REG_ADDR_MEAS_CFG, &(reg.regMap[Reg::REG_ADDR_MEAS_CFG]));
            if (OK != err)
                break;

        }

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Disables the PWM output
 * 
 * @details     PWM is disabled by default via the pwmSelect GPIO instance if provided. 
 *              Otherwise, the serial interface is used.
 *              
 * @warning     PWM interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @warning     If no interface for software disablement is provided, 
 *              the hardware configuration must ensure the PWM select sensor GPIO is 
 *              properly configured as PWM disabled.
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::disablePWM()
{
   Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the pwm interface available*/
        if(nullptr == pwm)
        {
            break;
        }

        /* Function requires sensor at least logic on*/    
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        if(nullptr != pwmSelect)
        {
            err = pwmSelect->disable();
            if (OK != err)
                break;
        }
        else if(nullptr != sbus)
        {
            reg.disablePWMOutput();

            err = sbus->write(Reg::REG_ADDR_MEAS_CFG, &(reg.regMap[Reg::REG_ADDR_MEAS_CFG]));
            if (OK != err)
                break;
        }

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Sets the PWM pulse mode
 * 
 * @details     The sensor supports 2 PWM pulse modes:
 *              - Single pulse. A single PWM pulse is generated before the device goes inactive.
 *              - Pulse train.  A train of 160 pulses is issued before the device goes inactive.
 *              
 *              By default, pulse train mode is enabled.             
 * 
 * @warning     PWM and serial interfaces required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @param[in]   mode    PWM pulse mode
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::setPWMMode(PWMMode_t mode)
{
   Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the serial and pwm interface available*/
        if(nullptr == sbus || nullptr == pwm)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        reg.setPWMMode(mode);

        err = sbus->write(Reg::REG_ADDR_MEAS_CFG, &(reg.regMap[Reg::REG_ADDR_MEAS_CFG]));
        if (OK != err)
        break;

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Sets the measurement period
 * 
 * @details     In continuous mode, the sensor will perform measurement with this period.         
 * 
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @param[in]   periodInSec Measurement period in seconds. Min period is 5 second, and max 4095 seconds.
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      CONFIG_ERROR if period value is invalid
 * @pre         None
 */
Error_t PASCO2::setMeasPeriod(int16_t periodInSec)
{
  Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the serial interface available*/
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        /* Measurement period has to be within [5, 4095] sec */
        if( periodInSec > periodMeasMax || periodInSec < periodMeasMin)
        {
            err = CONFIG_ERROR;
            break;
        }

        reg.setMeasRate(periodInSec);

        err = sbus->write(Reg::REG_ADDR_MEAS_RATE_H, &(reg.regMap[Reg::REG_ADDR_MEAS_RATE_H]), 2);
        if (OK != err)
            break;

        /* Verify proper register operation */
        err = sbus->read(Reg::REG_ADDR_SENS_STS, &(reg.regMap[Reg::REG_ADDR_SENS_STS]));
        if (OK != err)
            break;

    } while (0);

    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Gets the CO2 concentration readout value from the sensor
 *  
 * @details     If both the serial and the pwm interfaces are provided, the
 *              serial interface will be used.
 * 
 * @param[out]  co2ppm  Variable to store the read co2 concentration value (in ppm)  
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::getCO2(int16_t & co2ppm)
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires sensor logic and ir emitter on*/
        err = setStatus(ON);
        if(OK != err)
            break;

        if(nullptr != sbus)
        {
            err = sbus->read(Reg::REG_ADDR_CO2PPM_H, &(reg.regMap[Reg::REG_ADDR_CO2PPM_H]), 2); 
            if (OK != err)
                break;

            co2ppm = reg.getCO2PPM();

        }
        else if(nullptr != pwm)
        {
            double duty = 0.0;
            err = pwm->getDuty(duty);
            if (OK != err)
                break;

            co2ppm = (int16_t)(100*duty);   /** 10000/100 */
        }

    } while (0);

    PAS_CO2_LOG_RETURN(err);

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
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @param[out]  diagnosis  Struct to store the diagnosis flags values   
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::getDiagnosis(Diag_t & diagnosis)
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        err = sbus->read(Reg::REG_ADDR_SENS_STS, &(reg.regMap[Reg::REG_ADDR_SENS_STS]));
        if (OK != err)
            break;

        diagnosis.sensorRdy           = reg.isSensorReady();
        diagnosis.pwmPinEnabled       = reg.isPWMPinEnabled();
        diagnosis.outOfRangeTempError = reg.isTempOutofRange();
        diagnosis.outOfRange12VError  = reg.is12VOutofRange();
        diagnosis.commError           = reg.hasCommErrorOccurred();

        reg.clearTempOutofRangeFlag();
        reg.clear12VOutofRangeFlag();
        reg.clearCommErrorFlag();

        err = sbus->write(Reg::REG_ADDR_SENS_STS, &(reg.regMap[Reg::REG_ADDR_SENS_STS]));
        if (OK != err)
            break;

    } while (0);

    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Gets measurement status information
 *  
 * @details     The measurement status register provides the following information:
 *              - Data ready. After a measurement has been completed.
 *              - Interrupt active. After an interrupt event has occurred.
 *              - Alarm active. If the alarm has been activated.
 *              which will be stored in the MeasStatus_t struct varible passed by argument.
 *              After reading the flags, these are cleared in the device writing in 
 *              the corresponding clear flag bitfields.
 * 
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @param[out]  measSt  Struct to store the measurement status flags values  
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::getMeasStatus(MeasStatus_t & measSt)
{
    Error_t err = OK;
    
    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        err = sbus->read(Reg::REG_ADDR_MEAS_STS, &(reg.regMap[Reg::REG_ADDR_MEAS_STS]));
        if (OK != err)
            break;

        measSt.dataRdy     = reg.isDataReady();
        measSt.intActive   = reg.hasInterruptOcurred();
        measSt.alarmActive = reg.isAlarmSet();

        reg.clearInterruptFlag();
        reg.clearAlarmFlag();

        err = sbus->write(Reg::REG_ADDR_MEAS_STS, &(reg.regMap[Reg::REG_ADDR_MEAS_STS]));
        if (OK != err)
            break;
        
    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Enables the hardware interrupt
 *  
 * @warning     For event triggering and IO configuration the serial interface required.
 *              If unavailable the default interrupt the default values are INT_EARLY_MEAS
 *              and INT_IO_PUSH_PULL_ACTIVE_HIGH, respectively.
 * 
 * @warning     Interrupt GPIO interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @param[in]   *cback  Pointer to the interrupt callback function 
 * @param[in]   intType Interrupt event  
 * @param[in]   ioConf  Interrupt output voltage logic configuration
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::enableInterrupt(void (*cback) (void *), Int_t intType,  IntIOConf_t ioConf)
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        if(nullptr == interrupt)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        /* Register configuration requires the serial interface available*/
        if(nullptr != sbus)
        {
            reg.setInterrutFunc(intType);
            reg.setInterruptIOConf(ioConf);

            err = sbus->write(Reg::REG_ADDR_INT_CFG, &(reg.regMap[Reg::REG_ADDR_INT_CFG]));
            if (OK != err)
                break;
        }

        if(intType > INT_DISABLED)
        {
            if(nullptr != cback)
            {
                err = interrupt->enableInt(cback);
                if (OK != err)
                    break;
            }
        }

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Disables the hardware interrupt
 *  
 * @details     If the serial interface is available the interupt output will be as
 *              well disabled on the sensor. Otherwise, just the controller hardware
 *              GPIO interrupt is disabled.
 * 
 * @warning     Interrupt GPIO interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::disableInterrupt()
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        if(nullptr == interrupt)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        /* Function requires the serial interface available*/
        if(nullptr != sbus)
        {
            reg.setInterrutFunc(INT_DISABLED);

            err = sbus->write(Reg::REG_ADDR_INT_CFG, &(reg.regMap[Reg::REG_ADDR_INT_CFG]));
            if (OK != err)
                break;
        }

        err = interrupt->disableInt();
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Enables the automatic baseline compensation
 *  
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @param[in]   aboc    Automatic baseline compenstation mode  
 * @param[in]   abocRef Automatic baseline compensation reference
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::enableABOCompensation(ABOC_t aboc, int16_t abocRef)
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the serial interface available*/
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        reg.setABOCMode(aboc);

        err = sbus->write(Reg::REG_ADDR_MEAS_CFG, &(reg.regMap[Reg::REG_ADDR_MEAS_CFG]));
        if (OK != err)
            break;

        reg.setABOCReference(abocRef);

        err = sbus->write(Reg::REG_ADDR_CALIB_REF_H, &(reg.regMap[Reg::REG_ADDR_CALIB_REF_H]), 2);
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Disables the automatic baseline compensation
 *  
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::disableABOCompensation()
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the serial interface available*/
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        reg.setABOCMode(ABOC_DISABLED);

        err = sbus->write(Reg::REG_ADDR_MEAS_CFG, &(reg.regMap[Reg::REG_ADDR_MEAS_CFG]));
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Sets the pressure compensation reference 
 *  
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @param[in]   pressRef    Pressure reference value. Min value is 600, and max 1600.  
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @retval      CONFIG_ERROR if pressure reference value is invalid
 * @pre         None
 */
Error_t PASCO2::setPressureCompensation(uint16_t pressRef)
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the serial interface available*/
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        /* Referece pressure has to be within [600, 1600] hPa */
        if( pressRef > pressRefMax || pressRef < pressRefMin)
        {
            err = CONFIG_ERROR;
            break;
        }

        reg.setPressCompReference(pressRef);

        err = sbus->write(Reg::REG_ADDR_PRESS_REF_H, &(reg.regMap[Reg::REG_ADDR_PRESS_REF_H]), 2);
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Enables alarm mode 
 * 
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @param[in]   thres       Alarm CO2 concentration threshold (in ppm)
 * @param[in]   alarmType   Upper or lower threshold configuration
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::enableAlarm(int16_t thres, Alarm_t alarmType)
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the serial interface available*/
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        reg.setAlarmType(alarmType);

        err = sbus->write(Reg::REG_ADDR_INT_CFG, &(reg.regMap[Reg::REG_ADDR_INT_CFG]));
        if (OK != err)
            break;

        reg.setAlarmThreshold(thres);

        err = sbus->write(Reg::REG_ADDR_ALARM_TH_H, &(reg.regMap[Reg::REG_ADDR_ALARM_TH_H]), 2);
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Disables alarm mode 
 * 
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::disableAlarm()
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the serial interface available*/
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        reg.setAlarmThreshold(0x0000);

        err = sbus->write(Reg::REG_ADDR_ALARM_TH_H, &(reg.regMap[Reg::REG_ADDR_ALARM_TH_H]), 2);
        if (OK != err)
            break;

        reg.setAlarmType(ALARM_TH_CROSS_DOWN);

        err = sbus->write(Reg::REG_ADDR_INT_CFG, &(reg.regMap[Reg::REG_ADDR_INT_CFG]));
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Gets device product identifier 
 * 
 * @warning     Serial interface required. 
 *              If unavailable the function does nothing and returns without error (OK) 
 * 
 * @param[out]  prodID  Product identifier
 * @param[out]  revID   Version identifier
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::getDeviceID(uint8_t & prodID, uint8_t & revID)
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Function requires the serial interface available*/
        if(nullptr == sbus)
        {
            break;
        }

        /* Function requires sensor at least logic on*/
        err = setStatus(LOGIC_ONLY_ON, true);
        if(OK != err)
            break;

        err = sbus->read(Reg::REG_ADDR_PROD_ID, &(reg.regMap[Reg::REG_ADDR_PROD_ID]));
        if (OK != err)
            break;

        prodID = reg.getProductID();
        revID  = reg.getRevisionID();

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Saves the sensor register configuration and prepare the device for reset
 * 
 * @details     The scratch pad register is used for later if the reser was properly performed.
 *              This register must be 0x00 after reset.
 *              Serial interface required, but not verified as it is a private function.
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::preResetSaveConfig(void)
{
    Error_t err = OK;
    
    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Get current register configuration */
        err = sbus->read(Reg::REG_ADDR_PROD_ID, &(reg.regMap[Reg::REG_ADDR_PROD_ID]), reg.regCount);
        if (OK != err)
            break;

        /* Write some random value in the scratch pad*/
        reg.setStratchPad(0xEE);

        err = sbus->write(Reg::REG_ADDR_SCRATCH_PAD, &(reg.regMap[Reg::REG_ADDR_SCRATCH_PAD]));
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Restores the device register configuration after reset
 * 
 * @details     Once the sensor is ready. The scratch pad register has to be 0x00.s 
 *              Serial interface required, but not verified as it is a private function.
 * 
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      IC_POWERON_ERROR if device power on reset error
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::postResetRestoreConfig(void)
{
    Error_t err = OK;
    uint8_t scratchPad = 0xEE;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do
    {
        /* Read default startup/reset register values */
        err = sbus->read(Reg::REG_ADDR_PROD_ID, &(reg.regMap[Reg::REG_ADDR_PROD_ID]));
        if (OK != err)
            break;

        if(!reg.isSensorReady())
        {
                err = IC_POWERON_ERROR;
                break;
        }

        /* Check if reset was performed with scratch pad */ 
        err = sbus->read(Reg::REG_ADDR_SCRATCH_PAD, &(reg.regMap[Reg::REG_ADDR_SCRATCH_PAD]));
        if (OK != err)
            break;

        scratchPad = reg.getScratchPad();
        if(scratchPad != 0x00)
        {
            err = RESET_ERROR;
            break;
        }

        /* Write registers back */
        err = sbus->write(Reg::REG_ADDR_SENS_STS, &(reg.regMap[Reg::REG_ADDR_SENS_STS]), 4);
        if (OK != err)
            break;

        /* Write registers back */
        err = sbus->write(Reg::REG_ADDR_MEAS_STS, &(reg.regMap[Reg::REG_ADDR_MEAS_STS]), 8);
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Sets the device status
 * 
 * @details     From the host controller point of view, the device can be in the 
 *              following statuses:
 *              - UNITED. Controller peripheral not initialized
 *              - INITED. Controller peripheral initialized
 *              - LOGIC_ONLY_ON. Sensor VDD power logic on. INITED implicit
 *              - IR_ONLY_ON. IR emitter power on. INITED implicit
 *              - ON. Sensor powered on LOGIC_ONLY_ON, IR_ONLY_ON and INITED implicit
 *              
 *              The device configuration can be only performed if at least LOGIC_ONLY_ON
 *              is fulfilled. 
 *              For CO2 measurements, the IR emitter power has to be enabled. Thus,
 *              getCO2() is only available if the sensor is ON.
 *              
 *              Some statuses ential that the sensor has been previously in a certain 
 *              status. This is what implicit status means. For example, LOGIC_ONLY_ON
 *              means as well that the device is INITED.
 * 
 *              This functions takes care of the state machine transition between the 
 *              statuses.
 * 
 * @param[in]   newStatus           Desired new status to be set
 * @param[in]   enoughIfImplicit    Flag indicating if the the exact status is required, or just implicitly.
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if interface error
 * @pre         None
 */
Error_t PASCO2::setStatus(const Status_t newStatus, bool enoughIfImplicit)
{
    Error_t err = OK;

    PAS_CO2_LOG_MSG(__FUNCTION__);
    do{
        /* The the desired status only needs to be implicit in the current status */
        if(enoughIfImplicit)
        {
            /* If it is implicit. The status does not need to be changed. */
            if(UNINITED != newStatus)
            {
                if(newStatus == (status & newStatus))
                    break;
            }
        }

        /* The status has to be the exact one. Returns.*/
        if(newStatus == status)
        {
            break;
        }

        /* If new status requires more features enabled */
        else if(newStatus > status)
        {
            switch(newStatus)
            {   
                case INITED:
                    err = init();
                break;

                case LOGIC_ONLY_ON:
                    err = enableLogic();
                break;

                case IR_ONLY_ON:
                {
                    err = disableLogic();
                    if(OK == err)
                        err = enableIREmitter();
                }
                break;

                case ON:
                {
                    err = enableIREmitter();
                    if(OK == err)
                        err = enableLogic();
                }
                break;
                
                default:
                    /* Never entering here. We hope :) Do nothing*/
                break;
            }
        }
        /* If the new status requires disabling features */
        else
        {
            switch(newStatus)
            {
                case LOGIC_ONLY_ON: 
                {
                    err = disableIREmitter();
                    if(OK == err)
                        err = enableLogic();
                }
                break;
        
                case IR_ONLY_ON: 
                        err = disableLogic();
                break;

                case INITED:
                {
                    err = disableIREmitter();
                    if(OK == err)
                        err = disableLogic();
                }
                break;

                case UNINITED:
                    err = deinit();
                break;

                default:
                    /* Never entering here. We hope :) Do nothing*/
                break;
            }
        }
    } while (0);
    PAS_CO2_LOG_RETURN(err);

    return err;
}
