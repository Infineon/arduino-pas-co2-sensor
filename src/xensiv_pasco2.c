/***********************************************************************************************//**
 * \file xensiv_pasco2.c
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

#include <assert.h>

#include "xensiv_pasco2.h"

#define XENSIV_PASCO2_COMM_DELAY_MS             (5U)
#define XENSIV_PASCO2_COMM_TEST_VAL             (0xA5U)

#define XENSIV_PASCO2_SOFT_RESET_DELAY_MS       (2000U)

#define XENSIV_PASCO2_FCS_MEAS_RATE_S           (10)

#define XENSIV_PASCO2_I2C_WRITE_BUFFER_LEN      (17U)
#define XENSIV_PASCO2_UART_WRITE_XFER_BUF_SIZE  (8U)
#define XENSIV_PASCO2_UART_READ_XFER_BUF_SIZE   (5U)

#define XENSIV_PASCO2_UART_WRITE_XFER_RESP_LEN  (2U)
#define XENSIV_PASCO2_UART_READ_XFER_RESP_LEN   (3U)
#define XENSIV_PASCO2_UART_ACK                  (0x06U)
#define XENSIV_PASCO2_UART_NAK                  (0x15U)

static inline uint8_t xensiv_pasco2_digit_to_ascii(uint8_t digit)
{
    xensiv_pasco2_plat_assert(digit <= 0xFU);

    if (digit < 10U)
    {
        return (uint8_t)(digit + 0x30U);
    }
    else
    {
        return (uint8_t)(digit + 0x37U);
    }
}

static inline uint8_t xensiv_pasco2_ascii_to_digit(uint8_t ascii)
{
    xensiv_pasco2_plat_assert(((ascii >= (uint8_t)'0') && (ascii <= (uint8_t)'9')) || ((ascii >= (uint8_t)'A') && (ascii <= (uint8_t)'F')));

    if (ascii < (uint8_t)'A')
    {
        return (uint8_t)(ascii - (uint8_t)'0');
    }
    else
    {
        return (uint8_t)(10u + (ascii - (uint8_t)'A'));
    }
}

static int32_t xensiv_pasco2_i2c_read(const xensiv_pasco2_t * dev, uint8_t reg_addr, uint8_t * data, uint8_t len)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(reg_addr <= XENSIV_PASCO2_REG_SENS_RST);
    xensiv_pasco2_plat_assert(data != NULL);

    return xensiv_pasco2_plat_i2c_transfer(dev->ctx, XENSIV_PASCO2_I2C_ADDR, &reg_addr, 1, data, len);
}

static int32_t xensiv_pasco2_i2c_write(const xensiv_pasco2_t * dev, uint8_t reg_addr, const uint8_t * data, uint8_t len)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(reg_addr <= XENSIV_PASCO2_REG_SENS_RST);
    xensiv_pasco2_plat_assert(data != NULL);
    xensiv_pasco2_plat_assert((len + 1U) < XENSIV_PASCO2_I2C_WRITE_BUFFER_LEN);

    uint8_t w_data[XENSIV_PASCO2_I2C_WRITE_BUFFER_LEN];
    w_data[0] = reg_addr;
    for (uint8_t i = 0; i < len; ++i)
    {
        w_data[i + 1U] = data[i];
    }

    uint16_t w_len = (uint16_t)((uint16_t)len + 1U);

    return xensiv_pasco2_plat_i2c_transfer(dev->ctx, XENSIV_PASCO2_I2C_ADDR, w_data, w_len, NULL, 0);
}

static int32_t xensiv_pasco2_uart_read(const xensiv_pasco2_t * dev, uint8_t reg_addr, uint8_t * data, uint8_t len)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(reg_addr <= XENSIV_PASCO2_REG_SENS_RST);
    xensiv_pasco2_plat_assert(data != NULL);

    int32_t res = XENSIV_PASCO2_OK;

    for (uint8_t i = 0; i < len; ++i)
    {
        uint8_t uart_buf[XENSIV_PASCO2_UART_READ_XFER_BUF_SIZE] =
        {
            (uint8_t)'r',
            (uint8_t)',',
            xensiv_pasco2_digit_to_ascii((reg_addr & (uint8_t)0xF0) >> 4U),
            xensiv_pasco2_digit_to_ascii(reg_addr & (uint8_t)0x0F),
            (uint8_t)'\n'
        };

        res = xensiv_pasco2_plat_uart_write(dev->ctx, uart_buf, XENSIV_PASCO2_UART_READ_XFER_BUF_SIZE);

        if (XENSIV_PASCO2_OK == res)
        {
            res = xensiv_pasco2_plat_uart_read(dev->ctx, uart_buf, XENSIV_PASCO2_UART_READ_XFER_RESP_LEN);
            if (XENSIV_PASCO2_OK == res)
            {
                data[i] = (uint8_t)((xensiv_pasco2_ascii_to_digit(uart_buf[0]) << 4) + xensiv_pasco2_ascii_to_digit(uart_buf[1]));
            }
        }

        if (XENSIV_PASCO2_OK != res)
        {
            break;
        }

        reg_addr++;
    }

    return res;
}

static int32_t xensiv_pasco2_uart_write(const xensiv_pasco2_t * dev, uint8_t reg_addr, const uint8_t * data, uint8_t len)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(reg_addr <= XENSIV_PASCO2_REG_SENS_RST);
    xensiv_pasco2_plat_assert(data != NULL);

    int32_t res = XENSIV_PASCO2_OK;

    for (uint8_t i = 0; i < len; ++i)
    {
        uint8_t uart_buf[XENSIV_PASCO2_UART_WRITE_XFER_BUF_SIZE] =
        {
            (uint8_t)'w',
            (uint8_t)',',
            xensiv_pasco2_digit_to_ascii((reg_addr & 0xF0U) >> 4U), xensiv_pasco2_digit_to_ascii(reg_addr & 0x0FU),
            (uint8_t)',',
            xensiv_pasco2_digit_to_ascii((data[i] & 0xF0U) >> 4U), xensiv_pasco2_digit_to_ascii(data[i] & 0x0FU),
            (uint8_t)'\n'
        };

        res = xensiv_pasco2_plat_uart_write(dev->ctx, uart_buf, XENSIV_PASCO2_UART_WRITE_XFER_BUF_SIZE);

        if (XENSIV_PASCO2_OK == res)
        {
            res = xensiv_pasco2_plat_uart_read(dev->ctx, uart_buf, XENSIV_PASCO2_UART_WRITE_XFER_RESP_LEN);

            /* If command triggers a software reset ignores the sensor response */
            if ((XENSIV_PASCO2_REG_SENS_RST != reg_addr) || ((uint8_t)XENSIV_PASCO2_CMD_SOFT_RESET != data[i]))
            {
                res = (XENSIV_PASCO2_OK == res) ?
                      ((XENSIV_PASCO2_UART_ACK == uart_buf[0]) ? XENSIV_PASCO2_OK : XENSIV_PASCO2_ERR_COMM) :
                      XENSIV_PASCO2_ERR_COMM;
            }
            else
            {
                res = XENSIV_PASCO2_OK;
            }
        }

        if (XENSIV_PASCO2_OK != res)
        {
            break;
        }

           reg_addr++;
    }

    return res;
}

