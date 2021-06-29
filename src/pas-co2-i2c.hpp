/** 
 * @file        pas-co2-i2c.hpp
 * @brief       PAS CO2 I2C Protocol
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_I2C_HPP_
#define PAS_CO2_I2C_HPP_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_I2C)

#include <stdint.h>
#include "pas-co2-sbus.hpp"
#include "pas-co2-pal-i2c.hpp"

namespace pasco2
{

/**
 * @addtogroup co2api
 * @{
 */

class I2C : public SBus
{
    public:

        static const uint32_t dfltFreq = 400000;    /**< Default I2C SCKL frequency */

                 I2C        (I2CPAL   * const i2cpal,
                             uint32_t   freqHz = dfltFreq);
        virtual ~I2C        (void);

        Error_t  init       (void);
        Error_t  deinit     (void);
  
        Error_t  write      (const uint8_t   regAddr,
                             const uint8_t * data,
                                   uint16_t  length);
        Error_t  read       (const uint8_t   regAddr,       
                                   uint8_t * data,
                                   uint16_t  length);

        /**
         * @brief       Returns the bus protocol
         * @retval      BUS_PROTO_I2C
         */
        Proto_t  getProtocol(void)  { return BUS_PROTO_I2C; };     

    private:

        I2CPAL * const i2cpal;
        uint32_t       clkFreq;             /**< Signal clock frequency */
        const uint8_t  addr7bits = 0x28;    /**< Device adddress 7 bits format */
};

/** @} */

extern I2C i2c;

}

#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_I2C_H_ **/