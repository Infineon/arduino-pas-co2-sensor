/** 
 * @file        pas-co2-logger.hpp
 * @brief       PAS CO2 Logger
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_LOGGER_HPP_
#define PAS_CO2_LOGGER_HPP_

#include "pas-co2-conf.hpp"

#if (PAS_CO2_LOGGER_ENABLED == 1)

#include <stdint.h>
#include "pas-co2-pal-logger.hpp"

namespace pasco2
{

class Logger
{
    public:

                Logger              ();
                Logger              (LoggerPAL *logpal);
               ~Logger              ();
        void    init                ();
        void    deinit              ();
        void    print               (const char * format,
                                     ...);
        void    printfModule        (const char * format,
                                     const char * module,
                                     const char * color,
                                     ...);
        void    printModuleHex      (const uint8_t  * array,
                                           uint32_t   length,
                                     const char     * module,
                                     const char     * color);

    private:
        LoggerPAL * logpal;
};

/**
 * @brief Library logger
 */
extern Logger cotwo_log;

/** 
 * Logger colors options
 */
#define PAS_CO2_LOGGER_COLOR_RED                  "\x1b[31m"
#define PAS_CO2_LOGGER_COLOR_GREEN                "\x1b[32m"
#define PAS_CO2_LOGGER_COLOR_YELLOW               "\x1b[33m"
#define PAS_CO2_LOGGER_COLOR_BLUE                 "\x1b[34m"
#define PAS_CO2_LOGGER_COLOR_MAGENTA              "\x1b[35m"
#define PAS_CO2_LOGGER_COLOR_CYAN                 "\x1b[36m"
#define PAS_CO2_LOGGER_COLOR_LIGHT_GREY           "\x1b[90m"
#define PAS_CO2_LOGGER_COLOR_LIGHT_RED            "\x1b[91m"
#define PAS_CO2_LOGGER_COLOR_LIGHT_GREEN          "\x1b[92m"
#define PAS_CO2_LOGGER_COLOR_LIGHT_YELLOW         "\x1b[93m"
#define PAS_CO2_LOGGER_COLOR_LIGHT_BLUE           "\x1b[94m"
#define PAS_CO2_LOGGER_COLOR_LIGHT_MAGENTA        "\x1b[95m"
#define PAS_CO2_LOGGER_COLOR_LIGHT_CYAN           "\x1b[96m"
#define PAS_CO2_LOGGER_COLOR_DEFAULT              "\x1b[0m"

/**
 * @brief PAS CO2 logger module
 */ 

/**
 *  Logger color for different error types
 */
#define PAS_CO2_LOGGER_ERROR_COLOR               PAS_CO2_LOGGER_COLOR_RED
#define PAS_CO2_LOGGER_WARNING_COLOR             PAS_CO2_LOGGER_COLOR_YELLOW


#define PAS_CO2_LOG_INIT()\
{\
    cotwo_log.init();\
}

#define PAS_CO2_LOG_DEINIT()\
{\
    cotwo_log.deinit();\
}

/**
 * @brief PAS CO2 core logger module
 */ 
#if (PAS_CO2_CORE_LOGGER_ENABLED == 1)

#define PAS_CO2_LOGGER_SERVICE               "[pasco2]        : "
#define PAS_CO2_LOGGER_COLOR                 PAS_CO2_LOGGER_COLOR_GREEN

#define PAS_CO2_LOG_MSG(str)\
{\
    cotwo_log.printfModule(str, PAS_CO2_LOGGER_SERVICE, PAS_CO2_LOGGER_COLOR);\
}

#define PAS_CO2_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        cotwo_log.printfModule("fail with return code %i", PAS_CO2_LOGGER_SERVICE, PAS_CO2_LOGGER_ERROR_COLOR, ret);\
    }\
    else\
    {\
        cotwo_log.printfModule("pass", PAS_CO2_LOGGER_SERVICE, PAS_CO2_LOGGER_COLOR);\
    }\
}

