/***********************************************************************************************//**
 * \file xensiv_pasco2.h
 *
 * Description: This file contains the functions for interacting with the
 *              XENSIV™ PAS CO2 sensor.
 *
 ***************************************************************************************************
 * \copyright
 * Copyright 2021 Infineon Technologies AG
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **************************************************************************************************/

#ifndef XENSIV_PASCO2_H_
#define XENSIV_PASCO2_H_

/**
 * \addtogroup group_board_libs XENSIV™ PAS CO2 sensor
 * \{
 * Library which provides a platform-independent driver for the XENSIV™ PAS CO2 sensor.
 * It provides full access to all features of the sensor.
 *
 * The library depends on the target platform-specific implementation of the following functions:
 * - \ref xensiv_pasco2_plat_i2c_transfer
 * - \ref xensiv_pasco2_plat_uart_read, \ref xensiv_pasco2_plat_uart_write
 * - \ref xensiv_pasco2_plat_delay
 * - \ref xensiv_pasco2_plat_htons
 * - \ref xensiv_pasco2_plat_assert
 *
 * The driver provides a default implementation of these functions with weak linkage.
 * \ref xensiv_pasco2_plat_i2c_transfer must be overridden when using the I2C interface.
 * \ref xensiv_pasco2_plat_uart_read, \ref xensiv_pasco2_plat_uart_write must be overridden when using the UART interface.
 * \ref xensiv_pasco2_plat_delay must be overridden with an appropriate implementation for the target platform that delays the processing for a certain number of milliseconds.
 * \ref xensiv_pasco2_plat_htons implements byte reversing in C and can be overridden optionally to optimize the performance if the target platform provides a specific instruction to byte reversing.
 * \ref xensiv_pasco2_plat_assert is implemented using the standard assert.h, and can be optionally overriden for the target platform.
 *
 * See the implementation example of these functions in xensiv_pasco2_mtb.c
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "xensiv_pasco2_regs.h"

/************************************** Macros *******************************************/

/** Result code indicating a successful operation */
#define XENSIV_PASCO2_OK                    (0)
/** Result code indicating a communication error */
#define XENSIV_PASCO2_ERR_COMM              (1)
/** Result code indicating that an unexpectedly large I2C write was requested which is not supported */
#define XENSIV_PASCO2_ERR_WRITE_TOO_LARGE   (2)
/** Result code indicating that the sensor is not yet ready after reset */
#define XENSIV_PASCO2_ERR_NOT_READY         (3)
/** Result code indicating whether a non-valid command has been received by the serial communication interface */
#define XENSIV_PASCO2_ICCERR                (4)
/** Result code indicating whether a condition where VDD12V has been outside the specified valid range has been detected */
#define XENSIV_PASCO2_ORVS                  (5)
/** Result code indicating whether a condition where the temperature has been outside the specified valid range has been detected */
#define XENSIV_PASCO2_ORTMP                 (6)
/** Result code indicating that a new CO2 value is not yet ready */
#define XENSIV_PASCO2_READ_NRDY             (7)

/** Minimum allowed measurement rate */
#define XENSIV_PASCO2_MEAS_RATE_MIN         (5U)

/** Maximum allowed measurement rate */
#define XENSIV_PASCO2_MEAS_RATE_MAX         (4095U)

/** I2C address of the XENSIV™ PASCO2 sensor */
#define XENSIV_PASCO2_I2C_ADDR              (0x28U)

/********************************* Type definitions **************************************/

/** Enum defining the different device commands */
typedef enum
{
    XENSIV_PASCO2_CMD_SOFT_RESET = 0xA3U,               /**< Soft reset the sensor */
    XENSIV_PASCO2_CMD_RESET_ABOC = 0xBCU,               /**< Resets the ABOC context */
    XENSIV_PASCO2_CMD_SAVE_FCS_CALIB_OFFSET = 0xCFU,    /**< Saves the force calibration offset into the non volatile memory */
    XENSIV_PASCO2_CMD_RESET_FCS = 0xFCU,                /**< Resets the forced calibration correction factor */
} xensiv_pasco2_cmd_t;