static int32_t xensiv_pasco2_init(const xensiv_pasco2_t * dev)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    /* Check communication */
    uint8_t data = XENSIV_PASCO2_COMM_TEST_VAL;

    int32_t res = xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_SCRATCH_PAD, &data, 1U);

    if (XENSIV_PASCO2_OK == res)
    {
        res = xensiv_pasco2_get_reg(dev, (uint8_t)XENSIV_PASCO2_REG_SCRATCH_PAD, &data, 1U);
    }

    if ((XENSIV_PASCO2_OK == res) && (XENSIV_PASCO2_COMM_TEST_VAL == data))
    {
        /* Soft reset */
        res = xensiv_pasco2_cmd(dev, XENSIV_PASCO2_CMD_SOFT_RESET);
        xensiv_pasco2_plat_delay(XENSIV_PASCO2_SOFT_RESET_DELAY_MS);

        if (XENSIV_PASCO2_OK == res)
        {
            /* Read the sensor status and verify if the sensor is ready */
            res = xensiv_pasco2_get_reg(dev, (uint8_t)XENSIV_PASCO2_REG_SENS_STS, &data, 1U);
        }

        if (XENSIV_PASCO2_OK == res)
        {
            if ((data & XENSIV_PASCO2_REG_SENS_STS_ICCER_MSK) != 0U)
            {
                res = XENSIV_PASCO2_ICCERR;
            }
            else if ((data & XENSIV_PASCO2_REG_SENS_STS_ORVS_MSK) != 0U)
            {
                res = XENSIV_PASCO2_ORVS;
            }
            else if ((data & XENSIV_PASCO2_REG_SENS_STS_ORTMP_MSK) != 0U)
            {
                res = XENSIV_PASCO2_ORTMP;
            }
            else if ((data & XENSIV_PASCO2_REG_SENS_STS_SEN_RDY_MSK) == 0U)
            {
                res = XENSIV_PASCO2_ERR_NOT_READY;
            }
            else
            {
                res = XENSIV_PASCO2_OK;
            }
        }
    }
    else
    {
        res = XENSIV_PASCO2_ERR_COMM;
    }

    return res;
}

