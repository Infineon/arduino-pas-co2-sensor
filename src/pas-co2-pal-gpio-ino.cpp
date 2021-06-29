/** 
 * @file        pas-co2-pal-gpio-ino.cpp
 * @brief       PAS CO2 GPIO Arduino PAL 
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-pal-gpio-ino.hpp"
#include <Arduino.h>

using namespace pasco2;

/**
 * @brief       GPIO Ino constructor
 * 
 * @param[in]   pin     Pin number
 * @param[in]   mode    Arduino GPIO mode (INPUT, OUTPUT, INPUT_PULLUP)
 * @param[in]   logic   Voltage logic
 */
GPIOIno::GPIOIno(uint8_t      pin, 
                 uint8_t      mode, 
                 VLogic_t     logic) 
: pin(pin), mode(mode), logic(logic) 
{

}

/**
 * @brief   GPIO Ino destructor
 */
GPIOIno::~GPIOIno()
{

}


/**
 * @brief   Initializes the Arduino GPIO
 * @return  PAS CO2 error code
 * @retval  OK always
 */
Error_t GPIOIno::init()
{   
    pinMode(this->pin, this->mode);
    return pasco2::OK;
}

/**
 * @brief   Deinitializes the Arduino GPIO
 * @return  PAS CO2 error code
 * @retval  OK always
 */
Error_t GPIOIno::deinit()
{
    return pasco2::OK;
}

/**
 * @brief       Enables the Arduino GPIO interrupt
 * @param[in]   *cback Function pointer of the interrupt callback
 * @return      PAS CO2 error code
 * @retval      OK always
 */
Error_t GPIOIno::enableInt(void (*cback) (void *))
{
    attachInterrupt(digitalPinToInterrupt(this->pin), (void (*)())cback, RISING);
    return pasco2::OK;
}

/**
 * @brief   Disables the Arduino GPIO interrupt
 * @return  PAS CO2 error code
 * @retval  OK always
 */
inline Error_t GPIOIno::disableInt()
{
    detachInterrupt(digitalPinToInterrupt(this->pin));
    return pasco2::OK;
}

/**
 * @brief   Gets the latest Arduino interrupt event 
 * @return  GPIO interrupt event
 * @retval  INT_FALLING_EDGE if falling edge event
 * @retval  INT_RISING_EDGE if rising edge event
 */
inline GPIOIno::IntEvent_t GPIOIno::intEvent()
{
    IntEvent_t edge = INT_FALLING_EDGE;
    
    int val = digitalRead(this->pin);
    if(val == LOW)
    {
        edge = INT_FALLING_EDGE;
    }
    else if(val == HIGH) 
    {
        edge = INT_RISING_EDGE;
    }

    return edge;
}

/**
 * @brief       Reads the Arduino GPIO voltage level
 * @return      GPIO voltage level
 * @retval      GPIO_LOW if voltage low
 * @retval      GPIO_HIGH if voltage high
 */
inline GPIOIno::VLevel_t GPIOIno::read()
{
    return (VLevel_t) digitalRead(this->pin);
}

/**
 * @brief       Writes the Arduino GPIO output voltage level
 * @param[in]   level  Voltage level
 * @return      PAS CO2 error code
 * @retval      OK always
 */
inline Error_t GPIOIno::write(VLevel_t level)
{
    digitalWrite(this->pin, level);
    return pasco2::OK;
}

/**
 * @brief       Enables the Arduino GPIO output according to the GPIO logic
 *              - Low if negative
 *              - High if positive
 * @return      PAS CO2 error code
 * @retval      OK always
 */
inline Error_t GPIOIno::enable()
{
    if(this->logic == POSITIVE)
    {
        digitalWrite(this->pin, GPIO_HIGH);
    }
    else if(this->logic == NEGATIVE)
    {
        digitalWrite(this->pin, GPIO_LOW);
    }
    return pasco2::OK;
}

/**
 * @brief       Disables the Arduino GPIO output according to the GPIO logic
 *              - Low if positive
 *              - High if negative
 * @return      GPIO error code
 * @return      PAS CO2 error code
 * @retval      OK always
 */
inline Error_t GPIOIno::disable()
{
   if(this->logic == POSITIVE)
    {
        digitalWrite(this->pin, GPIO_LOW);
    }
    else if(this->logic == NEGATIVE)
    {
        digitalWrite(this->pin, GPIO_HIGH);
    }
    return pasco2::OK;
}