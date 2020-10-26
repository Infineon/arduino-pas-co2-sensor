/** 
 * @file        pas-co2-reg.cpp
 * @brief       PAS CO2 Register Map
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-reg.hpp"

using namespace pasco2;

#define REG_TBD 0x00    /**< To be defined */
#define REG_DS 0x00     /**< Device specific */

const Reg::BitField_t Reg::bitFields[] =
{
    { REG_ACCESS_R,     REG_ADDR_PROD_ID,       0xE0, 5, REG_DS },  /**< 0.  BITF_PROD */
    { REG_ACCESS_R,     REG_ADDR_PROD_ID,       0x1F, 0, REG_DS },  /**< 1.  BITF_REV */
    { REG_ACCESS_RH,    REG_ADDR_SENS_STS,      0x80, 7, 0x80   },  /**< 2.  BITF_SEN_RDY */
    { REG_ACCESS_RH,    REG_ADDR_SENS_STS,      0x40, 6, 0x80   },  /**< 3.  BITF_PWM_DIS_ST */
    { REG_ACCESS_RHS,   REG_ADDR_SENS_STS,      0x20, 5, 0x80   },  /**< 4.  BITF_ORTMP */
    { REG_ACCESS_RHS,   REG_ADDR_SENS_STS,      0x10, 4, 0x80   },  /**< 5.  BITF_ORVS */
    { REG_ACCESS_RHS,   REG_ADDR_SENS_STS,      0x08, 3, 0x80   },  /**< 6.  BITF_ICCER */
    { REG_ACCESS_W,     REG_ADDR_SENS_STS,      0x04, 2, 0x80   },  /**< 7.  BITF_ORTMP_CLR */
    { REG_ACCESS_W,     REG_ADDR_SENS_STS,      0x02, 1, 0x80   },  /**< 8.  BITF_ORVS_CLR */
    { REG_ACCESS_W,     REG_ADDR_SENS_STS,      0x01, 0, 0x80   },  /**< 9.  BITF_ICCER_CLR*/
    { REG_ACCESS_RES,   REG_ADDR_MEAS_RATE_H,   0xF0, 4, 0x00   },  /**< 10. BITF_RES0*/  
    { REG_ACCESS_RW,    REG_ADDR_MEAS_RATE_H,   0x0F, 0, 0x00   },  /**< 11. BITF_MEAS_RATE_H */
    { REG_ACCESS_RW,    REG_ADDR_MEAS_RATE_L,   0xFF, 0, 0x32   },  /**< 12. BITF_MEAS_RATE_L */
    { REG_ACCESS_RES,   REG_ADDR_MEAS_CFG,      0x80, 7, 0x22   },  /**< 13. BITF_RES1 */
    { REG_ACCESS_RES,   REG_ADDR_MEAS_CFG,      0x40, 6, 0x22   },  /**< 14. BITF_RES2 */
    { REG_ACCESS_RWH,   REG_ADDR_MEAS_CFG,      0x20, 5, 0x22   },  /**< 15. BITF_PWM_OUTEN */
    { REG_ACCESS_RW,    REG_ADDR_MEAS_CFG,      0x10, 4, 0x22   },  /**< 16. BITF_PWM_MODE */
    { REG_ACCESS_RWH,   REG_ADDR_MEAS_CFG,      0x0C, 2, 0x22   },  /**< 17. BITF_BOC_CFG */
    { REG_ACCESS_RWH,   REG_ADDR_MEAS_CFG,      0x03, 0, 0x22   },  /**< 18. BITF_OP_MODE */
    { REG_ACCESS_R,     REG_ADDR_CO2PPM_H,      0xFF, 0, 0x00   },  /**< 19. BITF_CO2_PPM_H */
    { REG_ACCESS_R,     REG_ADDR_CO2PPM_L,      0xFF, 0, 0x00   },  /**< 20. BITF_CO2_PMM_L */
    { REG_ACCESS_RES,   REG_ADDR_MEAS_STS,      0xC0, 6, 0x00   },  /**< 21. BITF_RES3 */
    { REG_ACCESS_RES,   REG_ADDR_MEAS_STS,      0x20, 5, 0x00   },  /**< 22. BITF_RES4 */
    { REG_ACCESS_RHS,   REG_ADDR_MEAS_STS,      0x10, 4, 0x00   },  /**< 23. BITF_DRDY */
    { REG_ACCESS_RHS,   REG_ADDR_MEAS_STS,      0x08, 3, 0x00   },  /**< 24. BITF_INT_STS */
    { REG_ACCESS_RHS,   REG_ADDR_MEAS_STS,      0x04, 2, 0x00   },  /**< 25. BITF_ALARM */
    { REG_ACCESS_W,     REG_ADDR_MEAS_STS,      0x02, 1, 0x00   },  /**< 26. BITF_INT_STS_CLR */
    { REG_ACCESS_W,     REG_ADDR_MEAS_STS,      0x01, 0, 0x00   },  /**< 27. BITF_ALARM_CLR */
    { REG_ACCESS_RW,    REG_ADDR_INT_CFG,       0xE0, 5, 0x00   },  /**< 28. BITF_RES5 */
    { REG_ACCESS_RW,    REG_ADDR_INT_CFG,       0x10, 4, 0x00   },  /**< 29. BITF_INT_TYP */
    { REG_ACCESS_RW,    REG_ADDR_INT_CFG,       0x0E, 1, 0x00   },  /**< 30. BITF_INT_FUNC */
    { REG_ACCESS_RW,    REG_ADDR_INT_CFG,       0x01, 0, 0x00   },  /**< 31. BITF_ALARM_TYP */
    { REG_ACCESS_RW,    REG_ADDR_ALARM_TH_H,    0xFF, 0, 0x00   },  /**< 32. BITF_ALARM_TH_H */
    { REG_ACCESS_RW,    REG_ADDR_ALARM_TH_L,    0xFF, 0, 0x00   },  /**< 33. BITF_ALARM_TH_L */
    { REG_ACCESS_RW,    REG_ADDR_PRESS_REF_H,   0xFF, 0, REG_TBD},  /**< 34. BITF_PRESS_REF_H */
    { REG_ACCESS_RW,    REG_ADDR_PRESS_REF_L,   0xFF, 0, REG_TBD},  /**< 35. BITF_PRESS_REF_L */
    { REG_ACCESS_RW,    REG_ADDR_CALIB_REF_H,   0xFF, 0, 0x00   },  /**< 36. BITF_CALIB_REF_H */
    { REG_ACCESS_RW,    REG_ADDR_CALIB_REF_L,   0xFF, 0, 0x00   },  /**< 37. BITF_CALIB_REF_L */
    { REG_ACCESS_RW,    REG_ADDR_SCRATCH_PAD,   0xFF, 0, 0x00   },  /**< 38. BITF_SCRATCH_PAD */
    { REG_ACCESS_W,     REG_ADDR_SENS_RST,      0xFF, 0, 0x00   }   /**< 39. BITF_SRTRG */
};