int32_t xensiv_pasco2_init_i2c(xensiv_pasco2_t * dev, void * ctx)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(ctx != NULL);

    dev->ctx = ctx;
    dev->read = xensiv_pasco2_i2c_read;
    dev->write = xensiv_pasco2_i2c_write;

    return xensiv_pasco2_init(dev);
}

int32_t xensiv_pasco2_init_uart(xensiv_pasco2_t * dev, void * ctx)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(ctx != NULL);

    dev->ctx = ctx;
    dev->read = xensiv_pasco2_uart_read;
    dev->write = xensiv_pasco2_uart_write;

    return xensiv_pasco2_init(dev);
}

int32_t xensiv_pasco2_set_reg(const xensiv_pasco2_t * dev, uint8_t reg_addr, const uint8_t * data, uint8_t len)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(data != NULL);

    int32_t res = dev->write(dev, reg_addr, data, len);
    xensiv_pasco2_plat_delay(XENSIV_PASCO2_COMM_DELAY_MS);

    return res;
}

int32_t xensiv_pasco2_get_reg(const xensiv_pasco2_t * dev, uint8_t reg_addr, uint8_t * data, uint8_t len)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(data != NULL);

    int32_t res = dev->read(dev, reg_addr, data, len);
    xensiv_pasco2_plat_delay(XENSIV_PASCO2_COMM_DELAY_MS);

    return res;
}

int32_t xensiv_pasco2_get_id(const xensiv_pasco2_t * dev, xensiv_pasco2_id_t * id)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(id != NULL);

    return xensiv_pasco2_get_reg(dev, (uint8_t)XENSIV_PASCO2_REG_PROD_ID, &(id->u), 1U);
}

int32_t xensiv_pasco2_get_status(const xensiv_pasco2_t * dev, xensiv_pasco2_status_t * status)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(status != NULL);

    return xensiv_pasco2_get_reg(dev, (uint8_t)XENSIV_PASCO2_REG_SENS_STS, &(status->u), 1U);
}

int32_t xensiv_pasco2_clear_status(const xensiv_pasco2_t * dev, uint8_t mask)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    return xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_SENS_STS, &mask, 1U);
}

int32_t xensiv_pasco2_get_interrupt_config(const xensiv_pasco2_t * dev, xensiv_pasco2_interrupt_config_t * int_config)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(int_config != NULL);

    return xensiv_pasco2_get_reg(dev, (uint8_t)XENSIV_PASCO2_REG_INT_CFG, &(int_config->u), 1U);
}

int32_t xensiv_pasco2_set_interrupt_config(const xensiv_pasco2_t * dev, xensiv_pasco2_interrupt_config_t int_config)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    return xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_INT_CFG, &(int_config.u), 1U);
}

int32_t xensiv_pasco2_get_measurement_config(const xensiv_pasco2_t * dev, xensiv_pasco2_measurement_config_t * meas_config)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(meas_config != NULL);

    return xensiv_pasco2_get_reg(dev, (uint8_t)XENSIV_PASCO2_REG_MEAS_CFG, &(meas_config->u), 1U);
}

int32_t xensiv_pasco2_set_measurement_config(const xensiv_pasco2_t * dev, xensiv_pasco2_measurement_config_t meas_config)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    return xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_MEAS_CFG, &(meas_config.u), 1U);
}

int32_t xensiv_pasco2_get_result(const xensiv_pasco2_t * dev, uint16_t * val)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(val != NULL);

    xensiv_pasco2_meas_status_t meas_status;
    int32_t res = xensiv_pasco2_get_measurement_status(dev, &meas_status);

    if (XENSIV_PASCO2_OK == res)
    {
        if (meas_status.b.drdy != 0U)
        {
            res = xensiv_pasco2_get_reg(dev, (uint8_t)XENSIV_PASCO2_REG_CO2PPM_H, (uint8_t *)val, 2U);
            *val = xensiv_pasco2_plat_htons(*val);
        }
        else
        {
            res =  XENSIV_PASCO2_READ_NRDY;
        }
    }

    return res;
}