#else

#define PAS_CO2_LOG_MSG(str)                 {   }  
#define PAS_CO2_LOG_RETURN(ret)              {   }

#endif /* PAS_CO2_CORE_LOGGER_ENABLED */

/**
 * @brief PAS CO2 serial logger module
 */ 
#if (PAS_CO2_SERIAL_LOGGER_ENABLED == 1)

/**
 *  Logger color for different error types
 */

#define PAS_CO2_SERIAL_LOGGER_SERVICE               "[pasco2 serial] : "
#define PAS_CO2_SERIAL_LOGGER_COLOR                 PAS_CO2_LOGGER_COLOR_BLUE

#define PAS_CO2_SERIAL_LOG_MSG(str)\
{\
    cotwo_log.printfModule(str, PAS_CO2_SERIAL_LOGGER_SERVICE, PAS_CO2_SERIAL_LOGGER_COLOR);\
}

#define PAS_CO2_SERIAL_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        cotwo_log.printfModule("fail with return code %i", PAS_CO2_SERIAL_LOGGER_SERVICE, PAS_CO2_SERIAL_LOGGER_COLOR, ret);\
    }\
    else\
    {\
        cotwo_log.printfModule("pass", PAS_CO2_SERIAL_LOGGER_SERVICE, PAS_CO2_SERIAL_LOGGER_COLOR);\
    }\
}

#else

#define PAS_CO2_SERIAL_LOG_MSG(str)                 {   }  
#define PAS_CO2_SERIAL_LOG_RETURN(ret)              {   }

#endif /* PAS_CO2_SERIAL_LOGGER_ENABLED */

/**
 * @brief PAS CO2 Pulse logger module
 */ 
#if (PAS_CO2_PULSE_LOGGER_ENABLED == 1)

/**
 *  Logger color for different error types
 */

#define PAS_CO2_PULSE_LOGGER_SERVICE               "[pasco2 pulse] : "
#define PAS_CO2_PULSE_LOGGER_COLOR                 PAS_CO2_LOGGER_COLOR_BLUE

#define PAS_CO2_PULSE_LOG_MSG(str)\
{\
    cotwo_log.printfModule(str, PAS_CO2_PULSE_LOGGER_SERVICE, PAS_CO2_PULSE_LOGGER_COLOR);\
}

#define PAS_CO2_PULSE_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        cotwo_log.printfModule("fail with return code %i", PAS_CO2_PULSE_LOGGER_SERVICE, PAS_CO2_PULSE_LOGGER_COLOR, ret);\
    }\
    else\
    {\
        cotwo_log.printfModule("pass", PAS_CO2_PULSE_LOGGER_SERVICE, PAS_CO2_PULSE_LOGGER_COLOR);\
    }\
}

#else

#define PAS_CO2_PULSE_LOG_MSG(str)                 {   }  
#define PAS_CO2_PULSE_LOG_RETURN(ret)              {   }

#endif /* PAS_CO2_PULSE_LOGGER_ENABLED */

/**
 * @brief PAS CO2 i2c logger module
 */ 
#if (PAS_CO2_I2C_LOGGER_ENABLED == 1)

#define PAS_CO2_I2C_LOGGER_SERVICE               "[pasco2 i2c]    : "
#define PAS_CO2_I2C_LOGGER_WRITE_SERVICE         "[pasco2 i2c]    : >>> "
#define PAS_CO2_I2C_LOGGER_READ_SERVICE          "[pasco2 i2c]    : <<< "
#define PAS_CO2_I2C_LOGGER_COLOR                 PAS_CO2_LOGGER_COLOR_CYAN

#define PAS_CO2_I2C_LOG_MSG(str)\
{\
    cotwo_log.printfModule(str, PAS_CO2_I2C_LOGGER_SERVICE, PAS_CO2_I2C_LOGGER_COLOR);\
}