Reg::Reg()
: regMap()
{

}

Reg::~Reg()
{

}

uint8_t Reg::getProductID(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_PROD], bitf);
    
    return bitf;
}

uint8_t Reg::getRevisionID(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_REV], bitf);
    
    return bitf;
}

bool Reg::isSensorReady(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_SEN_RDY], bitf);
    
    return bitf;
}

bool Reg::isPWMPinEnabled(void)
{
    uint8_t bitf = 0x00;

    getBitField(bitFields[BITF_PWM_DIS_ST], bitf);
    
    return !bitf;
}

bool Reg::isTempOutofRange(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_ORTMP], bitf);
    
    return bitf;
}   

void Reg::clearTempOutofRangeFlag(void)
{
    setBitField(bitFields[BITF_ORTMP_CLR], 1);
}

bool Reg::is12VOutofRange(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_ORVS], bitf);
    
    return bitf;
}

void Reg::clear12VOutofRangeFlag(void)
{
    setBitField(bitFields[BITF_ORVS_CLR], 1);
}

bool Reg::hasCommErrorOccurred(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_ICCER], bitf);
    
    return bitf;
}

void Reg::clearCommErrorFlag(void)
{
    setBitField(bitFields[BITF_ICCER_CLR], 1);
}

int16_t Reg::getMeasRate(void)
{
    uint8_t bitf      = 0x00;
    uint16_t measRate = 0x00;

    getBitField(bitFields[BITF_MEAS_RATE_H], bitf);
    measRate = (bitf << 8);
    getBitField(bitFields[BITF_MEAS_RATE_L], bitf);
    measRate |= bitf;

    return measRate;
}