/** Enum defining the different device operating modes */
typedef enum
{
    XENSIV_PASCO2_OP_MODE_IDLE = 0U,                    /**< The device does not perform any CO2 concentration measurement */
    XENSIV_PASCO2_OP_MODE_SINGLE = 1U,                  /**< The device triggers a single measurement sequence. At the end of the measurement sequence, the device automatically goes back to idle mode. */
    XENSIV_PASCO2_OP_MODE_CONTINUOUS = 2U               /**< The device periodically triggers a CO2 concentration measurement sequence.
                                                             Once a measurement sequence is completed, the device goes back to an inactive state and wakes
                                                             up automatically for the next measurement sequence. The measurement period can be programmed from 5 seconds to 4095 seconds. */
} xensiv_pasco2_op_mode_t;

/** Enum defining the different device baseline offset compensation (BOC) modes */
typedef enum
{
    XENSIV_PASCO2_BOC_CFG_DISABLE = 0U,                 /**< No offset compensation occurs */
    XENSIV_PASCO2_BOC_CFG_AUTOMATIC = 1U,               /**< The offset is periodically updated at each BOC computation */
    XENSIV_PASCO2_BOC_CFG_FORCED = 2U                   /**< Forced compensation */
} xensiv_pasco2_boc_cfg_t;

/** Enum defining the PWM mode configuration */
typedef enum
{
    XENSIV_PASCO2_PWM_MODE_SINGLE_PULSE = 0U,           /**< PWM single-pulse */
    XENSIV_PASCO2_PWM_MODE_TRAIN_PULSE = 1U             /**< PWM pulse-train mode */
} xensiv_pasco2_pwm_mode_t;

/** Enum defining different interrupt active levels */
typedef enum
{
    XENSIV_PASCO2_INTERRUPT_TYPE_LOW_ACTIVE = 0U,       /**< Pin INT is configured as push-pull and is active LOW */
    XENSIV_PASCO2_INTERRUPT_TYPE_HIGH_ACTIVE = 1U       /**< Pin INT is configured as push-pull and is active HIGH */
} xensiv_pasco2_interrupt_type_t;

/** Enum defining different pin interrupt functions */
typedef enum
{
    XENSIV_PASCO2_INTERRUPT_FUNCTION_NONE = 0U,         /**< Pin INT is inactive */
    XENSIV_PASCO2_INTERRUPT_FUNCTION_ALARM = 1U,        /**< Pin INT is configured as the alarm threshold violation notification pin */
    XENSIV_PASCO2_INTERRUPT_FUNCTION_DRDY = 2U,         /**< Pin INT is configured as the data ready notification pin */
    XENSIV_PASCO2_INTERRUPT_FUNCTION_BUSY = 3U,         /**< Pin INT is configured as the sensor busy notification pin */
    XENSIV_PASCO2_INTERRUPT_FUNCTION_EARLY = 4U         /**< Pin INT is configured as the early measurement start notification pin
                                                             @note This function is available only in continuous mode */
} xensiv_pasco2_interrupt_function_t;

/** Enum defining whether an alarm is issued in the case of a lower or higher threshold violation */
typedef enum
{
    XENSIV_PASCO2_ALARM_TYPE_HIGH_TO_LOW = 0U,          /**< CO2 ppm value falling below the alarm threshold */
    XENSIV_PASCO2_ALARM_TYPE_LOW_TO_HIGH = 1U           /**< CO2 ppm value rising above the alarm threshold */
} xensiv_pasco2_alarm_type_t;

/** Structure of the sensor's product and revision ID register (PROD_ID) */
typedef union
{
  struct
  {
    uint32_t rev:5;                                     /*!< Product and firmware revision */
    uint32_t prod:3;                                    /*!< Product type */
  } b;                                                  /*!< Structure used for bit  access */
  uint8_t u;                                            /*!< Type used for byte access */
} xensiv_pasco2_id_t;