#define PAS_CO2_I2C_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        cotwo_log.printfModule("fail with return code %i", PAS_CO2_I2C_LOGGER_SERVICE, PAS_CO2_LOGGER_ERROR_COLOR, ret);\
    }\
    else\
    {\
        cotwo_log.printfModule("pass", PAS_CO2_I2C_LOGGER_SERVICE, PAS_CO2_I2C_LOGGER_COLOR);\
    }\
}

#define PAS_CO2_I2C_LOG_READ_HEX(array, length)\
{\
    cotwo_log.printModuleHex(array, length, PAS_CO2_I2C_LOGGER_READ_SERVICE, PAS_CO2_I2C_LOGGER_COLOR);\
}

#define PAS_CO2_I2C_LOG_WRITE_HEX(array, length)\
{\
    cotwo_log.printModuleHex(array, length, PAS_CO2_I2C_LOGGER_WRITE_SERVICE, PAS_CO2_I2C_LOGGER_COLOR);\
} 

#else

#define PAS_CO2_I2C_LOG_MSG(str)                 {   }  
#define PAS_CO2_I2C_LOG_RETURN(ret)              {   }
#define PAS_CO2_I2C_LOG_READ_HEX(array, length)  {   }
#define PAS_CO2_I2C_LOG_WRITE_HEX(array, length) {   } 

#endif /* PAS_CO2_I2C_LOGGER_ENABLED */

/**
 * @brief PAS CO2 UART logger module
 */ 
#if (PAS_CO2_UART_LOGGER_ENABLED == 1)

#define PAS_CO2_UART_LOGGER_SERVICE               "[pasco2 uart]   : "
#define PAS_CO2_UART_LOGGER_WRITE_SERVICE         "[pasco2 uart]   : >>> "
#define PAS_CO2_UART_LOGGER_READ_SERVICE          "[pasco2 uart]   : <<< "
#define PAS_CO2_UART_LOGGER_COLOR                 PAS_CO2_LOGGER_COLOR_LIGHT_GREY

#define PAS_CO2_UART_LOG_MSG(str)\
{\
    cotwo_log.printfModule(str, PAS_CO2_UART_LOGGER_SERVICE, PAS_CO2_UART_LOGGER_COLOR);\
}

#define PAS_CO2_UART_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        cotwo_log.printfModule("fail with return code %i", PAS_CO2_UART_LOGGER_SERVICE, PAS_CO2_LOGGER_ERROR_COLOR, ret);\
    }\
    else\
    {\
        cotwo_log.printfModule("pass", PAS_CO2_UART_LOGGER_SERVICE, PAS_CO2_UART_LOGGER_COLOR);\
    }\
}

#define PAS_CO2_UART_LOG_READ_HEX(array, length)\
{\
    cotwo_log.printModuleHex(array, length, PAS_CO2_UART_LOGGER_READ_SERVICE, PAS_CO2_UART_LOGGER_COLOR);\
}

#define PAS_CO2_UART_LOG_WRITE_HEX(array, length)\
{\
    cotwo_log.printModuleHex(array, length, PAS_CO2_UART_LOGGER_WRITE_SERVICE, PAS_CO2_UART_LOGGER_COLOR);\
} 

#else

#define PAS_CO2_UART_LOG_MSG(str)                 {   }  
#define PAS_CO2_UART_LOG_RETURN(ret)              {   }
#define PAS_CO2_UART_LOG_READ_HEX(array, length)  {   }
#define PAS_CO2_UART_LOG_WRITE_HEX(array, length) {   }
#endif /* PAS_CO2_UART_LOGGER_ENABLED */


/**
 * @brief PAS CO2 PWM logger module
 */ 

#if (PAS_CO2_PWM_LOGGER_ENABLED == 1)

/**
 *  Logger color for different error types
 */

#define PAS_CO2_PWM_LOGGER_SERVICE               "[pasco2 pwm] : "
#define PAS_CO2_PWM_LOGGER_COLOR                 PAS_CO2_LOGGER_COLOR_BLUE

