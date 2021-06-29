/** 
 * @file        pas-co2-serial-ino.hpp
 * @brief       PAS CO2 Serial Arduino API
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_SERIAL_INO_HPP_
#define PAS_CO2_SERIAL_INO_HPP_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_SERIAL)

#include <Arduino.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include "pas-co2-serial.hpp"
#include "pas-co2-i2c.hpp"
#include "pas-co2-uart.hpp"
#include "pas-co2-pal-gpio-ino.hpp"
#include "pas-co2-pal-timer-ino.hpp"
#include "pas-co2-pal-i2c-ino.hpp"
#include "pas-co2-pal-uart-ino.hpp"
#include "pas-co2-platf-ino.hpp"

/**
 * @addtogroup co2sinoapi
 * @{
 */

class PASCO2SerialIno : public PASCO2Serial
{
    public:
        
        #if IS_INTF(PAS_CO2_INTF_I2C)

        /* I2C interface */
        PASCO2SerialIno(TwoWire * wire           = nullptr, 
                        uint8_t   intPin         = GPIOIno::unusedPin, 
                        uint8_t   protoSelectPin = GPIOIno::unusedPin,
                        uint8_t   power3V3Pin    = GPIOIno::unusedPin,
                        uint8_t   power12VPin    = GPIOIno::unusedPin);
        
        #endif

        #if IS_INTF(PAS_CO2_INTF_UART)

        /* UART interface */
        PASCO2SerialIno(HardwareSerial * serial, 
                        uint8_t          intPin         = GPIOIno::unusedPin, 
                        uint8_t          protoSelectPin = GPIOIno::unusedPin,
                        uint8_t          power3V3Pin    = GPIOIno::unusedPin,
                        uint8_t          power12VPin    = GPIOIno::unusedPin);
        #endif
        
        ~PASCO2SerialIno();

        /**
         * @brief   Begins the sensor
         * 
         * @note    Optional call. The sensor initializes and
         *          enables the sensor directly in the rest of
         *          API calls.
         * @return  PAS CO2 error code
         * @retval  OK if success
         * @retval  INTF_ERROR if interface error
         * @retval  IC_POWERON_ERROR if power-on error
         */
        Error_t begin   () { return enable(); };
        
        /**
         * @brief   Ends the sensor
         * 
         * @note    Optional call. The sensor initializes and
         *          enables the sensor directly in the rest of
         *          API calls.
         * @return  PAS CO2 error code
         * @retval  OK if success
         * @retval  INTF_ERROR if interface error
         */
        Error_t end     () { return disable(); };

    private:

        #if IS_INTF(PAS_CO2_INTF_I2C)
        I2CPALIno   * i2cpal;
        pasco2::I2C * i2c;
        #endif


        #if IS_INTF(PAS_CO2_INTF_UART)
        UARTPALIno   * uartpal;
        pasco2::UART * uart;
        #endif
        
        TimerIno  * timer;
        GPIOIno   * interrupt;
        GPIOIno   * protoSelect;
        GPIOIno   * power3V3;
        GPIOIno   * power12V;
};

/** @} */

#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_SERIAL_INO_HPP_ **/