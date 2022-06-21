/** 
 * @file        pas-co2-ino.hpp
 * @brief       XENSIV™ PAS CO2 Arduino API
 * @copyright   Copyright (c) 2020-2021 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_INO_HPP_
#define PAS_CO2_INO_HPP_

#include <Arduino.h>
#include <Wire.h>
#include <HardwareSerial.h>
#include "pas-co2-platf-ino.hpp"
#include "xensiv_pasco2.h"

/**
 * @addtogroup co2inoapi
 * @{
 */

typedef int32_t Error_t;
typedef xensiv_pasco2_status_t Diag_t;
typedef xensiv_pasco2_boc_cfg_t ABOC_t;

class PASCO2Ino
{
    public:

        static constexpr uint8_t       unusedPin = 0xFFU; /**< Unused pin */        

                PASCO2Ino (TwoWire * wire = &Wire, uint8_t intPin = unusedPin);
                PASCO2Ino (HardwareSerial * serial, uint8_t intPin = unusedPin);
                ~PASCO2Ino();
        Error_t begin           ();
        Error_t end             ();
        Error_t startMeasure    (int16_t  periodInSec = 0, int16_t alarmTh = 0, void (*cback) (void *) = nullptr, bool earlyNotification = false);
        Error_t stopMeasure     ();
        Error_t getCO2          (int16_t & CO2PPM);
        Error_t getDiagnosis    (Diag_t & diagnosis);
        Error_t setABOC         (ABOC_t aboc, int16_t abocRef);
        Error_t setPressRef     (uint16_t pressRef);
        Error_t performForcedCompensation(uint16_t co2Ref);
        Error_t clearForcedCompensation  ();
        Error_t reset           ();
        Error_t getDeviceID     (uint8_t & prodID, uint8_t & revID);

        Error_t getRegister     (uint8_t regAddr, uint8_t * data, uint8_t len);
        Error_t setRegister     (uint8_t regAddr, const uint8_t * data, uint8_t len);

    private:

        TwoWire         * i2c;          /**< I2C interface*/
        HardwareSerial  * uart;         /**< UART interface */   
        uint8_t           intPin;       /**< Interrupt pin */

        static constexpr uint16_t baudrateBps = 9600;      /**< UART baud rate in bps */
        static constexpr uint32_t freqHz      = 100000;    /**< I2C frequency in Hz*/

        xensiv_pasco2_t   dev;          /**< XENSIV™ PAS CO2 corelib object */
};

/** @} */

#endif /** PAS_CO2_INO_HPP_ **/