/** 
 * @file        pas-co2-ino.cpp
 * @brief       XENSIV™ PAS CO2 Arduino API
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-ino.hpp"

/**
 * @brief   Assertion of XENSIV™ PAS CO2 return code
 */
#define INO_ASSERT_RET(x)   if( x != XENSIV_PASCO2_OK ) { return x; }

/**
 * @brief   External serial interface init and deinit
 * 
 * @details Comment this define for the serial interface initialization
 *          and denitialization (i2c and uart) to be handled by the begin()
 *          and end() class function respectively 
 * 
 */
#define PAS_CO2_SERIAL_PAL_INIT_EXTERNAL

/**
 * @brief      XENSIV™ PAS CO2 I2C Arduino Constructor
 *
 * @param[in]   wire    TwoWire interface instance. Default is the Arduino primary Wire instance.
 * @param[in]   intPin  Interrupt pin. Default is UnusedPin         
 * @pre         None
 */
PASCO2Ino::PASCO2Ino(TwoWire * wire,
                                 uint8_t   intPin)
: i2c(wire), uart(nullptr), intPin(intPin)
{

}

/**
 * @brief      XENSIV™ PAS CO2 UART Arduino Constructor
 * 
 * @param[in]   serial  Serial interface instance
 * @param[in]   intPin  Interrupt pin. Default is UnusedPin         
 * @pre         None
 */
PASCO2Ino::PASCO2Ino(HardwareSerial * serial,
                                 uint8_t          intPin)
: i2c(nullptr), uart(serial), intPin(intPin)
{

}

/**
 * @brief       XENSIV™ PAS CO2 Arduino Destructor 
 * @details     It disables the sensor and deletes all the
 *              dynamically created PAL instances in the 
 *              constructor
 * @pre         None
 */
PASCO2Ino::~PASCO2Ino()
{

}

/**
 * @brief   Begins the sensor
 * 
 * @details Initializes the serial interface if the initialization
 *          is delegated to the PASCO2 class.
 *          Sets the I2C freq or UART baudrate to the default values 
 *          prior the serial interface initialization.
 *          Initializes the interrupt pin if used.
 * 
 * @return  XENSIV™ PAS CO2 error code
 * @retval  XENSIV_PASCO2_OK if success 
 * @pre     None
 */
Error_t PASCO2Ino::begin()
{
    int32_t ret = XENSIV_PASCO2_OK;
    xensiv_pasco2_measurement_config_t  measConf;

    /* Initialize sensor interface */
    if(nullptr != i2c)
    {
        #ifndef PAS_CO2_SERIAL_PAL_INIT_EXTERNAL
        i2c->begin();
        i2c->setClock(freqHz);
        #endif 
        ret = xensiv_pasco2_init_i2c(&dev, i2c);
    }
    else if(nullptr != uart)
    {
        #ifndef PAS_CO2_SERIAL_PAL_INIT_EXTERNAL
        uart->begin(baudrateBps);   
        #endif
        ret = xensiv_pasco2_init_uart(&dev, uart);
    }

    /* Initialize int_pin */
    if( unusedPin != intPin)
    {
        pinMode(intPin, INPUT_PULLUP);
    }

    /**
     * Set the sensor in idle mode.
     * In case PWM_DIS is by hardware configuring 
     * the device to continuous mode
     */
    ret = xensiv_pasco2_get_measurement_config(&dev, &measConf);
    INO_ASSERT_RET(ret);

    measConf.b.op_mode = XENSIV_PASCO2_OP_MODE_IDLE;

    ret = xensiv_pasco2_set_measurement_config(&dev, measConf);
     
    return ret;
}

/**
 * @brief   Ends the sensor
 * 
 * @details Deinitializes the serial interface if the deinitialization
 *          is delegated to the PASCO2Ino class. 
 *          Deinitializes the interrupt pin if used.
 * 
 * @return  XENSIV™ PAS CO2 error code
 * @retval  XENSIV_PASCO2_OK always
 * @pre     begin()
 */
