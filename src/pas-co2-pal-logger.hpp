/** 
 * @file        pas-co2-pal-logger.hpp
 * @brief       PAS CO2 Logger Platform Abstraction Layer
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PAL_LOGGER_HPP_
#define PAS_CO2_PAL_LOGGER_HPP_

#include "pas-co2-conf.hpp"

#if (PAS_CO2_LOGGER_ENABLED == 1)

#include <stdint.h>
#include "pas-co2-types.hpp"

namespace pasco2
{

/**
 * @addtogroup co2pal
 * @{
 */

class LoggerPAL
{
    public:
    
        /**
         * @brief       Initializes the Logger interface
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t     init    () = 0;
        
        /**
         * @brief       Deinitializes the logger interface 
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t     deinit  () = 0;
        
        /**
         * @brief       Writes in the logger interface
         * @param[in]   *logData    Pointer to the array holding the data to be logged
         * @param[in]   length      Length in bytes of the data to be written 
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INTF_ERROR if error
         */
        virtual Error_t     write   (const uint8_t * logData, uint32_t length) = 0;
};

/** @} */

}

#endif /* PAS_CO2_LOGGER_ENABLED */
#endif /**< PAS_CO2_PAL_LOGGER_HPP_ */