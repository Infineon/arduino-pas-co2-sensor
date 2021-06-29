/** 
 * @file        pas-co2-pal-i2c-ino.cpp
 * @brief       PAS CO2 I2C Arduino PAL  
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-pal-i2c-ino.hpp"

#if IS_INTF(PAS_CO2_INTF_I2C)

#include <stdarg.h>
#include "pas-co2-i2c.hpp"
#include "pas-co2-logger.hpp"

using namespace pasco2;

#define PAS_CO2_I2C_PALINO_LOGGER_ENABLED 0

#if ((PAS_CO2_LOGGER_ENABLED == 1) && (PAS_CO2_I2C_PALINO_LOGGER_ENABLED == 1))

#define PAS_CO2_I2C_PALINO_LOGGER_SERVICE               "[ino i2c pal]   : "
#define PAS_CO2_I2C_PALINO_LOGGER_WRITE_SERVICE         "[ino i2c pal]   : >>> h' "
#define PAS_CO2_I2C_PALINO_LOGGER_READ_SERVICE          "[ino i2c pal]   : <<< h' "
#define PAS_CO2_I2C_PALINO_LOGGER_COLOR                  PAS_CO2_LOGGER_COLOR_LIGHT_BLUE

#define PAS_CO2_I2C_PALINO_LOG_MSG(str)\
{\
    cotwo_log.printfModule(str, PAS_CO2_I2C_PALINO_LOGGER_SERVICE, PAS_CO2_I2C_PALINO_LOGGER_COLOR);\
}

#define PAS_CO2_I2C_PALINO_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        cotwo_log.printfModule("fail with return code %i", PAS_CO2_I2C_PALINO_LOGGER_SERVICE, PAS_CO2_LOGGER_ERROR_COLOR, ret);\
    }\
    else\
    {\
        cotwo_log.printfModule("pass", PAS_CO2_I2C_PALINO_LOGGER_SERVICE, PAS_CO2_I2C_PALINO_LOGGER_COLOR);\
    }\
}

#define PAS_CO2_I2C_PALINO_READ_HEX(array, length)\
{\
    cotwo_log.printModuleHex(array, length, PAS_CO2_I2C_PALINO_LOGGER_READ_SERVICE, PAS_CO2_I2C_PALINO_LOGGER_COLOR);\
}

#define PAS_CO2_I2C_PALINO_WRITE_HEX(array, length)\
{\
    cotwo_log.printModuleHex(array, length, PAS_CO2_I2C_PALINO_LOGGER_WRITE_SERVICE, PAS_CO2_I2C_PALINO_LOGGER_COLOR);\
}

#else

#define PAS_CO2_I2C_PALINO_LOG_MSG(str)                {   }  
#define PAS_CO2_I2C_PALINO_LOG_RETURN(ret)             {   }
#define PAS_CO2_I2C_PALINO_READ_HEX(array, length)     {   }
#define PAS_CO2_I2C_PALINO_WRITE_HEX(array, length)    {   } 

#endif

/**
 * @brief       I2C PAL Ino constructor
 * @param[in]   wire Arduino Wire instance
 */
I2CPALIno::I2CPALIno(TwoWire * wire)
: wire(wire)
{

}

/**
 * @brief   I2C PAL Ino destructor
 */
I2CPALIno::~I2CPALIno()
{

}

/**
 * @brief       Sets the I2C Arduino PAL frequency 
 * @param[in]   clockHz  Clock frequency in Hertzs
 * @return      PAS CO2 error code
 * @retval      OK always
 */
