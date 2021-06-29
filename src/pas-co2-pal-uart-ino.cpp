/** 
 * @file        pas-co2-pal-uart-ino.cpp
 * @brief       PAS CO2 UART Arduino PAL
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_UART)                  

#include "pas-co2-pal-uart-ino.hpp"

/**
 * @brief       UART Arduino PAL constructor
 * @param[in]   *serial Serial UART instance
 */
UARTPALIno::UARTPALIno(HardwareSerial * serial)
: serial(serial)
{

}

/**
 * @brief   UART Arduino PAL destructor
 */
UARTPALIno::~UARTPALIno()
{

}

/**
 * @brief       Initializes the UART Arduino PAL
 * @details     This will initialize the serial instance with 
 *              default configuration 115200 bps and 8-N-1.
 *              Use config() to change the UART configuration.
 * @return      PAS CO2 error code
 * @retval      OK if success (always)
 */
Error_t UARTPALIno::init()
{
    #if !defined(ARDUINO_ARCH_SAM)
    serial->begin(115200, SERIAL_8N1);
    #else 
    serial->begin(115200);
    #endif

    return pasco2::OK;
}

/**
 * @brief       Deinitializes the UART Arduino PAL
 * @return      PAS CO2 error code
 * @retval      OK if success (always)
 */
Error_t UARTPALIno::deinit()
{
    serial->end();

    return pasco2::OK;
}

/**
 * @brief       Configures the UART Arduino PAL settings
 * @param[in]   baudrate    Baudrate in bits per second
 * @param[in]   dataBits    Number of data bits transmitted between start and
 *                          stop conditions
 * @param[in]   parity      Parity bit configuration
 * @param[in]   stopBits    Number of stop bits
 * @return      PAS CO2 error code
 * @retval      OK if success 
 * @retval      INTF_ERROR if config parameter not supported
 */
