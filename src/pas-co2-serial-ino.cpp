/** 
 * @file        pas-co2-serial-ino.cpp
 * @brief       PAS CO2 Serial Arduino API
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-serial-ino.hpp"

#if IS_INTF(PAS_CO2_INTF_SERIAL)

using namespace pasco2;

#if IS_INTF(PAS_CO2_INTF_I2C)

/**
 * @brief      PAS CO2 Serial Arduino Constructor
 * 
 *   Mandatory arguments: 
 *      - The TwoWire instance needs to be provided
 * 
 *   Optional  arguments: 
 *      - All other pins instances.
 * 
 *      Most of the GPIO will be fixed by hardware. In particular those enabling 
 *      the sensor main power and the emitter power. 
 *      They are part of the sensor library to provide potential absolute control 
 *      by software.
 *      The hardware interface and its features, are also provided in the library.
 *      But it can be as well handled externally 
 * 
 * @param[in]   *wire           TwoWire interface instance
 * @param[in]   intPin          Interrupt pin. Default is UnusedPin         
 * @param[in]   protoSelectPin  Protocol select pin. Default is UnusedPin
 * @param[in]   power3V3Pin     Power 3V3 pin. Default is UnusedPin
 * @param[in]   power12VPin     Power 12V pin. Default is UnusedPin
 * @pre         None
 */
PASCO2SerialIno::PASCO2SerialIno(TwoWire * wire,
                                 uint8_t   intPin, 
                                 uint8_t   protoSelectPin,
                                 uint8_t   power3V3Pin,
                                 uint8_t   power12VPin)
: 
PASCO2Serial(((nullptr == wire) ? &(pasco2::i2c) : (i2c = new I2C(i2cpal = new I2CPALIno(wire)))), 
            (timer = new TimerIno()),
            ((GPIOIno::unusedPin == intPin) ?           nullptr : (interrupt    = new GPIOIno(intPin, INPUT_PULLUP,  GPIO::VLogic_t::POSITIVE))),
            ((GPIOIno::unusedPin == protoSelectPin) ?   nullptr : (protoSelect  = new GPIOIno(protoSelectPin, OUTPUT,  GPIO::VLogic_t::POSITIVE))),
            ((GPIOIno::unusedPin == power3V3Pin) ?      nullptr : (power3V3     = new GPIOIno(power3V3Pin, OUTPUT,  GPIO::VLogic_t::POSITIVE))),
            ((GPIOIno::unusedPin == power12VPin) ?      nullptr : (power12V     = new GPIOIno(power12VPin, OUTPUT,  GPIO::VLogic_t::POSITIVE))))
{

}
#endif

#if IS_INTF(PAS_CO2_INTF_UART)

/**
 * @brief      PAS CO2 Serial Arduino Constructor
 * 
 *   Mandatory arguments: 
 *      - The HardwareSerial instance needs to be provided
 * 
 *   Optional  arguments: 
 *      - All other pins instances.
 * 
 *      Most of the GPIO will be fixed by hardware. In particular those enabling 
 *      the sensor main power and the emitter power. 
 *      They are part of the sensor library to provide potential absolute control 
 *      by software.
 *      The hardware interface and its features, are also provided in the library.
 *      But it can be as well handled externally 
 * 
 * @param[in]   *serial         Serial interface instance
 * @param[in]   intPin          Interrupt pin. Default is UnusedPin         
 * @param[in]   protoSelectPin  Protocol select pin. Default is UnusedPin
 * @param[in]   power3V3Pin     Power 3V3 pin. Default is UnusedPin
 * @param[in]   power12VPin     Power 12V pin. Default is UnusedPin
 * @pre         None
 */
PASCO2SerialIno::PASCO2SerialIno(HardwareSerial * serial,
                                 uint8_t          intPin, 
                                 uint8_t          protoSelectPin,
                                 uint8_t          power3V3Pin,
                                 uint8_t          power12VPin)
: 
PASCO2Serial((uart = new UART(uartpal = new UARTPALIno(serial))), 
            (timer = new TimerIno()),
            ((GPIOIno::unusedPin == intPin) ?           nullptr : (interrupt    = new GPIOIno(intPin, INPUT_PULLUP,  GPIO::VLogic_t::POSITIVE))),
            ((GPIOIno::unusedPin == protoSelectPin) ?   nullptr : (protoSelect  = new GPIOIno(protoSelectPin, OUTPUT,  GPIO::VLogic_t::POSITIVE))),
            ((GPIOIno::unusedPin == power3V3Pin) ?      nullptr : (power3V3     = new GPIOIno(power3V3Pin, OUTPUT,  GPIO::VLogic_t::POSITIVE))),
            ((GPIOIno::unusedPin == power12VPin) ?      nullptr : (power12V     = new GPIOIno(power12VPin, OUTPUT,  GPIO::VLogic_t::POSITIVE))))
{

}
#endif

/**
 * @brief       PAS CO2 Serial Arduino Destructor 
 * @details     It disables the sensor and deletes all the
 *              dynamically created PAL instances in the 
 *              constructor
 * @pre         None
 */
PASCO2SerialIno::~PASCO2SerialIno()
{

    PASCO2Serial::disable();

    #if IS_INTF(PAS_CO2_INTF_I2C)
    if(nullptr != i2cpal)
    {
        if(i2cpal != static_cast<I2CPAL *>(&i2cpalino))
        {
            delete i2cpal;
            i2cpal = nullptr; 
        }
    }

    if(nullptr != i2c)
    {
        if(i2c != static_cast<I2C *>(&(pasco2::i2c)))
        {
            delete i2c;
            i2c = nullptr; 
        }
    }
    #endif
    
    #if IS_INTF(PAS_CO2_INTF_UART)
    if(nullptr != uartpal)
    {
        delete uartpal;
        uartpal = nullptr; 
    }

    if(nullptr != uart)
    {
        delete uart;
        uart = nullptr;
    }
    #endif

    if(nullptr != timer)
    {
        delete timer;
        timer = nullptr; 
    }

    if(nullptr != interrupt)
    {
        delete interrupt;
        interrupt = nullptr; 
    }

    if(nullptr != protoSelect)
    {
        delete protoSelect;
        protoSelect = nullptr; 
    }

    if(nullptr != power3V3)
    {
        delete power3V3;
        power3V3 = nullptr;
    }

    if(nullptr != power12V)
    {
        delete power12V;
        power12V = nullptr;
    }
}

#endif /** PAS_CO2_INTF **/
