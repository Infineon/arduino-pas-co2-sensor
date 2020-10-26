/** 
 * @file        pas-co2-serial.hpp
 * @brief       PAS CO2 Serial API 
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef PAS_CO2_SERIAL_HPP_
#define PAS_CO2_SERIAL_HPP_

#include "pas-co2-conf.hpp"

#if IS_INTF(PAS_CO2_INTF_SERIAL)

#include "pas-co2-types.hpp"
#include "pas-co2.hpp"

using namespace pasco2;

/**
 * @addtogroup co2sapi
 * @{
 */

class PASCO2Serial: private PASCO2
{
    public:

        PASCO2Serial(pasco2::SBus  * const sbus,
                     pasco2::Timer * const timer       = nullptr,
                     pasco2::GPIO  * const interrupt   = nullptr, 
                     pasco2::GPIO  * const protoSelect = nullptr,
                     pasco2::GPIO  * const power3V3    = nullptr,
                     pasco2::GPIO  * const power12V    = nullptr);

        virtual ~PASCO2Serial();

        Error_t enable      ();
        Error_t disable     ();

        Error_t startMeasure(int16_t periodInSec = 0, int16_t alarmTh = 0, void (*cback) (void *) = nullptr);
        Error_t stopMeasure ();

        Error_t getCO2      (int16_t & CO2PPM);

        Error_t getDiagnosis(Diag_t & diagnosis);

        Error_t calibrate   (ABOC_t aboc, int16_t abocRef, uint16_t pressRef);

        Error_t reset       ();

        Error_t getDeviceID (uint8_t & prodID, uint8_t & revID);

};

/** @} */

#endif /** PAS_CO2_INTF **/
#endif /** PAS_CO2_HPP_ **/