int32_t xensiv_pasco2_set_measurement_rate(const xensiv_pasco2_t * dev, uint16_t val)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert((val >= XENSIV_PASCO2_MEAS_RATE_MIN) && (val <= XENSIV_PASCO2_MEAS_RATE_MAX));

    val = xensiv_pasco2_plat_htons(val);
    return xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_MEAS_RATE_H, (uint8_t *)&val, 2U);
}

int32_t xensiv_pasco2_get_measurement_status(const xensiv_pasco2_t * dev, xensiv_pasco2_meas_status_t * status)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(status != NULL);

    return xensiv_pasco2_get_reg(dev, (uint8_t)XENSIV_PASCO2_REG_MEAS_STS, &(status->u), 1U);
}

int32_t xensiv_pasco2_clear_measurement_status(const xensiv_pasco2_t * dev, uint8_t mask)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    return xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_MEAS_STS, &mask, 1U);
}

int32_t xensiv_pasco2_set_alarm_threshold(const xensiv_pasco2_t * dev, uint16_t val)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    val = (uint16_t)xensiv_pasco2_plat_htons(val);
    return xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_ALARM_TH_H, (uint8_t *)&val, 2U);
}

int32_t xensiv_pasco2_set_pressure_compensation(const xensiv_pasco2_t * dev, uint16_t val)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    val = (uint16_t)xensiv_pasco2_plat_htons(val);
    return xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_PRESS_REF_H, (uint8_t *)&val, 2U);
}

int32_t xensiv_pasco2_set_offset_compensation(const xensiv_pasco2_t * dev, uint16_t val)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    val = (uint16_t)xensiv_pasco2_plat_htons(val);
    return xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_CALIB_REF_H, (uint8_t *)&val, 2U);
}

int32_t xensiv_pasco2_set_scratch_pad(const xensiv_pasco2_t * dev, uint8_t val)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    return xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_SCRATCH_PAD, &val, 1U);
}

int32_t xensiv_pasco2_get_scratch_pad(const xensiv_pasco2_t * dev, uint8_t * val)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert(val != NULL);

    return xensiv_pasco2_get_reg(dev, (uint8_t)XENSIV_PASCO2_REG_SCRATCH_PAD, val, 1U);
}

int32_t xensiv_pasco2_cmd(const xensiv_pasco2_t * dev, xensiv_pasco2_cmd_t cmd)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    return xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_SENS_RST, (const uint8_t * )&cmd, 1U);
}

int32_t xensiv_pasco2_start_single_mode(const xensiv_pasco2_t * dev)
{
    xensiv_pasco2_plat_assert(dev != NULL);

    xensiv_pasco2_measurement_config_t meas_config;
    int32_t res = xensiv_pasco2_get_measurement_config(dev, &meas_config);

    if (XENSIV_PASCO2_OK == res)
    {
        if (meas_config.b.op_mode != XENSIV_PASCO2_OP_MODE_IDLE)
        {
            meas_config.b.op_mode = XENSIV_PASCO2_OP_MODE_IDLE;
            res = xensiv_pasco2_set_measurement_config(dev, meas_config);
        }
    }

    if (XENSIV_PASCO2_OK == res)
    {
        meas_config.b.op_mode = XENSIV_PASCO2_OP_MODE_SINGLE;
        meas_config.b.boc_cfg = XENSIV_PASCO2_BOC_CFG_AUTOMATIC;
        res = xensiv_pasco2_set_measurement_config(dev, meas_config);
    }

    return res;
}