void Reg::setMeasRate(const int16_t period)
{
    setBitField(bitFields[BITF_MEAS_RATE_H], (period >> 8));
    setBitField(bitFields[BITF_MEAS_RATE_L], (period & 0xFF));
}

void Reg::enablePWMOutput(void)
{
    setBitField(bitFields[BITF_PWM_OUTEN], 1);
}

void Reg::disablePWMOutput(void)
{
    setBitField(bitFields[BITF_PWM_OUTEN], 0);
}

bool Reg::isPWMOutputEnabled(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_PWM_OUTEN], bitf);
    
    return bitf;
}

PWMMode_t Reg::getPWMMode(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_PWM_MODE], bitf);
    
    return (PWMMode_t)bitf;
}

void Reg::setPWMMode(const PWMMode_t mode)
{
    setBitField(bitFields[BITF_PWM_MODE], mode);
}

ABOC_t Reg::getABOCMode(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_BOC_CFG], bitf);
    
    return (ABOC_t)bitf;
}

void Reg::setABOCMode(const ABOC_t abocMode)
{
    setBitField(bitFields[BITF_BOC_CFG], abocMode);
}


OpMode_t Reg::getOperationMode(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_OP_MODE], bitf);
    
    return (OpMode_t)bitf;
}

void Reg::setOperationMode(const OpMode_t mode)
{
    setBitField(bitFields[BITF_OP_MODE], mode);
}


uint16_t Reg::getCO2PPM(void)
{
    uint8_t bitf    = 0x00;
    uint16_t co2ppm = 0x00;

    getBitField(bitFields[BITF_CO2_PPM_H], bitf);
    co2ppm = (bitf << 8);
    getBitField(bitFields[BITF_CO2_PMM_L], bitf);
    co2ppm |= bitf;

    return co2ppm;
}

bool Reg::isDataReady(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_DRDY], bitf);
    
    return bitf;
}

bool Reg::hasInterruptOcurred(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_INT_STS], bitf);
    
    return bitf;
}

void Reg::clearInterruptFlag(void)
{
    setBitField(bitFields[BITF_INT_STS_CLR], 1);
}

bool Reg::isAlarmSet(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_ALARM], bitf);
    
    return bitf;
}

void Reg::clearAlarmFlag(void)
{
    setBitField(bitFields[BITF_ALARM_CLR], 1);
}

IntIOConf_t Reg::getInterruptIOConf(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_INT_TYP], bitf);
    
    return (IntIOConf_t)bitf;
}

void Reg::setInterruptIOConf(const IntIOConf_t ioConf)
{
    setBitField(bitFields[BITF_INT_TYP], ioConf);
}

Int_t Reg::getInterruptFunc(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_INT_FUNC], bitf);
    
    return (Int_t)bitf;
}

void Reg::setInterrutFunc(const Int_t func)
{
    setBitField(bitFields[BITF_INT_FUNC], func);
}       

Alarm_t Reg::getAlarmType(void)
{
    uint8_t bitf = 0x00;
    
    getBitField(bitFields[BITF_ALARM_TYP], bitf);
    
    return (Alarm_t)bitf;
}

