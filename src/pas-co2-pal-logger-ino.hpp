/** 
 * @file        pas-co2-pal-logger-ino.hpp
 * @brief       PAS CO2 Logger Arduino PAL 
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PAL_LOGGER_INO_H_
#define PAS_CO2_PAL_LOGGER_INO_H_

#include "pas-co2-conf.hpp"
#include "pas-co2-pal-logger.hpp"

#if (PAS_CO2_LOGGER_ENABLED == 1)

using namespace pasco2;

/**
 * @addtogroup co2inopal
 * @{
 */

class LoggerIno : public LoggerPAL
{
        public:
                        LoggerIno    ();
                       ~LoggerIno    (); 
            Error_t     init         ();
            Error_t     deinit       ();
            Error_t     write        (const uint8_t * log_data, uint32_t length);
};

/** @} */

/**
 * @brief Default LoggerIno
 */
extern LoggerIno logpalino;

#endif /* PAS_CO2_LOGGER_ENABLED */
#endif /** PAS_CO2_PAL_LOGGER_INO_H_ **/