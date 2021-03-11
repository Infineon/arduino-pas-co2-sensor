/** 
 * @file        pas-co2-uart.cpp
 * @brief       PAS CO2 UART Protocol
 * @date        August 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-uart.hpp"

#if IS_INTF(PAS_CO2_INTF_UART)

#include "pas-co2-logger.hpp"

using namespace pasco2;

/**
 * @brief         UART interface constructor
 * @param[in]     *uartpal      UART pal interface
 * @param[in]     baudrateBps   Baudrate in bit per second. Default 19200 bps. 
 *                              Valid Range [9600, 19200].
 * @pre           None       
 */
UART::UART(UARTPAL  * const uartpal,
           uint32_t         baudrateBps) 
: uartpal(uartpal),
  baudrateBps(baudrateBps)
{

}

/**
 * @brief         UUART interface desconstructor
 * @pre           None       
 */
UART::~UART(void)
{

}

/**
 * @brief         Initializes the I2C interface   
 * 
 * @details       Initiliazes the host I2C peripheral and set its clock frequency
 * 
 * @return        PAS CO2 error code
 * @retval        OK if success
 * @retval        INTF_ERROR if hardware interface error
 * @pre           None
 */
Error_t UART::init(void)
{
      Error_t err = OK;
      
      PAS_CO2_UART_LOG_MSG(__FUNCTION__);
      do
      {
            err = uartpal->init();
            if (OK != err)
                  break;

            err = uartpal->config(baudrateBps, dataBits, parity, stopBits);
            if (OK != err)
                  break;

      } while (0);
      PAS_CO2_UART_LOG_RETURN(err);

      return err;
}

Error_t UART::deinit(void)
{
      Error_t err = OK;
      
      PAS_CO2_UART_LOG_MSG(__FUNCTION__);

      err = uartpal->deinit();

      PAS_CO2_UART_LOG_RETURN(err);

      return err;
}

/**
 * @brief       Writes data to a register or set of consecutive registers
 * 
 * @param[in]   regAddr     Register start write address
 * @param[in]   *data       Pointer to the data array to be written  
 * @param[in]   length      Number of registers to be written in the bus
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if error
 * @pre         init()
 */
Error_t UART::write(const uint8_t   memAddr,
                    const uint8_t * data,
                          uint16_t  length)
{
      Error_t         err                           = OK; 
      const uint8_t   requestWFrameLength           = 8;
      const uint8_t   ackFrameLength                = 2;    
      uint8_t         reqFrame[requestWFrameLength] = {0};
      uint8_t         replyFrame[ackFrameLength]    = {0};
      uint16_t        wLen                          = (uint16_t)requestWFrameLength;
      uint16_t        aLen                          = (uint16_t)ackFrameLength;
      uint32_t        timeoutMs                     = 200;

      PAS_CO2_UART_LOG_MSG(__FUNCTION__);

      for(uint8_t i = 0; i < length ; i++)
      {
            assembleWriteRequestFrame((memAddr + i), data[i], (char*)reqFrame);

            PAS_CO2_UART_LOG_WRITE_HEX(reqFrame, requestWFrameLength);

            err = uartpal->write(reqFrame, wLen);
            if (OK != err)
                  break;

            err = uartpal->waitReceive(aLen, timeoutMs);
            if (OK != err)
                  break;

            err = uartpal->read(replyFrame, aLen);
            if (OK != err)
                  break;

      	PAS_CO2_UART_LOG_READ_HEX(replyFrame, ackFrameLength);

            if(false == isAckFrame((char*)replyFrame, aLen))
            {
                  err = INTF_ERROR; 
                  break;
            }    
      }

      PAS_CO2_UART_LOG_RETURN(err);

      return err;
}

/**
 * @brief       Reads data from a register or set of consecutive registers
 * 
 * @param[in]   regAddr     Register start read address
 * @param[out]  *data       Pointer to the array that will store the read data   
 * @param[in]   length      Number of registers to be read from the bus
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if error
 * @pre         init()
 */
Error_t UART::read(const uint8_t   memAddr,
                         uint8_t * data,
                         uint16_t  length)
{
      Error_t        err                           = OK;       
      const uint8_t  reqRFrameLength               = 5;
      const uint8_t  replyRFrameLength             = 3;
      uint8_t        reqFrame[reqRFrameLength]     = {0};
      uint8_t        replyFrame[replyRFrameLength] = {0};
      uint16_t       wLen                          = (uint16_t)reqRFrameLength;
      uint16_t       aLen                          = (uint16_t)replyRFrameLength;
      uint32_t       timeoutMs                     = 200;

      PAS_CO2_UART_LOG_MSG(__FUNCTION__);

      for(uint8_t i = 0; i < length ; i++)
      {
            assembleReadRequestFrame((memAddr + i), (char*)reqFrame);

            PAS_CO2_UART_LOG_WRITE_HEX(reqFrame, reqRFrameLength);

            err = uartpal->write(reqFrame, wLen);
            if (OK != err)
                  break;

            err = uartpal->waitReceive(replyRFrameLength, timeoutMs);
            if (OK != err)
                  break;

            err = uartpal->read(replyFrame, aLen);
            if (OK != err)
                  break;

      	PAS_CO2_UART_LOG_READ_HEX(replyFrame, replyRFrameLength);

            if(true == isNackFrame((char*)replyFrame, aLen))
            {
                  err = INTF_ERROR; 
                  break;
            }    

            data[i] = disassembleReadReplyFrame((char*)replyFrame);
      }

      PAS_CO2_UART_LOG_RETURN(err);

      return err;
}