/** Structure of the sensor's status register (SENS_STS) */
typedef union
{
  struct
  {
    uint32_t :3;
    uint32_t iccerr:1;                                  /*!< Communication error notification bit.
                                                             Indicates whether an invalid command has been received by the serial communication interface*/
    uint32_t orvs:1;                                    /*!< Out-of-range VDD12V error bit */
    uint32_t ortmp:1;                                   /*!< Out-of-range temperature error bit */
    uint32_t pwm_dis_st:1;                              /*!< PWM_DIS pin status */
    uint32_t sen_rdy:1;                                 /*!< Sensor ready bit */
  } b;                                                  /*!< Structure used for bit  access */
  uint8_t u;                                            /*!< Type used for byte access */
} xensiv_pasco2_status_t;

/** Structure of the sensor's measurement configuration register (MEAS_CFG) */
typedef union
{
  struct
  {
    uint32_t op_mode:2;                                 /*!< @ref xensiv_pasco2_op_mode_t */
    uint32_t boc_cfg:2;                                 /*!< @ref xensiv_pasco2_boc_cfg_t */
    uint32_t pwm_mode:1;                                /*!< @ref xensiv_pasco2_pwm_mode_t */
    uint32_t pwm_outen:1;                               /*!< PWM output software enable bit */
    uint32_t :2;
  } b;                                                  /*!< Structure used for bit  access */
  uint8_t u;                                            /*!< Type used for byte access */
} xensiv_pasco2_measurement_config_t;

/** Structure of the sensor's interrupt configuration register (INT_CFG) */
typedef union
{
  struct
  {
    uint32_t alarm_typ:1;                               /*!< @ref xensiv_pasco2_alarm_type_t */
    uint32_t int_func:3;                                /*!< @ref xensiv_pasco2_interrupt_function_t */
    uint32_t int_typ:1;                                 /*!< @ref xensiv_pasco2_interrupt_type_t */
    uint32_t :3;
  } b;                                                  /*!< Structure used for bit access */
  uint8_t u;                                            /*!< Type used for byte access */
} xensiv_pasco2_interrupt_config_t;

/** Structure of the sensor's measurement status register (MEAS_STS) */
typedef union
{
  struct
  {
    uint32_t :2;
    uint32_t alarm:1;                                   /*!< Set at the end of every measurement sequence if a threshold violation occurs */
    uint32_t int_sts:1;                                 /*!< Indicates whether the INT pin has been latched to active state (if alarm or data is ready) */
    uint32_t drdy:1;                                    /*!< Indicates whether new data is available */
    uint32_t :3;
  } b;                                                  /*!< Structure used for bit  access */
  uint8_t u;                                            /*!< Type used for byte access */
} xensiv_pasco2_meas_status_t;

struct xensiv_pasco2;                                   /* Forward declaration */

/* Function pointer to the platform-specific function for reading the sensor registers via I2C/UART */
typedef int32_t (*xensiv_pasco2_read_fptr_t)(const struct xensiv_pasco2 * dev, uint8_t reg_addr, uint8_t * data, uint8_t len);

/* Function pointer to the platform-specific function for writing  the sensor registers via I2C/UART */
typedef int32_t (*xensiv_pasco2_write_fptr_t)(const struct xensiv_pasco2 * dev, uint8_t reg_addr, const uint8_t * data, uint8_t len);

/** Structure of the XENSIV™ PAS CO2 sensor device. Initialized using \ref xensiv_pasco2_init_i2c or \ref xensiv_pasco2_init_uart */
typedef struct xensiv_pasco2
{
    void * ctx;                         /*!< Context for I2C/UART platform-specific read and write functions */
    xensiv_pasco2_read_fptr_t read;     /*!< Pointer to the register read function which depends on the communication interface used */
    xensiv_pasco2_write_fptr_t write;   /*!< Pointer to the register write function which depends on the communication interface used */
} xensiv_pasco2_t;

