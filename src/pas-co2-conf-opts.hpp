/** 
 * @file        pas-co2-conf-opts.hpp
 * @brief       PAS CO2 Library Configuration Options
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_CONF_OPTS_HPP_
#define PAS_CO2_CONF_OPTS_HPP_

/**
 * @name Sensor interfaces
 * @{
 */

#define PAS_CO2_INTF_SERIAL     0x01U                       /**< Serial interface */
#define PAS_CO2_INTF_UART       0x03U                       /**< Serial UART interface */
#define PAS_CO2_INTF_I2C        0x05U                       /**< Serial I2C Interface */
#define PAS_CO2_INTF_PULSE      0x08U                       /**< Pulse interface */
#define PAS_CO2_INTF_PWM        0x18U                       /**< Hardware PWM interface */
#define PAS_CO2_INTF_PWM_SW     0x28U                       /**< Software PWM interface */
#define PAS_CO2_INTF_ALL        (PAS_CO2_INTF_UART | \
                                 PAS_CO2_INTF_I2C  | \
                                 PAS_CO2_INTF_PWM  | \
                                 PAS_CO2_INTF_PWM_SW)       /**< All interface enabled */
/** @} */

#endif /** PAS_CO2_OPTS_HPP_ **/