/** 
 * @file        pas-co2-test-bb.hpp
 * @brief       PAS CO2 Black Box Tests  
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef TEST_BB_HPP_
#define TEST_BB_HPP_

#include "pas-co2-serial.hpp"
#include "pas-co2-pal-timer.hpp"

#define ASSERT_ERROR_RET(err) { if(OK != err) { break; }}

void test_serialAPI     (PASCO2Serial & cotwo, Timer & timer);

// void test_serialUARTCAPI(void);

// void test_pulsePWMAPI   (void);
// void test_pulsePWMSWAPI (void);

// void test_coreAPI       (void);

#endif /** TEST_BB_H_ **/