Error_t I2CPALIno::setClockFreq(const uint32_t clockHz)
{
    Error_t err = pasco2::OK;

    PAS_CO2_I2C_PALINO_LOG_MSG(__FUNCTION__);

    wire->setClock((uint32_t)clockHz);

    PAS_CO2_I2C_PALINO_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Initializes the I2C Arduino PAL
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if error
 */
Error_t I2CPALIno::init()
{
    Error_t err = pasco2::OK;

    PAS_CO2_I2C_PALINO_LOG_MSG(__FUNCTION__);

    wire->begin();

    PAS_CO2_I2C_PALINO_LOG_RETURN(err);

    return err;
}

/**
 * @brief       Deinitializes the I2C Arduino PAL
 * @warning     end() call not implemented depending on core
 *              Arduino toolchain flags are not here declared.
 * @return      PAS CO2 error code
 * @retval      OK always
 */
Error_t I2CPALIno::deinit()
{	
    PAS_CO2_I2C_PALINO_LOG_MSG(__FUNCTION__);

    // #if defined(/* core supporting end() */)
    // wire->end();
    // #endif

    PAS_CO2_I2C_PALINO_LOG_RETURN(pasco2::OK);
    
    return pasco2::OK;
}

/**
 * @brief           Reads from the I2C Arduino PAL the device memory registers
 *                  
 *  The memory addresses length is 8 bits, which point to 8 bits word memory registers.
 *  In bulk read mode (length > 1 byte), the register address passed by argument will be the 
 *  start read address, and the subsequent registers will be read up until the requested 
 *  length is satisfied.
 *  
 * @param[in]       slave7BAddr Slave address 7 bits format. Without write/read bit (8 bit address right shifted 1 bit)
 * @param[in]       memAddress  Device memory register start read address 
 * @param[out]      *data       Pointer to the array that will store the read data
 * @param[in,out]   length      Number of bytes to be read from the bus. Outputs the actual number of bytes read.
 * @return          PAS CO2 error code
 * @retval          OK if success
 * @retval          INTF_ERROR if error
 */
Error_t I2CPALIno::read(const uint8_t slave7BAddr, const uint8_t memAddr, uint8_t * data, uint16_t & length)
{
    Error_t err = pasco2::OK;
    
    PAS_CO2_I2C_PALINO_LOG_MSG(__FUNCTION__);
    do{

        if(maxRequestFromBytes < length)
        {   
            err = INTF_ERROR;
            break;
        }

        PAS_CO2_I2C_PALINO_WRITE_HEX(&slave7BAddr, 1);
        PAS_CO2_I2C_PALINO_WRITE_HEX(&memAddr, 1);

        wire->beginTransmission(slave7BAddr);

        uint8_t written = wire->write(memAddr);
        if(written != 1)
        {
            PAS_CO2_I2C_PALINO_LOG_MSG("error write mem Address");
            err = INTF_ERROR;
            break;
        }

        if(0 != wire->endTransmission(false))
        {
            PAS_CO2_I2C_PALINO_LOG_MSG("end transmission error");
            err = INTF_ERROR; 
            break;
        }

        //uint8_t bytesRead = wire->requestFrom(slave7BAddr, length, memAddr, 1, true);
        uint8_t bytesRead = wire->requestFrom(slave7BAddr, (uint8_t)length);

        if(bytesRead < length)
        {
            PAS_CO2_I2C_PALINO_LOG_MSG("error bytesRead < length");
            err = INTF_ERROR;
            break;
        } 

        for(uint16_t i = 0; (i < length) && (wire->available() > 0) ; i++)
        {
            data[i] = wire->read();
        }

        if(0 != wire->endTransmission(true))
        {
            PAS_CO2_I2C_PALINO_LOG_MSG("end 2 transmission error");
            err = INTF_ERROR; 
            break;
        }

        length = bytesRead;
        
        PAS_CO2_I2C_PALINO_READ_HEX(data, length);

    }while(0);

    PAS_CO2_I2C_PALINO_LOG_RETURN(err);
    
    return err;
}

/**
 * @brief           Writes in the I2C Arduino PAL the device memory registers
 * 
 *  The memory addresses length is 8 bits, which point to 8 bits word memory registers.
 *  In bulk write mode (length > 1 byte), the register address passed by argument will be the 
 *  start write address, and the subsequent registers will be written up until the requested length 
 *  is satisfied. 
 * 
 * @param[in]       slave7BAddr Slave address 7 bits format. Without write/read bit (8 bit address right shifted 1 bit)
 * @param[in]       memAddress  Device memory register start write address
 * @param[in]       *data       Pointer to the data array to be written  
 * @param[in,out]   length      Number of bytes to be written in the bus. Outputs the actual number of bytes written.
 * @return          PAS CO2 error code
 * @retval          OK if success
 * @retval          INTF_ERROR if  error
 */
Error_t I2CPALIno::write(const uint8_t slave7BAddr, const uint8_t memAddr, const uint8_t * data, uint16_t & length)
{
    Error_t err = pasco2::OK;

    PAS_CO2_I2C_PALINO_LOG_MSG(__FUNCTION__);
    do
    {
        PAS_CO2_I2C_PALINO_WRITE_HEX(&slave7BAddr, 1);
        PAS_CO2_I2C_PALINO_WRITE_HEX(&memAddr, 1);
        
        wire->beginTransmission(slave7BAddr);
        
        uint8_t written = wire->write(memAddr);

        if(written != 1)
        {
             PAS_CO2_I2C_PALINO_LOG_MSG("error write mem Address");
             err = INTF_ERROR;
             break;
        }

        written = wire->write(data, length);

        if(written != length)
        {
            err = INTF_ERROR;
            break;
        }

        if(0 != wire->endTransmission(true))
        {
            PAS_CO2_I2C_PALINO_LOG_MSG("end transmission error");
            err = INTF_ERROR;
        }

        length = written;

        PAS_CO2_I2C_PALINO_WRITE_HEX(data, length);
    
    }while(0);

    PAS_CO2_I2C_PALINO_LOG_RETURN(err);

    return err;
}

/**
 * @brief Library I2C instance
 */

I2CPALIno i2cpalino;
namespace pasco2
{
I2C i2c(static_cast<I2CPAL *>(&i2cpalino));
}

#endif /** PAS_CO2_INTF **/