Error_t PASCO2Ino::end()
{
    /**< Deinitialize sensor interface*/
    if(nullptr != i2c)
    {
        #ifndef PAS_CO2_SERIAL_PAL_INIT_EXTERNAL
        #if !defined(ARDUINO_ARCH_ESP32)
        i2c->end();
        #endif
        #endif
    }   
    else if(nullptr != uart)
    {
        #ifndef PAS_CO2_SERIAL_PAL_INIT_EXTERNAL
        uart->end();
        #endif
    }

    /* Deinitialize interrupt pin */
    if(unusedPin != intPin)
    {
        detachInterrupt(digitalPinToInterrupt(intPin));
    }

    return XENSIV_PASCO2_OK;
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
 *              will be triggered to perform a single shot measurement. 
 *              The user needs to poll with getCO2() until the CO2 value is 
 *              available and has been read out from the sensor.
 *              The CO2 concentration value read will be zero as long as 
 *              no value is available or if any error occurred in the 
 *              readout attempt. 
 *              Polling example:
 * 
 *              @code
 *              PASCO2Ino cotwo(serial_intf); 
 *              int16_t   co2ppm;
 * 
 *              serial_intf.begin();
 * 
 *              cotwo.begin();              
 * 
 *              cotwo.startMeasure();
 * 
 *              do{ cotwo.getCO2(co2ppm); } while (co2ppm == 0);  
 *              @endcode
 * 
 *              Continuous measurement
 *              ---------------------------------------------------------------
 *              Continuous measurements (periodInSec) will configure the sensor
 *              to perform a measurement every desired period. Between 5 and
 *              4095 seconds.
 *              Without further arguments, the user has to poll with getCO2()
 *              until the value is available. Any super loop or thread 
 *              routine, can just consists on reading the CO2 (getCO2()). 
 *              For example, measure every 5 minutes:
 * 
 *              @code
 *              PASCO2Ino cotwo(serial_intf);
 *              int16_t   co2ppm;
 * 
 *              serial_intf.begin();
 * 
 *              cotwo.begin();  
 * 
 *              cotwo.startMeasure(300);
 * 
 *              while(1)
 *              {
 *                  delay(300000); // Measure will be ready every 5 min
 * 
 *                  do{ cotwo.getCO2(co2ppm); } while (co2ppm == 0);  
 *                  // ... do something with the co2 value ... 
 *              }
 *              @endcode
 *
 *              Synching readouts with the hardware interrupt
 *              ---------------------------------------------------------------
 *              In order not to saturate the sensor with constant serial 
 *              requests, especially in continuous mode, it is recommended 
 *              to synch the readout with a timer. Or even better using 
 *              the hardware GPIO hardware interrupt.
 *              If the interrupt pin has been provided, passing
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
 *              PASCO2Ino cotwo(serial_intf, interrupt);
 *              int16_t   co2ppm;
 *              
 *              serial_intf.begin();
 *              
 *              cotwo.begin();  
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
 *              is activated, and the sensor internal flag will be enabled 
 *              if the concentration of CO2 goes above the specified value.
 *              This option is better combined with the interupt mode. Thus,
 *              if the interrupt mode is available and a callback function
 *              is passed, the interrupt will occurr only when the co2 
 *              concentration goes above the threshold. 
 *              This makes mostly sense for continuous measurement configuration. 
 *              But it can be used as well for a single shot configuration
 * 
 *              Early notification
 *              ---------------------------------------------------------------
 *              The early notification mode can be used for battery power
 *              solutions. The interrupt signal can trigger the enablement of
 *              the 12V emitter power supply just before the measurement is
 *              performed, and switch it off as the interrupt signal is disabled.
 *              Therefore, the power supply 12V only needs to be on during the
 *              CO2 sensing.
 *                  
 *              When this flag is set, the alarm interrupt functionality is not 
 *              available. Both options cannot be combined.
 *              
 *                 
 * @param[in]   periodInSec         Enables continuous measurement with the specified period. 
 *                                  The default value is 0, meaning single shot operation. 
 *                                  The valid period range goes between 5 and 4095 seconds
 * @param[in]   alarmTh             Enables upper alarm threshold mode for the specified
 *                                  ppm value 
 *                                  The default value is 0, meaning no alarm mode. 
 *                                  For any non-zero value, the sensor will internally set 
 *                                  the alarm flag. If an interrupt callback function is 
 *                                  provided, then the interrupt will occurr only when the 
 *                                  defined threshold has been tresspassed
 * @param[in]   cback               Pointer to the callback function to be called upon
 *                                  interrupt
 * @param[in]   earlyNotification   Enables early notifification interrupt. Disabled (false) by default 
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         begin()
 */
Error_t PASCO2Ino::startMeasure(int16_t periodInSec, int16_t alarmTh, void (*cback) (void *), bool earlyNotification)
{
    xensiv_pasco2_measurement_config_t  measConf;
    xensiv_pasco2_interrupt_config_t intConf; 
    int32_t ret = XENSIV_PASCO2_OK;   

    /* Get meas configuration*/
    ret = xensiv_pasco2_get_measurement_config(&dev, &measConf);
    INO_ASSERT_RET(ret);

    /**
     * Set the device in idle mode to avoid 
     * any conflict if stopMeasure() was not
     * previously called.
     */
    measConf.b.op_mode = XENSIV_PASCO2_OP_MODE_IDLE;

    ret = xensiv_pasco2_set_measurement_config(&dev, measConf);
    INO_ASSERT_RET(ret);

    /* Get int configuration */
    ret = xensiv_pasco2_get_interrupt_config(&dev, &intConf);
    INO_ASSERT_RET(ret);

    /* Default configuration */
    measConf.b.op_mode = XENSIV_PASCO2_OP_MODE_SINGLE;
    intConf.b.int_func = XENSIV_PASCO2_INTERRUPT_FUNCTION_DRDY;
             

    if( periodInSec > 0 )
    {   
        ret = xensiv_pasco2_set_measurement_rate(&dev, periodInSec);
        INO_ASSERT_RET(ret);

        measConf.b.op_mode = XENSIV_PASCO2_OP_MODE_CONTINUOUS;
    }

    if( alarmTh >  0 )
    {
        ret = xensiv_pasco2_set_alarm_threshold(&dev, alarmTh);
        INO_ASSERT_RET(ret);

        intConf.b.alarm_typ = XENSIV_PASCO2_ALARM_TYPE_LOW_TO_HIGH;
        intConf.b.int_func  = XENSIV_PASCO2_INTERRUPT_FUNCTION_ALARM;
    }
    else
    {
        ret = xensiv_pasco2_set_alarm_threshold(&dev, 0x0000);
        INO_ASSERT_RET(ret);

        intConf.b.alarm_typ = XENSIV_PASCO2_ALARM_TYPE_HIGH_TO_LOW;       
    }

    if(cback != nullptr)
    {
        /* Enable sensor interrupt */
        intConf.b.int_typ = XENSIV_PASCO2_INTERRUPT_TYPE_HIGH_ACTIVE;
        #if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_RENESAS) || defined(ARDUINO_ARCH_PSOC6)
            PinStatus int_event;
        #else
            uint8_t int_event;
        #endif        
        int_event = RISING;

        if(true == earlyNotification)
        {
           /* In this case it would be useful to have an interrupt
              for both the rising and falling edge. */
            int_event = CHANGE;
        }

        /* Enable mcu interupt */
        attachInterrupt(digitalPinToInterrupt(intPin), (void (*)())cback, int_event);
    }
    else
    {
        /* Disable sensor interrupt */
        intConf.b.int_func = XENSIV_PASCO2_INTERRUPT_FUNCTION_NONE;

        if(unusedPin != intPin)
        {
            /* Disable mcu interrupt */
            detachInterrupt(digitalPinToInterrupt(intPin));
        }
    }

    /* This option will disable the alarm interrupt function */ 
    if(true == earlyNotification)
    {
        intConf.b.int_func = XENSIV_PASCO2_INTERRUPT_FUNCTION_EARLY;
    }

    ret = xensiv_pasco2_set_interrupt_config(&dev, intConf);
    INO_ASSERT_RET(ret);

    ret = xensiv_pasco2_set_measurement_config(&dev, measConf);

    return ret;
}

/**
 * @brief       Stops the internal measuring of the sensor 
 * 
 * @details     Sets operation mode to idle
 * 
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         begin()
 */
Error_t PASCO2Ino::stopMeasure()
{
    int32_t ret = XENSIV_PASCO2_OK;  

    xensiv_pasco2_measurement_config_t  measConf;

    /* Get meas configuration*/
    ret = xensiv_pasco2_get_measurement_config(&dev, &measConf);
    INO_ASSERT_RET(ret);

    /* Set meas configuration to idle mode */
    measConf.b.op_mode = XENSIV_PASCO2_OP_MODE_IDLE;
    ret = xensiv_pasco2_set_measurement_config(&dev, measConf);

    return ret;
}

/**
 * @brief       Gets the CO2 concentration measured
 *               
 * 
 * @details     The value read is zero when no measurement is 
 *              yet available or an error has ocurrred.
 * 
 * @param[out]  co2ppm  CO2 concentration read (in ppm)
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         startMeasure()
 */
Error_t PASCO2Ino::getCO2(int16_t & CO2PPM)
{
    int32_t ret = XENSIV_PASCO2_OK;  

    /* Initially set to 0.*/
    CO2PPM = 0;

    /* Read the data */
    ret = xensiv_pasco2_get_result(&dev, (uint16_t*)&CO2PPM);
    INO_ASSERT_RET(ret);

    /* Clear masks from status register */
    ret = xensiv_pasco2_clear_measurement_status(&dev,(XENSIV_PASCO2_REG_MEAS_STS_INT_STS_CLR_MSK | XENSIV_PASCO2_REG_MEAS_STS_ALARM_CLR_MSK));
    INO_ASSERT_RET(ret);

    return ret;
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
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         None
 */
Error_t PASCO2Ino::getDiagnosis(Diag_t & diagnosis)
{
    int32_t ret = XENSIV_PASCO2_OK; 

    /* Get current status */
    ret = xensiv_pasco2_get_status(&dev, &diagnosis);
    INO_ASSERT_RET(ret);

    /* Clear read flags */
    ret = xensiv_pasco2_clear_status(&dev, (XENSIV_PASCO2_REG_SENS_STS_ICCER_CLR_MSK |
                                            XENSIV_PASCO2_REG_SENS_STS_ORVS_CLR_MSK  |
                                            XENSIV_PASCO2_REG_SENS_STS_ORTMP_CLR_MSK ));

    return ret;
}

/**
 * @brief       Configures the sensor automatic baseline compensation
 * 
 * @param[in]   aboc        Automatic baseline compenstation mode  
 * @param[in]   abocRef     Automatic baseline compensation reference
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         begin()
 */
Error_t PASCO2Ino::setABOC(ABOC_t aboc, int16_t abocRef)
{
    xensiv_pasco2_measurement_config_t  measConf;
    int32_t ret = XENSIV_PASCO2_OK; 

    /* Get meas configuration */
    ret = xensiv_pasco2_get_measurement_config(&dev, &measConf);
    INO_ASSERT_RET(ret);

    /* Set compensation offset */
    ret = xensiv_pasco2_set_offset_compensation(&dev, (uint16_t) abocRef);
    INO_ASSERT_RET(ret);

    /* Set meas configuration with ABOC */
    measConf.b.boc_cfg = aboc;
    ret = xensiv_pasco2_set_measurement_config(&dev, measConf);

    return ret;
}

/**
 * @brief       Performs force compensation
 * 
 * @details     Calculates the offset compensation when the sensor is exposed to a CO2 reference
 *              value.
 * @warning     The device is left in idle mode after the compensation value is stored in
 *              non-volatile memory. 
 * 
 * @param[in]   co2Ref  Automatic baseline compenstation mode  
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         begin()
 */
Error_t PASCO2Ino::performForcedCompensation(uint16_t co2Ref)
{
    return xensiv_pasco2_perform_forced_compensation(&dev, co2Ref);
}

/**
 * @brief       Resets the forced calibration correction factor
 * 
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         begin()
 */
Error_t PASCO2Ino::clearForcedCompensation()
{
    return xensiv_pasco2_cmd(&dev, XENSIV_PASCO2_CMD_RESET_FCS);
}

/**
 * @brief       Sets the sensor pressure reference
 * 
 * @param[in]   pressRef    Pressure reference value. Min value is 750, and max 1150.
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         begin()
 */
Error_t PASCO2Ino::setPressRef(uint16_t pressRef)
{
    int32_t ret = XENSIV_PASCO2_OK; 

    ret = xensiv_pasco2_set_pressure_compensation(&dev, pressRef);

    return ret;
}

/**
 * @brief       Resets the sensor via serial command
 * 
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         begin()
 */
Error_t PASCO2Ino::reset()
{
    int32_t ret = XENSIV_PASCO2_OK; 

    ret = xensiv_pasco2_cmd(&dev, XENSIV_PASCO2_CMD_SOFT_RESET);
    INO_ASSERT_RET(ret);

    return ret;
}

/**
 * @brief       Gets device product identifier 
 *  
 * @param[out]  prodID  Product identifier
 * @param[out]  revID   Version identifier
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         begin()
 */
Error_t PASCO2Ino::getDeviceID(uint8_t & prodID, uint8_t & revID)
{
    int32_t ret = XENSIV_PASCO2_OK; 
    xensiv_pasco2_id_t id;
    
    ret = xensiv_pasco2_get_id(&dev, &id);
    INO_ASSERT_RET(ret);

    prodID = id.b.prod;
    revID =  id.b.rev;
    
    return ret;
}

/**
 * @brief       Reads from the sensor device into the given data buffer
 *  
 * @param[in]   regAddr Start register address
 * @param[out]  data    Pointer to the data buffer to store the register values of the sensor
 * @param[in]   len     Number of bytes of data to be read
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         begin()
 */
Error_t PASCO2Ino::getRegister(uint8_t regAddr, uint8_t * data, uint8_t len)
{
    return xensiv_pasco2_get_reg(&dev, regAddr, data, len);
}

/**
 * @brief      Writes the given data buffer into the sensor device
 *  
 * @param[in]   regAddr Start register address
 * @param[in]   data    Pointer to the data buffer to be written in the sensor
 * @param[in]   len     Number of bytes of data to be written
 * @return      XENSIV™ PAS CO2 error code
 * @retval      XENSIV_PASCO2_OK if success
 * @pre         begin()
 */
Error_t PASCO2Ino::setRegister(uint8_t regAddr, const uint8_t * data, uint8_t len)
{
    return xensiv_pasco2_set_reg(&dev, regAddr, data, len);
}