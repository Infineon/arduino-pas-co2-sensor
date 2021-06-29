/** 
 * @file        pas-co2-platf-ino.cpp
 * @brief       PAS CO2 Arduino Hardware Platforms
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */


#include "pas-co2-platf-ino.hpp"
#include "pas-co2-pal-gpio-ino.hpp"
#include "pas-co2-conf.hpp"

/**
 * @brief Default platform selection by conditional compiling
 */ 

#ifdef XMC1100_XMC2GO                   /**< XMC2Go */

PlatformIno_t PASCO2_S2Go_XMC2Go = { 
    &Wire, 
    nullptr,  
    GPIOIno::unusedPin,
    GPIOIno::unusedPin,
    8,
    4,
    5,
    9
};  

PlatformIno_t * pltf = &PASCO2_S2Go_XMC2Go;

#if IS_INTF(PAS_CO2_INTF_UART)
#warning  "XMC2Go only has one Serial interface. It can be allocated \
for serial debugging through USB-UART inteface with the PC, \
or in the board pins 6 and 7. If UART mode is configured \
for the sensor, no serial debugging is available."
#endif      

#elif defined(XMC4700_Relax_Kit)       /**< XMC4700 relax kit */

PlatformIno_t PASCO2_S2Go_XMC4700 = {
    &Wire, 
    &Serial1,  
    GPIOIno::unusedPin,
    GPIOIno::unusedPin,
    5,
    17,
    9,
    2
};

PlatformIno_t * pltf  = &PASCO2_S2Go_XMC4700;

#elif defined(XMC1100_Boot_Kit)       /**< XMC1100 boot kit */

PlatformIno_t PASCO2_S2Go_XMC1100BK = {
    &Wire, 
    nullptr,  
    GPIOIno::unusedPin,
    GPIOIno::unusedPin,
    5,
    17,
    9,
    2  
};

PlatformIno_t * pltf  = &PASCO2_S2Go_XMC1100BK;

#if IS_INTF(PAS_CO2_INTF_UART)
#warning  "XMC2Go only has one Serial interface. It can be allocated \
for serial debugging through USB-UART inteface with the PC, \
or in the board pins 6 and 7. If UART mode is configured \
for the sensor, no serial debugging is available."
#endif    

#else

PlatformIno_t PASCO2_dflt_pltf = {
    &Wire, 
    nullptr,  
    GPIOIno::unusedPin,
    GPIOIno::unusedPin,
    GPIOIno::unusedPin,
    GPIOIno::unusedPin,
    GPIOIno::unusedPin,
    GPIOIno::unusedPin  
};

PlatformIno_t * pltf = &PASCO2_dflt_pltf;

#endif