#define PAS_CO2_PWM_LOG_MSG(str)\
{\
    cotwo_log.printfModule(str, PAS_CO2_PWM_LOGGER_SERVICE, PAS_CO2_PULSE_LOGGER_COLOR);\
}

#define PAS_CO2_PWM_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        cotwo_log.printfModule("fail with return code %i", PAS_CO2_PWM_LOGGER_SERVICE, PAS_CO2_PULSE_LOGGER_COLOR, ret);\
    }\
    else\
    {\
        cotwo_log.printfModule("pass", PAS_CO2_PWM_LOGGER_SERVICE, PAS_CO2_PULSE_LOGGER_COLOR);\
    }\
}

#else

#define PAS_CO2_PWM_LOG_MSG(str)                 {   }  
#define PAS_CO2_PWM_LOG_RETURN(ret)              {   }

#endif /* PAS_CO2_PWM_INTF */

/**
 * @brief PAS CO2 Register module
 */ 
#if (PAS_CO2_REG_LOGGER_ENABLED == 1)

#define PAS_CO2_REG_LOGGER_SERVICE               "[pasco2 reg]    : " 
#define PAS_CO2_REG_LOGGER_COLOR                 PAS_CO2_LOGGER_COLOR_GREEN

#define PAS_CO2_REG_LOG_MSG(str)\
{\
    cotwo_log.printfModule(str, PAS_CO2_REG_LOGGER_SERVICE, PAS_CO2_REG_LOGGER_COLOR);\
}

#define PAS_CO2_REG_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        cotwo_log.printfModule("fail with return code %i", PAS_CO2_REG_LOGGER_SERVICE, PAS_CO2_LOGGER_ERROR_COLOR, ret);\
    }\
    else\
    {\
        cotwo_log.printfModule("pass", PAS_CO2_REG_LOGGER_SERVICE, PAS_CO2_REG_LOGGER_COLOR);\
    }\
}

#define PAS_CO2_REG_VALUE_HEX(addr_ptr)\
{\
    cotwo_log.printModuleHex(addr_ptr, 1, PAS_CO2_REG_LOGGER_SERVICE, PAS_CO2_REG_LOGGER_COLOR);\
}

#define PAS_CO2_REG_BITF_VALUE(map, addr, bitf)\
{\
    cotwo_log.printfModule("%x :: 0x%02x -> bfield 0x%02x", PAS_CO2_REG_LOGGER_SERVICE, PAS_CO2_REG_LOGGER_COLOR, addr, map[addr], bitf);\
}

#define PAS_CO2_REG_MAP_HEX(map, length, addr)\
{\
    cotwo_log.print("%s%s", PAS_CO2_REG_LOGGER_COLOR, PAS_CO2_REG_LOGGER_SERVICE);\
    cotwo_log.print("%x :: 0x%02x\r\n", 0, map[0]);\
    for(uint8_t i = 1; i < length; i++)\
    {\
        cotwo_log.print("%17x :: ", i);\
        cotwo_log.print("0x%02x", map[i]);\
        if(i == addr)\
            cotwo_log.print("<---");\
            	                    \
        cotwo_log.print("\r\n");\
    }\
    cotwo_log.print("%s", PAS_CO2_LOGGER_COLOR_DEFAULT);\
}

#else

#define PAS_CO2_REG_VALUE_HEX(addr_ptr)          {   } 
#define PAS_CO2_REG_BITF_VALUE(map, addr, bitf)  {   }
#define PAS_CO2_REG_MAP_HEX(map, length, addr)   {   }   


#endif /* PAS_CO2_REG_LOGGER_ENABLED */

/**
 * @brief PAS CO2 Application module
 */ 
#if (PAS_CO2_APP_LOGGER_ENABLED == 1)

/**
 *  Logger color for different error types
 */
