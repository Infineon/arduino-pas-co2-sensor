/** 
 * @file        pas-co2-pal-timer.hpp
 * @brief       PAS CO2 Timer Platform Abstraction Layer
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_PAL_TIMER_HPP_
#define PAS_CO2_PAL_TIMER_HPP_

#include <stdint.h>
#include "pas-co2-types.hpp"

namespace pasco2
{

/**
 * @addtogroup co2pal
 * @{
 */

class Timer
{
    public: 

        /**
         * @brief       Initialiazes the timer
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INIT_ERROR if error
         */
        virtual  Error_t         init           () = 0;

        /**
         * @brief       Deinitialiazes the timer
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INIT_ERROR if error
         */
        virtual  Error_t         deinit         () = 0;

        /**
         * @brief       Starts the timer
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INIT_ERROR if error
         */
        virtual  Error_t         start          () = 0;

        /**
         * @brief       Elapsed time since the timer was started in milliseconds
         * @param[out]  elapsed Time in milliseconds 
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INIT_ERROR if error    
         */
        virtual  Error_t         elapsed        (uint32_t & elapsed) = 0;

        /**
         * @brief       Elapsed time since the timer was started in microseconds
         * @param[out]  elapsed Time in microseconds
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INIT_ERROR if error    
         */
        virtual  Error_t         elapsedMicro   (uint32_t & elapsed) = 0;
        
        /**
         * @brief       Stops the timer
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INIT_ERROR if error
         */
         virtual  Error_t         stop          () = 0;
        
        /**
         * @brief       Introduces a delay during the specified time    
         * @param[in]   timeout    Delay time in milliseconds   
         * @return      PAS CO2 error code
         * @retval      OK if success
         * @retval      INIT_ERROR if error
         */
        virtual  Error_t         delay   (const uint32_t timeout) = 0;
};

/** @} */

}

#endif /** PAS_CO2_PAL_TIMER_HPP_ **/