/**
 * @brief       Converts an unsigned 4 bits integer to its hexadecimal ascii value
 * 
 * @param[in]   uint4   Unsigned 4 bits integer 
 * @return      hexadecimal ascii value (char)
 */
char UART::uint4ToHexChar(uint8_t uint4)
{
      return ((uint4 <= 9) ? '0' + uint4 : ('a' + uint4  - 10));
}

/**
 * @brief       Assembles the register write request frame
 * @details     The UART write request frames have the following structure:
 * 
 *              "w," + <ascii hex 2 char register address> + "," +
 *                <ascii hex 2 char register value> "\n"
 * 
 *              For example, in order to write 0xE2 in the register 0x0A, the corresponding
 *              char string will be: "w,0a,e2\n". 
 *              Also uppercase ascii letters are allowed
 * 
 * @param[in]   regAddr Register address
 * @param[in]   regVal  Register value to be written
 * @param[out]  *frame  Pointer to store the assembled write frame 
 * @return      void
 */
void UART::assembleWriteRequestFrame(const uint8_t   regAddr,
                                     const uint8_t   regVal,
                                           char    * frame)
{
      frame[0] = 'w';
      frame[1] = ',';
      frame[2] = uint4ToHexChar((regAddr & 0xF0) >> 4);
      frame[3] = uint4ToHexChar((regAddr & 0x0F)); 
      frame[4] = ',';
      frame[5] = uint4ToHexChar((regVal & 0xF0) >> 4); 
      frame[6] = uint4ToHexChar((regVal & 0x0F)); 
      frame[7] = '\n';
      frame[8] = '\0';
}

/**
 * @brief       Assembles the register read request frame
 * @details     The UART read request frames have the following structure:
 * 
 *              "w," + <ascii hex 2 char register address> + "\n"
 * 
 *              For example, in order to read the register 0x0E, the corresponding
 *              char string will be: "r,0e\n". 
 *              Also uppercase ascii letters are allowed
 * 
 * @param[in]   regAddr Register address to be read
 * @param[out]  *frame  Pointer to store the assembled read frame 
 * @return      void
 */
void UART::assembleReadRequestFrame(const uint8_t   regAddr,
                                          char    * frame)
{
      frame[0] = 'r';
      frame[1] = ',';
      frame[2] = uint4ToHexChar((regAddr & 0xF0) >> 4);
      frame[3] = uint4ToHexChar((regAddr & 0x0F)); 
      frame[4] = '\n';
      frame[5] = '\0';
}

/**
 * @brief       Disassembles the read response frame
 * @details     The UART read response frames have the following structure:
 * 
 *              <ascii hex 2 char register value> + "\n"
 * 
 *              For example, a register with value 0xAB will reply the 
 *              frame: "ab\n". 
 *              Also uppercase ascii letters are allowed
 * 
 * @param[in]   *frame  Pointer to read response frame
 * @return      register value as unsigned 8 bits integer (uint8_t)
 */
uint8_t UART::disassembleReadReplyFrame(const char * frame)
{
      uint8_t uint4bits = 0;
      uint8_t regVal    = 0;
      int8_t  k         = 1;

      for(char * c = (char*)frame; k >= 0; c++)
      {   
            uint4bits = 0;

            if (*c <= '9')
            {
                  uint4bits = (*c - '0'); 
            }
            else if(*c >= 'A' && *c <= 'F')
            {
                  uint4bits = (*c - 'A' + 10); 
            }
            else if(*c >= 'a' && *c <= 'f')
            {
                  uint4bits = (*c - 'a' + 10); 
            }

            regVal |= (uint4bits & 0x0F) << (4 * k--);
      }

      return regVal;
}

/**
 * @brief       Checks if a frame is an ack reply frame
 * @details     The UART ack response frame is formed by the ascii ack 
 *              value (0x06) followed by a new line character ('\n' or 
 *              0xA). As hexadecimal, the array {0x06, 0xA}
 * 
 * @param[in]   *frame  Pointer to reply frame
 * @param[in]   len     Frame length in number of bytes
 * @return      bool
 * @retval      TRUE if the frame is an ack frame
 * @retval      FALSE if the frame is NOT an ack frame
 */
bool UART::isAckFrame(const char    * frame,
                      const uint8_t   len)
{
      uint8_t ackFrameLength  = 2;
      bool    rslt            = false;

      if(len  == ackFrameLength &&  \
         0x06 == frame[0]       &&  \
         '\n' == frame[1]
        )
      {
            rslt = true;
      }

      return rslt;
}

/**
 * @brief       Checks if a frame is an nack reply frame
 * @details     The UART nack response frame is formed by the ascii nack 
 *              value (0x15) followed by a new line character ('\n' or 
 *              0xA). As hexadecimal, the array {0x15, 0xA}
 * 
 * @param[in]   *frame  Pointer to reply frame
 * @param[in]   len     Frame length in number of bytes
 * @return      bool
 * @retval      TRUE if the frame is an ack frame
 * @retval      FALSE if the frame is NOT an ack frame
 */
bool UART::isNackFrame(const  char    * frame,
                       const  uint8_t   len)
{
      uint8_t nackFrameLength = 2;
      bool    rslt            = false;

      if(len  == nackFrameLength &&  \
         0x15 == frame[0]        &&  \
         '\n' == frame[1]
        )
      {
            rslt = true;
      }

      return rslt;
}
                                      

#endif /** PAS_CO2_INTF **/