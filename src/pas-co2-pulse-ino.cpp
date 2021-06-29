/** 
 * @file        pas-co2-pulse-ino.cpp
 * @brief       PAS CO2 Pulse Arduino API
 * @date        January 2021
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-pulse-ino.hpp"

#if IS_INTF(PAS_CO2_INTF_PULSE)

using namespace pasco2;

/**
 * @brief      PAS CO2 Pulse Arduino Constructor
 * 
 *   Mandatory arguments: 
 *      - The PWM pin needs to be provided
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
 * @param[in]   pwmPin          PWM pin.
 * @param[in]   intPin          Interrupt pin. Default is UnusedPin         
 * @param[in]   power3V3Pin     Power 3V3 pin. Default is UnusedPin
 * @param[in]   power12VPin     Power 12V pin. Default is UnusedPin
 * @param[in]   pwmSelPin       PWM select pin. Default is UnusedPin
 * @pre         None
 */
PASCO2PulseIno::PASCO2PulseIno(uint8_t   pwmPin, 
                               uint8_t   intPin,
                               uint8_t   power3V3Pin,
                               uint8_t   power12VPin,
                               uint8_t   pwmSelPin)
: 
PASCO2Pulse((pwm = new PWMIno((pwmGPIO  = new GPIOIno(pwmPin, INPUT_PULLUP, GPIO::VLogic_t::POSITIVE)),
                              (pwmTimer = new TimerIno()),
                               PWMSW::Mode_t::MODE_POLLING)), 
            pwmTimer,
            ((GPIOIno::unusedPin == intPin) ?       nullptr : (interrupt  = new GPIOIno(intPin, INPUT_PULLUP, GPIO::VLogic_t::POSITIVE))),
            ((GPIOIno::unusedPin == power3V3Pin) ?  nullptr : (power3V3   = new GPIOIno(power3V3Pin, OUTPUT,  GPIO::VLogic_t::POSITIVE))),
            ((GPIOIno::unusedPin == power12VPin) ?  nullptr : (power12V   = new GPIOIno(power12VPin, OUTPUT,  GPIO::VLogic_t::POSITIVE))),
            ((GPIOIno::unusedPin == pwmSelPin) ?    nullptr : (pwmSelect  = new GPIOIno(pwmSelPin,   OUTPUT,  GPIO::VLogic_t::POSITIVE))))
{

}

/**
 * @brief       PAS CO2 Pulse Arduino Destructor 
 * @details     It disables the sensor and deletes all the
 *              dynamically created PAL instances in the 
 *              constructor
 * @pre         None
 */
PASCO2PulseIno::~PASCO2PulseIno()
{

    PASCO2Pulse::disable();

    if(nullptr != pwm)
    {
        delete pwm;
        pwm = nullptr;
    }

    if(nullptr != pwmGPIO)
    {
        delete pwmGPIO;
        pwmGPIO = nullptr;
    }

    if(nullptr != pwmTimer)
    {
        delete pwmTimer;
        pwmTimer = nullptr;
    }

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

    if(nullptr != pwmSelect)
    {
        delete pwmSelect;
        pwmSelect = nullptr; 
    }
}

#endif /** PAS_CO2_INTF **/