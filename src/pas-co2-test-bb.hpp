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


#if IS_INTF(PAS_CO2_INTF_SERIAL)
void test_serialAPI     (PASCO2Serial & cotwo, Timer & timer);
#endif /* PAS_CO2_SERIAL_ENABLED */

#if IS_INTF(PAS_CO2_INTF_PULSE)
void test_pulseAPI      (PASCO2Pulse  & cotwo, Timer & timer);
#endif /* PAS_CO2_PULSE_ENABLED */

#endif /** PAS_CO2_TEST_BB_H_ **/
