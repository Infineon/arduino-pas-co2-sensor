/** 
 * @file        pas-co2-reg.hpp
 * @brief       PAS CO2 Register Map
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_REG_HPP_
#define PAS_CO2_REG_HPP_

#include <stdint.h>
#include "pas-co2-types.hpp"

namespace pasco2
{

/**
 * @addtogroup co2api
 * @{
 */

class Reg
{
    public:

        /**
        * @name   Register Addresses
        * @{
        */
        enum Addr_t
        {
            REG_ADDR_PROD_ID        = (0x00U),  /**< Product and revision ID register address */
            REG_ADDR_SENS_STS       = (0x01U),  /**< Sensor status register address */
            REG_ADDR_MEAS_RATE_H    = (0x02U),  /**< Measurement period MSB configuration register address */
            REG_ADDR_MEAS_RATE_L    = (0x03U),  /**< Measurement period LSB register address */
            REG_ADDR_MEAS_CFG       = (0x04U),  /**< Measurement mode configuration register address */
            REG_ADDR_CO2PPM_H       = (0x05U),  /**< CO2 concentration result MSB register address */
            REG_ADDR_CO2PPM_L       = (0x06U),  /**< CO2 concentration result LSB register address */
            REG_ADDR_MEAS_STS       = (0x07U),  /**< Measurement status register address */
            REG_ADDR_INT_CFG        = (0x08U),  /**< Interrupt pin configuration register address */
            REG_ADDR_ALARM_TH_H     = (0x09U),  /**< Alarm threshold MSB register address */
            REG_ADDR_ALARM_TH_L     = (0x0AU),  /**< Alarm threshold LSB register address */
            REG_ADDR_PRESS_REF_H    = (0x0BU),  /**< Pressure compensation MSB register address */
            REG_ADDR_PRESS_REF_L    = (0x0CU),  /**< Pressure compensation LSB register address */
            REG_ADDR_CALIB_REF_H    = (0x0DU),  /**< Automatic baseline offset MSB register address */
            REG_ADDR_CALIB_REF_L    = (0x0EU),  /**< Automatic baseline offset LSB register address */
            REG_ADDR_SCRATCH_PAD    = (0x0FU),  /**< Scratch pad register address */
            REG_ADDR_SENS_RST       = (0x10U)   /**< Soft reset register address */
        };
        /** @} */

        static const uint8_t    regCount  = 0x11U;  /**< Number of registers */
                     uint8_t    regMap[regCount];   /**< Register map */

                    Reg                     ();
                   ~Reg                     ();

        uint8_t     getProductID            (void);
        uint8_t     getRevisionID           (void);

        bool        isSensorReady           (void);
        bool        isPWMPinEnabled         (void);

        bool        isTempOutofRange        (void);
        void        clearTempOutofRangeFlag (void);

        bool        is12VOutofRange         (void);
        void        clear12VOutofRangeFlag  (void);

        bool        hasCommErrorOccurred    (void);
        void        clearCommErrorFlag      (void);

        int16_t     getMeasRate             (void);
        void        setMeasRate             (const int16_t period);

        void        enablePWMOutput         (void);
        void        disablePWMOutput        (void);
        bool        isPWMOutputEnabled      (void);

        PWMMode_t   getPWMMode              (void);
        void        setPWMMode              (const PWMMode_t mode);

        ABOC_t      getABOCMode             (void);
        void        setABOCMode             (const ABOC_t abocMode);

        void        setOperationMode        (const OpMode_t mode);
        OpMode_t    getOperationMode        (void);

        uint16_t    getCO2PPM               (void);

        bool        isDataReady             (void);

        bool        hasInterruptOcurred     (void);
        void        clearInterruptFlag      (void);

        bool        isAlarmSet              (void);
        void        clearAlarmFlag          (void);

        IntIOConf_t getInterruptIOConf      (void);
        void        setInterruptIOConf      (const IntIOConf_t ioConf);

        Int_t       getInterruptFunc        (void);
        void        setInterrutFunc         (const Int_t func);         

        Alarm_t     getAlarmType            (void);
        void        setAlarmType            (const Alarm_t func);      

        int16_t     getAlarmThreshold       (void);
        void        setAlarmThreshold       (const int16_t alarmTh);
    
        uint16_t    getPressCompReference   (void);
        void        setPressCompReference   (const uint16_t pressRef);

        int16_t     getABOCReference        (void);
        void        setABOCReference        (const int16_t abocRef);

        uint8_t     getScratchPad           (void);
        void        setStratchPad           (const uint8_t value);

        void        setSoftReset            (void);

    private:

        /**
         * @brief Register access type
         */
        enum Access_t
        {
            REG_ACCESS_R    = (0x01U),   /**< Read access register */
            REG_ACCESS_W    = (0x02U),   /**< Write access register */
            REG_ACCESS_RW   = (0x03U),   /**< Read & write access register */
            REG_ACCESS_RH   = (0x05U),   /**< Read & internal hardware/firmware access register */
            REG_ACCESS_RWH  = (0x07U),   /**< Read & write & hardware/firmware access register */
            REG_ACCESS_S    = (0x08U),   /**< Sticky access register */
            REG_ACCESS_RHS  = (0x0DU),   /**< Read & internal sticky hardware/firmware access register */
            REG_ACCESS_RES  = (0x10U)    /**< Reserved access register */
        };

        /**
         * @brief Bit field parameters 
         */
        typedef struct 
        {
            uint8_t regAccess;      /**< Bitfield register access */
            uint8_t regAddress;     /**< Bitfiled register address */
            uint8_t mask;           /**< Bitfield mask */
            uint8_t position;       /**< Bitfiled position */
            uint8_t resetValue;     /**< Bitfield register reset value */
        }BitField_t;

        static const BitField_t bitFields[];    /**< Registers bitfields */

        /**
         * @brief Bit fields
         */
        enum BitFieldReg_t
        {
            BITF_PROD = 0,    
            BITF_REV,        
            BITF_SEN_RDY,        
            BITF_PWM_DIS_ST,      
            BITF_ORTMP,
            BITF_ORVS,
            BITF_ICCER, 
            BITF_ORTMP_CLR,
            BITF_ORVS_CLR,
            BITF_ICCER_CLR,
            BITF_RES0,
            BITF_MEAS_RATE_H,
            BITF_MEAS_RATE_L,
            BITF_RES1,
            BITF_RES2,
            BITF_PWM_OUTEN,
            BITF_PWM_MODE,
            BITF_BOC_CFG,
            BITF_OP_MODE,
            BITF_CO2_PPM_H,
            BITF_CO2_PMM_L,
            BITF_RES3,
            BITF_RES4,
            BITF_DRDY,
            BITF_INT_STS,
            BITF_ALARM,
            BITF_INT_STS_CLR,
            BITF_ALARM_CLR,
            BITF_RES5,
            BITF_INT_TYP,
            BITF_INT_FUNC,
            BITF_ALARM_TYP,
            BITF_ALARM_TH_H,
            BITF_ALARM_TH_L,
            BITF_PRESS_REF_H,
            BITF_PRESS_REF_L,
            BITF_CALIB_REF_H,
            BITF_CALIB_REF_L,
            BITF_SCRATCH_PAD,
            BITF_SRTRG
        };

        static const uint8_t softResetValue = 0xA3; /**< Value to write in the register address to perform soft reset */

        bool     getBitField     (BitField_t bitField, uint8_t & bitFValue);
        bool     setBitField     (BitField_t bitField, uint8_t bitFNewValue);
};

/** @} */

}

#endif /** PAS_CO2_REG_HPP_ **/