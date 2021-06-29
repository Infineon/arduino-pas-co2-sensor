/** 
 * @file        pas-co2-conf.hpp
 * @brief       PAS CO2 Library Configuration
 * 
 * C Macro defines for library conditional compilation of the library modules.
 * The available options of each define are listed in the file
 * pas-co2-conf-opts.hpp.
 * If any parameter is not defined, either as a compilation flag or in this 
 * file, the default value will be defined in pas-co2-conf-dfl.hpp. 
 * 
 * - Framework. Selection of the specific PAL implementation.
 * - Interface. The sensor includes different firmware modules to operate the sensor
 *              via I2C or UART, or/and PWM. Through this define, those unused modules
 *              are not added during compilation.
 * - Logger.    The logger and its different levels enable serial trace debugging of the 
 *              library. When enabled the program size is significantly increased. Thus,
 *              it is recommended to disable it for final productive releases.
 * 
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 *              
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_CONF_HPP_
#define PAS_CO2_CONF_HPP_

#include "pas-co2-conf-opts.hpp"

/**
 * @brief Sensor physical interface
 */

#ifndef PAS_CO2_INTF 
#define PAS_CO2_INTF                PAS_CO2_INTF_ALL           
#endif

/**
 * @brief Check if the interface is included
 *        in the defined interface 
 */

#define IS_INTF(_INTF_)             ((PAS_CO2_INTF & _INTF_) == _INTF_)     

/**
 * @brief Logger enable
 */

#ifndef PAS_CO2_LOGGER_ENABLED
#define PAS_CO2_LOGGER_ENABLED      1           /**< (1) Enabled, (0) Disabled */
#endif

/**
 * @brief Enable for each module logger
 */

#if     (PAS_CO2_LOGGER_ENABLED == 1)

#define PAS_CO2_CORE_LOGGER_ENABLED     0           /**< Core module */
#define PAS_CO2_SERIAL_LOGGER_ENABLED   0           /**< Serial module */
#define PAS_CO2_I2C_LOGGER_ENABLED      0           /**< I2C module */
#define PAS_CO2_UART_LOGGER_ENABLED     0           /**< UART module */
#define PAS_CO2_PULSE_LOGGER_ENABLED    0           /**< Pulse module */
#define PAS_CO2_PWM_LOGGER_ENABLED      0           /**< PWM SW module */
#define PAS_CO2_REG_LOGGER_ENABLED      0           /**< Register module */
#define PAS_CO2_APP_LOGGER_ENABLED      1           /**< Application level module */

#endif 

#include "pas-co2-conf-dfl.hpp"

#endif /** PAS_CO2_CONF_HPP_ **/