void Reg::setAlarmType(const Alarm_t func)
{
    setBitField(bitFields[BITF_ALARM_TYP], func);
}

int16_t Reg::getAlarmThreshold(void)
{
    uint8_t bitf     = 0x00;
    uint16_t alarmth = 0x00;

    getBitField(bitFields[BITF_ALARM_TH_H], bitf);
    alarmth = (bitf << 8);
    getBitField(bitFields[BITF_ALARM_TH_L], bitf);
    alarmth |= bitf;

    return alarmth;
}

void Reg::setAlarmThreshold(const int16_t alarmTh)
{
    setBitField(bitFields[BITF_ALARM_TH_H], (alarmTh >> 8));
    setBitField(bitFields[BITF_ALARM_TH_L], (alarmTh & 0xFF));
}

uint16_t Reg::getPressCompReference(void)
{
    uint8_t bitf    = 0x00;
    uint16_t pressc = 0x00;

    getBitField(bitFields[BITF_PRESS_REF_H], bitf);
    pressc = (bitf << 8);
    getBitField(bitFields[BITF_PRESS_REF_L], bitf);
    pressc |= bitf;

    return pressc;
}

void Reg::setPressCompReference(const uint16_t pressRef)
{
    setBitField(bitFields[BITF_PRESS_REF_H], (pressRef >> 8));
    setBitField(bitFields[BITF_PRESS_REF_L], (pressRef & 0xFF));
}

int16_t Reg::getABOCReference(void)
{
    uint8_t bitf     = 0x00;
    uint16_t abocref = 0x00;

    getBitField(bitFields[BITF_CALIB_REF_H], bitf);
    abocref = (bitf << 8);
    getBitField(bitFields[BITF_CALIB_REF_L], bitf);
    abocref |= bitf;

    return abocref;
}

void Reg::setABOCReference(const int16_t abocRef)
{
    setBitField(bitFields[BITF_CALIB_REF_H], (abocRef >> 8));
    setBitField(bitFields[BITF_CALIB_REF_L], (abocRef & 0xFF));
}

uint8_t Reg::getScratchPad(void)
{
    uint8_t bitf = 0x00;

    getBitField(bitFields[BITF_SCRATCH_PAD], bitf);

    return bitf;
}

void Reg::setStratchPad(const uint8_t value)
{
    setBitField(bitFields[BITF_SCRATCH_PAD], value);
}

void Reg::setSoftReset(void)
{
    setBitField(bitFields[BITF_SRTRG], softResetValue);
}

/**
 * @brief       Gets the bit field value 
 * 
 * @param[in]   bitField    Bit field parameters structure
 * @param[out]  bitFValue   Value of the bit field
 * @retval      TRUE if success
 * @retval      FALSE if register access denied
 * @pre         None
 */
bool Reg::getBitField(BitField_t bitField, uint8_t & bitFValue)
{
    bool err = false;

    if( (REG_ACCESS_R & bitField.regAccess) ==  REG_ACCESS_R)
    {
        bitFValue =  ((regMap[bitField.regAddress] & bitField.mask) >> bitField.position); 
        err = true;
    }

    return err;
}

/**
 * @brief       Sets the bit field value 
 * 
 * @param[in]   bitField        Bit field parameters structure
 * @param[in]   bitFNewValue    Value of the bit field
 * @retval      TRUE if success
 * @retval      FALSE if register access denied
 * @pre         None
 */
bool Reg::setBitField(BitField_t bitField, uint8_t bitFNewValue)
{
    bool err = false;

    if( (REG_ACCESS_W & bitField.regAccess) == REG_ACCESS_W)
    {
        regMap[bitField.regAddress] = (regMap[bitField.regAddress] & ~bitField.mask) | ((bitFNewValue << bitField.position) & bitField.mask);
        err = true;
    }

    return err;
}