Error_t UARTPALIno::config(const uint32_t   baudrate, 
                           const DataBits_t dataBits, 
                           const Parity_t   parity, 
                           const StopBits_t stopBits)
{
    Error_t   err = pasco2::OK;

    #if   defined(ARDUINO_ARCH_AVR)
    uint8_t conf;
    #elif defined(ARDUINO_ARM_XMC)
    XMC_UART_MODE_t conf;
    #elif defined(ARDUINO_ARCH_ESP32)
    uint32_t conf;
    #elif defined(ARDUINO_ARCH_SAM)
    #warning "arduino sam core only allows default 8-N-1 configuration"
    uint32_t conf;
    #else
    uint32_t conf;
    #endif

    conf = SERIAL_8N1;

    switch(dataBits)
    {
        case UART_5BITS:
        {
            switch(parity)
            {
                case UART_PARITY_NONE:
                    if(stopBits == UART_1STOPBIT)
                    {
                        conf = SERIAL_5N1;
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_5N2;
                    }
                    else
                    {
                        err = INTF_ERROR;
                    }
                break;

                case UART_PARITY_ODD:
                    if(stopBits == UART_1STOPBIT)
                    {
                       conf = SERIAL_5O1; 
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_5O2;                       
                    }
                    else
                    {
                        err = INTF_ERROR;
                    }
                break;

                case UART_PARITY_EVEN:
                    if(stopBits == UART_1STOPBIT)
                    {
                        conf = SERIAL_5E1; 
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_5E2; 
                    }       
                    else
                    {
                        err = INTF_ERROR;
                    }         
                break;

                default:
                    err = INTF_ERROR;
                break;
            }
        }
        break;

        case UART_6BITS:
        {
            switch(parity)
            {
                case UART_PARITY_NONE:
                    if(stopBits == UART_1STOPBIT)
                    {
                        conf = SERIAL_6N1;
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_6N2;
                    }
                    else
                    {
                        err = INTF_ERROR;
                    }
                break;

                case UART_PARITY_ODD:
                    if(stopBits == UART_1STOPBIT)
                    {
                       conf = SERIAL_6O1; 
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_6O2;                       
                    }
                    else
                    {
                        err = INTF_ERROR;
                    }
                break;

                case UART_PARITY_EVEN:
                    if(stopBits == UART_1STOPBIT)
                    {
                        conf = SERIAL_6E1; 
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_6E2; 
                    }
                    else
                    {
                        err = INTF_ERROR;
                    }                
                break;

                default:
                    err = INTF_ERROR;
                break;
            }
        }
        break;

        case UART_7BITS:
        {
            switch(parity)
            {
                case UART_PARITY_NONE:
                    if(stopBits == UART_1STOPBIT)
                    {
                        conf = SERIAL_7N1;
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_7N2;
                    }
                    else
                    {
                        err = INTF_ERROR;
                    }
                break;

                case UART_PARITY_ODD:
                    if(stopBits == UART_1STOPBIT)
                    {
                       conf = SERIAL_7O1; 
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_7O2;                       
                    }
                    else
                    {
                        err = INTF_ERROR;
                    }
                break;

                case UART_PARITY_EVEN:
                    if(stopBits == UART_1STOPBIT)
                    {
                        conf = SERIAL_7E1; 
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_7E2; 
                    }  
                    else
                    {
                        err = INTF_ERROR;
                    }             
                break;

                default:
                    err = INTF_ERROR;
                break;
            }
        }     
        break;

        case UART_8BITS:
        {
            switch(parity)
            {
                case UART_PARITY_NONE:
                    if(stopBits == UART_1STOPBIT)
                    {
                        conf = SERIAL_8N1;
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_8N2;
                    }
                    else
                    {
                        err = INTF_ERROR;
                    }
                break;

                case UART_PARITY_ODD:
                    if(stopBits == UART_1STOPBIT)
                    {
                       conf = SERIAL_8O1; 
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_8O2;                       
                    }
                    else
                    {
                        err = INTF_ERROR;
                    }
                break;

                case UART_PARITY_EVEN:
                    if(stopBits == UART_1STOPBIT)
                    {
                        conf = SERIAL_8E1; 
                    }
                    else if(stopBits == UART_2STOPBITS)
                    {
                        conf = SERIAL_8E2; 
                    } 
                    else
                    {
                        err = INTF_ERROR;
                    }               
                break;

                default:
                    err = INTF_ERROR;
                break;
            }
        }       
        break;

        case UART_9BITS:
        {
            err = INTF_ERROR;
        }      
        break; 

        default:
            err = INTF_ERROR;
        break;
    }

    #if (defined(ARDUINO_ARCH_AVR)   || \
         defined(ARDUINO_ARCH_ESP32) || \
         defined(ARDUINO_ARM_XMC))
    serial->begin(baudrate, conf);
    #else
    serial->begin(baudrate);
    #endif

    return err;
}

/**
 * @brief           Writes in the UART Arduino PAL
 * @param[in]       *data       Pointer to the data array to be written
 * @param[in,out]   length      Data length in bytes. Outputs the actual number
 *                              of written bytes
 * @return          PAS CO2 error code
 * @retval          OK if success (always)
 */
Error_t UARTPALIno::write(const uint8_t  * data,      
                                uint16_t & length)
{
    int sent = serial->write(data, length);

    length = (uint16_t)sent;

    return pasco2::OK;
}

/**
 * @brief           Reads from the UART Arduino PAL
 * @param[in]       *data       Pointer to the array that will store the read 
 *                              data
 * @param[in,out]   length      Data length in bytes. Outputs the actual number 
 *                              of read bytes
 * @return          PAS CO2 error code
 * @retval          OK if success (always)
 */
Error_t UARTPALIno::read(uint8_t  * data,     
                         uint16_t & length)
{
    size_t read = serial->readBytes(data, length);

    length = (size_t) read;
    
    return pasco2::OK;
}

/**
 * @brief           Waits for a maximum timeout for the specified amount of
 *                  bytes to be received
 * @param[in]       bytesRcvd   Expected number of bytes to be received
 * @param[in]       timeoutMs   Maximum time to wait in milliseconds
 * @return          PAS CO2 error code
 * @retval          OK if success
 * @retval          INTF_ERROR if error or timeout expires
 */
Error_t UARTPALIno::waitReceive(uint16_t  bytesRcvd, 
                                uint32_t  timeoutMs)

{
    serial->setTimeout((long)timeoutMs);

    return pasco2::OK;
}

#endif /** PAS_CO2_INTF **/