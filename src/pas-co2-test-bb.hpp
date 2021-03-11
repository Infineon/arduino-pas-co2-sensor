/** 
 * @file        pas-co2-test-bb.hpp
 * @brief       PAS CO2 Black Box Tests  
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_TEST_BB_HPP_
#define PAS_CO2_TEST_BB_HPP_

#include "pas-co2-serial.hpp"
#include "pas-co2-pulse.hpp"
#include "pas-co2-pal-timer.hpp"

#define ASSERT_ERROR_RET(err)\
{\
    if(OK != err)\
    {\
        PAS_CO2_APP_LOG_RETURN(err);\
        break;\
    }\
}

void test_serialAPI     (PASCO2Serial & cotwo, Timer & timer);
void test_pulseAPI      (PASCO2Pulse  & cotwo, Timer & timer);

#endif /** PAS_CO2_TEST_BB_H_ **/
