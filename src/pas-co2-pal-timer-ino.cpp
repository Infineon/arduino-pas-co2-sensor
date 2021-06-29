/** 
 * @file        pas-co2-pal-timer-ino.cpp
 * @brief       PAS CO2 Timer Arduino PAL 
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include <Arduino.h>
#include "pas-co2-pal-timer-ino.hpp"

using namespace pasco2;

/**
 * @brief Timer Ino constructor
 */
TimerIno::TimerIno() : curTime(0)
{

}

/**
 * @brief Timer Ino destructor
 */
TimerIno::~TimerIno()
{
    curTime = 0;
}

/**
 * @brief   Initialiazes the Arduino timer
 * @return  PAS CO2 error code
 * @retval  OK if success (always)
 */
inline  Error_t TimerIno::init()
{
    curTime = 0;
    return pasco2::OK;
}

/**
 * @brief   Deinitialiazes the Arduino timer
 * @return  PAS CO2 error code
 * @retval  OK if success (always)
 */
inline  Error_t TimerIno::deinit()
{
    curTime = 0;
    return pasco2::OK;
}

/**
 * @brief   Starts the Arduino timer
 * @return  PAS CO2 error code
 * @retval  OK if success (always)
 */
inline Error_t TimerIno::start()
{
    curTime = micros();
    return pasco2::OK;
}

/**
 * @brief       Elapsed time since the timer was started 
 * @param[out]  elapsed Time in milliseconds 
 * @return      PAS CO2 error code
 * @retval      OK if success (always)
 */
inline Error_t TimerIno::elapsed(uint32_t & elapsed)
{
    elapsed = (uint32_t)((micros() - curTime)/1000);
    return pasco2::OK;
}

/**
 * @brief       Elapsed time since the timer was started in microseconds
 * @param[out]  elapsed Time in microseconds
 * @return      PAS CO2 error code
 * @retval      OK if success (always)  
 */
inline Error_t TimerIno::elapsedMicro(uint32_t & elapsed)
{
    elapsed = (micros() - curTime);
    return pasco2::OK;
}

/**
 * @brief   Stops the Arduino timer
 * @return  PAS CO2 error code
 * @retval  OK if success (always)
 */
inline Error_t TimerIno::stop()
{
    //Nothing
    curTime = 0;
    return pasco2::OK;
}

/**
 * @brief       Introduces a delay during the specified time  
 * @note        No delay() implementation since it cause troubles out of sketches 
                https://www.arduino.cc/reference/en/language/functions/time/delay/
                 
 * @param[in]   timeout    Delay time in milliseconds   
 * @return      PAS CO2 error code
 * @retval      OK if success (always)
 */
inline Error_t TimerIno::delay(const uint32_t timeout)
{
    TimerIno t;
    uint32_t etime = 0;

    t.init();
    t.start();
    while(etime < timeout){ t.elapsed(etime); };
    t.stop();
    t.deinit();

    return pasco2::OK;
}