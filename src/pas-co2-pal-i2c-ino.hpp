/** 
 * @file        pas-co2-pal-i2c-ino.hpp
 * @brief       PAS CO2 I2C Arduino PAL  
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PAL_I2C_INO_H_
#define PAS_CO2_PAL_I2C_INO_H_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_I2C)

#include <Wire.h>
#include "pas-co2-pal-i2c.hpp"

using namespace pasco2;

/**
 * @addtogroup co2inopal
 * @{
 */

class I2CPALIno : virtual public I2CPAL
{
    public:
                   I2CPALIno     (TwoWire * wire = &Wire);
                  ~I2CPALIno     ();
        Error_t    init          ();
        Error_t    deinit        ();
        Error_t    setClockFreq  (const uint32_t clockHz);
        Error_t    read          (const uint8_t slave7BAddr, const uint8_t memAddr,       uint8_t * data, uint16_t & length);
        Error_t    write         (const uint8_t slave7BAddr, const uint8_t memAddr, const uint8_t * data, uint16_t & length);

    private:

        TwoWire * wire;

        static const uint8_t maxRequestFromBytes = 255; /* Maximum read bytes allowed by requestFrom() Wire "read function. */

};

/** @} */

extern I2CPALIno i2cpalino;

#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_PAL_I2C_INO_H_ **/