int32_t xensiv_pasco2_start_continuous_mode(const xensiv_pasco2_t * dev, uint16_t val)
{
    xensiv_pasco2_plat_assert(dev != NULL);
    xensiv_pasco2_plat_assert((val >= XENSIV_PASCO2_MEAS_RATE_MIN) && (val <= XENSIV_PASCO2_MEAS_RATE_MAX));

    xensiv_pasco2_measurement_config_t meas_config;
    int32_t res = xensiv_pasco2_get_measurement_config(dev, &meas_config);

    if (XENSIV_PASCO2_OK == res)
    {
        if (meas_config.b.op_mode != XENSIV_PASCO2_OP_MODE_IDLE)
        {
            meas_config.b.op_mode = XENSIV_PASCO2_OP_MODE_IDLE;
            res = xensiv_pasco2_set_measurement_config(dev, meas_config);
        }
    }

    if (XENSIV_PASCO2_OK == res)
    {
        val = xensiv_pasco2_plat_htons(val);
        res = xensiv_pasco2_set_reg(dev, (uint8_t)XENSIV_PASCO2_REG_MEAS_RATE_H, (uint8_t *)&val, 2U);
    }

    if (XENSIV_PASCO2_OK == res)
    {
        meas_config.b.op_mode = XENSIV_PASCO2_OP_MODE_CONTINUOUS;
        meas_config.b.boc_cfg = XENSIV_PASCO2_BOC_CFG_AUTOMATIC;
        res = xensiv_pasco2_set_measurement_config(dev, meas_config);
    }

    return res;
}

int32_t xensiv_pasco2_perform_forced_compensation(const xensiv_pasco2_t * dev, uint16_t co2_ref)
{
    xensiv_pasco2_measurement_config_t meas_config;
    int32_t res = xensiv_pasco2_get_measurement_config(dev, &meas_config);

    if (XENSIV_PASCO2_OK == res)
    {
        meas_config.b.op_mode = XENSIV_PASCO2_OP_MODE_IDLE;
        res = xensiv_pasco2_set_measurement_config(dev, meas_config);
    }

    if (XENSIV_PASCO2_OK == res)
    {
        res = xensiv_pasco2_set_measurement_rate(dev, XENSIV_PASCO2_FCS_MEAS_RATE_S);
    }

    if (XENSIV_PASCO2_OK == res)
    {
        res = xensiv_pasco2_set_offset_compensation(dev, co2_ref);
    }

    if (XENSIV_PASCO2_OK == res)
    {
        meas_config.b.op_mode = XENSIV_PASCO2_OP_MODE_CONTINUOUS;
        meas_config.b.boc_cfg = XENSIV_PASCO2_BOC_CFG_FORCED;
        res = xensiv_pasco2_set_measurement_config(dev, meas_config);
    }

    if (XENSIV_PASCO2_OK == res)
    {
        /* wait until the FCS is finished */
        do
        {
            res = xensiv_pasco2_get_measurement_config(dev, &meas_config);
        } while ((XENSIV_PASCO2_OK != res) || (XENSIV_PASCO2_BOC_CFG_FORCED == meas_config.b.boc_cfg));
    }

    if (XENSIV_PASCO2_OK == res)
    {
        meas_config.b.op_mode = XENSIV_PASCO2_OP_MODE_IDLE;
        res = xensiv_pasco2_set_measurement_config(dev, meas_config);
    }

    if (XENSIV_PASCO2_OK == res)
    {
        res = xensiv_pasco2_cmd(dev, XENSIV_PASCO2_CMD_SAVE_FCS_CALIB_OFFSET);
    }

    return res;
}

__attribute__((weak)) int32_t xensiv_pasco2_plat_i2c_transfer(void * ctx, uint16_t dev_addr, const uint8_t * tx_buffer, size_t tx_len, uint8_t * rx_buffer, size_t rx_len)
{
    (void)ctx;
    (void)dev_addr;
    (void)tx_buffer;
    (void)tx_len;
    (void)rx_buffer;
    (void)rx_len;

    return XENSIV_PASCO2_ERR_COMM;
}

__attribute__((weak)) int32_t xensiv_pasco2_plat_uart_read(void *ctx, uint8_t * data, size_t len)
{
    (void)ctx;
    (void)data;
    (void)len;

    return XENSIV_PASCO2_ERR_COMM;
}

__attribute__((weak)) int32_t xensiv_pasco2_plat_uart_write(void *ctx, uint8_t * data, size_t len)
{
    (void)ctx;
    (void)data;
    (void)len;

    return XENSIV_PASCO2_ERR_COMM;
}

__attribute__((weak)) void xensiv_pasco2_plat_delay(uint32_t ms)
{
    (void)ms;
}

__attribute__((weak)) uint16_t xensiv_pasco2_plat_htons(uint16_t x)
{
    return ((uint16_t)(((x & 0x00ffU) << 8) |
                       ((x & 0xff00U) >> 8)));
}

__attribute__((weak)) void xensiv_pasco2_plat_assert(int expr)
{
    assert(expr);
}
