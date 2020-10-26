/** 
 * @file        pas-co2-platf-ino.cpp
 * @brief       PAS CO2 Arduino Hardware Platforms
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-platf-ino.hpp"

#if (PAS_CO2_FRAMEWORK == PAS_CO2_FRMWK_ARDUINO)

#include "pas-co2-pal-gpio-ino.hpp"

PlatformIno_t PASCO2_S2Go_XMC2Go = { 
    &Wire, 
    &Serial,  
    GPIOIno::unusedPin,
    GPIOIno::unusedPin,
    8,
    4,
    5,
    9
    };   

#endif /** PAS_CO2_FRAMEWORK **/