/******************************* Function prototypes *************************************/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the XENSIV™ PAS CO2 device using the I2C interface.
 * It initializes the dev structure, verifies the integrity of the communication layer of the serial communication interface, and checks whether the sensor is ready
 *
 * @param[inout] dev Pointer to a XENSIV™ PAS CO2 sensor device structure allocated by the user,
 * but the init function will initialize its contents
 * @param[in] ctx Pointer to the platform-specific I2C communication handler
 * @return XENSIV_PASCO2_OK if the initialization was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_init_i2c(xensiv_pasco2_t * dev, void *ctx);

/**
 * @brief Initializes the XENSIV™ PAS CO2 device using the UART interface.
 * It initializes the dev structure, verifies the integrity of the communication layer of the serial communication interface, and checks whether the sensor is ready
 *
 * @param[inout] dev Pointer to a XENSIV™ PAS CO2 sensor device structure allocated by user,
 * but the init function will initialize its contents
 * @param[in] ctx Pointer to the platform-specific I2C communication handler
 * @return XENSIV_PASCO2_OK if the initialization was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_init_uart(xensiv_pasco2_t * dev, void *ctx);

/**
 * @brief Writes the given data buffer into the sensor device.
 * Writes the given data buffer to the sensor register map starting at the register address
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] reg_addr Start register address
 * @param[in] data Pointer to the data buffer to be written in the sensor
 * @param[in] len Number of bytes of data to be written
 * @return XENSIV_PASCO2_OK if writing to the register was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_set_reg(const xensiv_pasco2_t * dev, uint8_t reg_addr, const uint8_t * data, uint8_t len);

/**
 * @brief Reads from the sensor device into the given data buffer.
 * Reads from the sensor register map sensor starting at register address into the given data buffer
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] reg_addr Start register address
 * @param[out] data Pointer to the data buffer to store the register values of the sensor
 * @param[in] len Number of bytes of data to be read
 * @return XENSIV_PASCO2_OK if reading from the register was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_get_reg(const xensiv_pasco2_t * dev, uint8_t reg_addr, uint8_t * data, uint8_t len);

/**
 * @brief Gets the sensor device product and version ID
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[out] id Pointer to populate with the sensor device product and version ID
 * @note : Refer to the register map description of the XENSIV™ PAS CO2 device for detailed information on the ID format
 * @return XENSIV_PASCO2_OK if reading the product id was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_get_id(const xensiv_pasco2_t * dev, xensiv_pasco2_id_t * id);

/**
 * @brief Gets the sensor device status
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[out] status Pointer to populate with the sensor device status
 * @return XENSIV_PASCO2_OK if reading the device status was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_get_status(const xensiv_pasco2_t * dev, xensiv_pasco2_status_t * status);

/**
 * @brief Clears the sensor device status bits
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] mask ORed combination of the following values:
 *                 @arg @ref XENSIV_PASCO2_REG_SENS_STS_ICCER_CLR_MSK Clears the ICCER status sticky bit
 *                 @arg @ref XENSIV_PASCO2_REG_SENS_STS_ORVS_CLR_MSK  Clears the ORVS status sticky bit
 *                 @arg @ref XENSIV_PASCO2_REG_SENS_STS_ORTMP_CLR_MSK Clears the ORTMP status sticky bit
 * @return XENSIV_PASCO2_OK if the status bits clearing was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_clear_status(const xensiv_pasco2_t * dev, uint8_t mask);

/**
 * @brief Gets the sensor device interrupt configuration
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[out] int_config Pointer to populate with the sensor device interrupt configuration
 * @return XENSIV_PASCO2_OK if getting the interrupt configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_get_interrupt_config(const xensiv_pasco2_t * dev, xensiv_pasco2_interrupt_config_t * int_config);

/**
 * @brief Sets the sensor device interrupt configuration
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] int_config New sensor device interrupt configuration to apply
 * @return XENSIV_PASCO2_OK if setting the interrupt configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_set_interrupt_config(const xensiv_pasco2_t * dev, xensiv_pasco2_interrupt_config_t int_config);

/**
 * @brief Gets the sensor device measurement configuration
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[out] meas_config Pointer to populate with the sensor device measurement configuration
 * @return XENSIV_PASCO2_OK if getting the measurement configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_get_measurement_config(const xensiv_pasco2_t * dev, xensiv_pasco2_measurement_config_t * meas_config);

/**
 * @brief Sets the sensor device measurement configuration
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] meas_config New sensor device measurement configuration to apply
 * @return XENSIV_PASCO2_OK if setting the measurement configuration was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_set_measurement_config(const xensiv_pasco2_t * dev, xensiv_pasco2_measurement_config_t meas_config);

/**
 * @brief Gets the current CO2 ppm values from the sensor device
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[out] val Pointer to populate with the CO2 ppm value
 * @return XENSIV_PASCO2_OK if obtaining the current CO2 value successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_get_result(const xensiv_pasco2_t * dev, uint16_t * val);

/**
 * @brief Sets the measurement rate for continuos mode
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] val New measurement rate to apply [5-4095s]
 * @return XENSIV_PASCO2_OK if setting the measurement rate was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_set_measurement_rate(const xensiv_pasco2_t * dev, uint16_t val);

/**
 * @brief Gets the measurement status of the sensor device.
 * Used to check if a new CO2 concentration value is available to be read (status.b.drdy) using \ref xensiv_pasco2_get_result
 * Used to check if an interrupt is asserted (status.b.int_sts).
 * Used to check if an alarm is triggered (status.b.alarm)
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[out] status Pointer to populate with the sensor device measurement status
 * @return XENSIV_PASCO2_OK if getting the measurement was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_get_measurement_status(const xensiv_pasco2_t * dev, xensiv_pasco2_meas_status_t * status);

/**
 * @brief Clears the measurement status of the sensor device
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] mask ORed combination of the following values:
 *                 @arg @ref XENSIV_PASCO2_REG_MEAS_STS_INT_STS_CLR_MSK Clears the sticky bit MEAS_STS.INT_STS and forces the INT pin to inactive level
 *                 @arg @ref XENSIV_PASCO2_REG_MEAS_STS_ALARM_CLR_MSK   Clears the sticky bit MEAS_STS.ALARM
 * @return XENSIV_PASCO2_OK if clearing the measurement status selected bits was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_clear_measurement_status(const xensiv_pasco2_t * dev, uint8_t mask);

/**
 * @brief Sets the alarm threshold
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] val New alarm threshold value to apply
 * @return XENSIV_PASCO2_OK if setting the alarm threshold was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_set_alarm_threshold(const xensiv_pasco2_t * dev, uint16_t val);

/**
 * @brief Sets the pressure compensation value.
 * The CO2 concentration value acquired by the sensor is dependent on the external atmospheric pressure. To
 * compensate for this effect, the application can provide the value of the atmospheric pressure using this
 * function. At the end of a measurement sequence, the device reads the pressure value and applies it for
 * compensation on the CO2 concentration value before storing it into the result registers, and can be
 * retrieved using \ref xensiv_pasco2_get_result
 *
 * @param[in] dev Pointer to a XENSIV™ PAS CO2 sensor device
 * @param[in] val New pressure compensation value to apply
 * @return XENSIV_PASCO2_OK if setting the pressure reference value was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_set_pressure_compensation(const xensiv_pasco2_t * dev, uint16_t val);

/**
 * @brief Sets the offset compensation value
 * Defines the reference value used for ABOC and force calibration
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] val New pressure calibration value to apply
 * @return XENSIV_PASCO2_OK if setting the measurement offset was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_set_offset_compensation(const xensiv_pasco2_t * dev, uint16_t val);

/**
 * @brief Writes to the scratchpad register
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] val New scratchpad register value to apply
 * @return XENSIV_PASCO2_OK if writing to the scratch pad register was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_set_scratch_pad(const xensiv_pasco2_t * dev, uint8_t val);

/**
 * @brief Reads from the scratchpad register
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[out] val Pointer to populate with the sensor device scratchpad register value
 * @return XENSIV_PASCO2_OK if reading from the scratch pad register was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_get_scratch_pad(const xensiv_pasco2_t * dev, uint8_t * val);

/**
 * @brief Triggers a sensor device command
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] cmd Command to trigger
 * @return XENSIV_PASCO2_OK if triggering the command  was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_cmd(const xensiv_pasco2_t * dev, xensiv_pasco2_cmd_t cmd);

/**
 * @brief Triggers a single mode measurement
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @return XENSIV_PASCO2_OK if starting the measurement was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_start_single_mode(const xensiv_pasco2_t * dev);

/**
 * @brief Starts measurements in continuous mode
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] val Measurement rate to apply [5-4095s]
 * @return XENSIV_PASCO2_OK if starting the measurements was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_start_continuous_mode(const xensiv_pasco2_t * dev, uint16_t val);

/**
 * @brief Performs force compensation.
 * Used to calculate the offset compensation when the sensor is exposed to a CO2 reference value.
 * The device is left is idle mode and the new offset compensation value is stored in non-volatile memory.
 *
 * @param[in] dev Pointer to the XENSIV™ PAS CO2 sensor device
 * @param[in] cmd Command to trigger
 * @return XENSIV_PASCO2_OK if the force compensation was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_perform_forced_compensation(const xensiv_pasco2_t * dev, uint16_t co2_ref);

/**
 * @brief Target platform-specific function to perform I2C write/read transfer.
 * Synchronously writes a block of data and optionally receive a block of data.
 * @param[in] ctx Target platform object
 * @param[in] dev_addr device address (7-bit)
 * @param[in] tx_buffer I2C send data
 * @param[in] tx_len I2C send data size
 * @param[in] rx_buffer I2C receive data @note Can be NULL to indicate no read access.
 * @param[in] rx_len I2C receive data size
 * @return XENSIV_PASCO2_OK if the I2C transfer was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_plat_i2c_transfer(void * ctx, uint16_t dev_addr, const uint8_t * tx_buffer, size_t tx_len, uint8_t * rx_buffer, size_t rx_len);

/**
 * @brief Target platform-specific function to read over UART
 *
 * @param[in] ctx UART object
 * @param[out] data Receive buffer
 * @param[in] len Number of bytes to receive
 * @return XENSIV_PASCO2_OK if the UART read was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_plat_uart_read(void *ctx, uint8_t * data, size_t len);

/**
 * @brief Target platform-specific function to write over UART
 *
 * @param[in] ctx UART object
 * @param[in] data Transmit buffer
 * @param[in] len Number of bytes to transmit
 * @return XENSIV_PASCO2_OK if the UART write was successful; an error indicating what went wrong otherwise
 */
int32_t xensiv_pasco2_plat_uart_write(void *ctx, uint8_t * data, size_t len);

/**
 * @brief Target platform-specific function that waits for a specified time period in milliseconds
 *
 * @param[in] ms Number of miliseconds to wait for
 */
void xensiv_pasco2_plat_delay(uint32_t ms);

/**
 * @brief Target platform-specific function to reverse the byte order (16-bit)
 *
 * @param[in] x Value to reverse
 * @return Reversed value
 */
uint16_t xensiv_pasco2_plat_htons(uint16_t x);

/**
 * @brief Target platform-specific function that implements a runtime assertion; used to verify the assumptions made by the program and take appropiate actions if assumption is false
 *
 * @param[in] expr Expression to be verified
 */
void xensiv_pasco2_plat_assert(int expr);

#ifdef __cplusplus
}
#endif

/** \} group_board_libs */

#endif
