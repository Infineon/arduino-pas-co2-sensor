/** 
 * @file        pas-co2-pal-gpio-ino.hpp
 * @brief       PAS CO2 GPIO Arduino PAL  
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PAL_GPIO_INO_HPP_
#define PAS_CO2_PAL_GPIO_INO_HPP_

#include <stdint.h>
#include "pas-co2-pal-gpio.hpp"

using namespace pasco2;

/**
 * @addtogroup co2inopal
 * @{
 */

class GPIOIno : virtual public pasco2::GPIO 
{
    private:
        uint8_t     pin;                /**< Pin number*/
        uint8_t     mode;               /**< Pin mode*/
        VLogic_t    logic;              /**< Pin logic */

    public:

        static const uint8_t       unusedPin = 0xFFU; /**< Unused pin */

                    GPIOIno       (uint8_t  pin, 
                                   uint8_t  mode, 
                                   VLogic_t logic);
                   ~GPIOIno       ();
        Error_t     init          ();            
        Error_t     deinit        ();
        Error_t     enableInt     (void (*cback) (void *));    
        Error_t     disableInt    ();
        IntEvent_t  intEvent      ();
        VLevel_t    read          ();
        Error_t     write         (VLevel_t level);
        Error_t     enable        ();
        Error_t     disable       ();
};

/** @} */

#endif /** PAS_CO2_PAL_GPIO_INO_HPP_ **/