#define PAS_CO2_APP_LOGGER_ERROR_COLOR               PAS_CO2_LOGGER_COLOR_RED
#define PAS_CO2_APP_LOGGER_WARNING_COLOR             PAS_CO2_LOGGER_COLOR_YELLOW


#define PAS_CO2_APP_LOGGER_SERVICE               "[pasco2 app]    : "
#define PAS_CO2_APP_LOGGER_COLOR                 PAS_CO2_LOGGER_COLOR_MAGENTA

#define PAS_CO2_APP_LOG_MSG(str)\
{\
    cotwo_log.printfModule(str, PAS_CO2_APP_LOGGER_SERVICE, PAS_CO2_APP_LOGGER_COLOR);\
}

#define PAS_CO2_APP_LOG_VAR(str, var)\
{\
    cotwo_log.printfModule(str, PAS_CO2_APP_LOGGER_SERVICE, PAS_CO2_APP_LOGGER_COLOR, var);\
}\

#define PAS_CO2_APP_LOG_RETURN(ret)\
{\
    if( 0 > ret)\
    {\
        cotwo_log.printfModule("fail with return code %i", PAS_CO2_APP_LOGGER_SERVICE, PAS_CO2_APP_LOGGER_ERROR_COLOR, ret);\
    }\
    else\
    {\
        cotwo_log.printfModule("pass", PAS_CO2_APP_LOGGER_SERVICE, PAS_CO2_APP_LOGGER_COLOR);\
    }\
}

#else

#define PAS_CO2_APP_LOG_MSG(str)        {}
#define PAS_CO2_APP_LOG_VAR(str, var)   {}
#define PAS_CO2_APP_LOG_RETURN(ret)     {}

#endif /* PAS_CO2_APP_LOGGER_ENABLED */

}

#else

/** 
 * Logger not enabled. All macros are empty.
 */

#define PAS_CO2_LOG_INIT()                       {   }
#define PAS_CO2_LOG_DEINIT()                     {   }

#define PAS_CO2_LOG_MSG(str)                     {   }
#define PAS_CO2_LOG_RETURN(ret)                  {   }

#define PAS_CO2_SERIAL_LOG_MSG(str)              {   }  
#define PAS_CO2_SERIAL_LOG_RETURN(ret)           {   }

#define PAS_CO2_PULSE_LOG_MSG(str)               {   }  
#define PAS_CO2_PULSE_LOG_RETURN(ret)            {   }

#define PAS_CO2_I2C_LOG_MSG(str)                 {   }
#define PAS_CO2_I2C_LOG_RETURN(ret)              {   }
#define PAS_CO2_I2C_LOG_READ_HEX(array, length)  {   }
#define PAS_CO2_I2C_LOG_WRITE_HEX(array, length) {   } 

#define PAS_CO2_UART_LOG_MSG(str)                {   }  
#define PAS_CO2_UART_LOG_RETURN(ret)             {   }
#define PAS_CO2_UART_LOG_READ_HEX(array, length) {   }
#define PAS_CO2_UART_LOG_WRITE_HEX(array, length){   }

#define PAS_CO2_PWM_LOG_MSG(str)                 {   }  
#define PAS_CO2_PWM_LOG_RETURN(ret)              {   }

#define PAS_CO2_REG_LOG_MSG(str)                 {   }
#define PAS_CO2_REG_LOG_RETURN(ret)              {   }
#define PAS_CO2_REG_VALUE_HEX(addr_ptr)          {   } 
#define PAS_CO2_REG_BITF_VALUE(map, addr, bitf)  {   }
#define PAS_CO2_REG_MAP_HEX(map, length, addr)   {   } 

#define PAS_CO2_APP_LOG_MSG(str)                 {   }
#define PAS_CO2_APP_LOG_VAR(str, var)            {   }
#define PAS_CO2_APP_LOG_RETURN(ret)              {   }

#endif /* PAS_CO2_LOGGER_ENABLED */

#endif /* PAS_CO2_LOGGER_HPP_ */