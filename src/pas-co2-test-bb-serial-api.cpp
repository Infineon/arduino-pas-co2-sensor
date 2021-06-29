/** 
 * @file        pas-co2-test-bb-serial-api.hpp
 * @brief       PAS CO2 Black Box Serial API Tests  
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-test-bb.hpp"
#include "pas-co2-serial.hpp"
#include "pas-co2-logger.hpp"

#if IS_INTF(PAS_CO2_INTF_SERIAL)

volatile static bool intFlag = false;
static void isr(void *)
{
    intFlag = true;
    PAS_CO2_APP_LOG_MSG("isr ocurred");
}

void test_serialAPI(PASCO2Serial & cotwo, Timer & timer)
{

    Error_t err = OK;

    /* Initialize timer */
    timer.init();

    /* Enable sensor */
    PAS_CO2_APP_LOG_MSG("enabling sensor...");
    err = cotwo.enable();
    PAS_CO2_APP_LOG_RETURN(err);
    if(OK != err) { return; }
    PAS_CO2_APP_LOG_MSG("sensor enabled");

    do
    {
        /* Device info */
        PAS_CO2_APP_LOG_MSG("getting device info...");
        uint8_t prodID, revID;
        err = cotwo.getDeviceID(prodID, revID);
        PAS_CO2_APP_LOG_RETURN(err);
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_VAR("prod id -> %u ", prodID);
        PAS_CO2_APP_LOG_VAR("rev id  -> %u", revID);
        PAS_CO2_APP_LOG_MSG("device info retrieved");

        /* Single shot */
        PAS_CO2_APP_LOG_MSG("starting single shot measuring... ");
        err = cotwo.startMeasure();
        PAS_CO2_APP_LOG_RETURN(err);
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_MSG("measuring triggered");

        PAS_CO2_APP_LOG_MSG("reading co2 value... ");
        int16_t  co2ppm = 0;
        uint8_t iter = 0 ;
        while(0 >= co2ppm && iter < 5)
        {
            /* Wait for 2.5 seconds */
            timer.delay(2500);
            /* Attempt value read if measurement has finished */
            err = cotwo.getCO2(co2ppm);
            /* Get CO2 timeout of 10 seconds*/
            iter++;
        }
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_VAR("co2 ppm value -> %i",co2ppm);
        PAS_CO2_APP_LOG_MSG("co2 value read");

        /* Periodic Polling*/
        PAS_CO2_APP_LOG_MSG("starting periodic measuring... ");
        err = cotwo.startMeasure(6);
        PAS_CO2_APP_LOG_RETURN(err);
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_MSG("periodic measuring triggered");

        PAS_CO2_APP_LOG_MSG("reading co2 values... ");
        
        for (uint8_t i = 0; i < 3; i++)
        {
            co2ppm = 0;
            iter = 0;
            while(0 >= co2ppm && iter < 4)
            {
                /* Wait for 7 seconds */
                timer.delay(7000);
                /* Attempt value read if measurement has finished */
                err = cotwo.getCO2(co2ppm);
                /* Get CO2 timeout of 22 seconds*/
                iter++;
            }
            ASSERT_ERROR_RET(err);
            PAS_CO2_APP_LOG_VAR("co2 ppm value -> %i",co2ppm);
        }
        PAS_CO2_APP_LOG_MSG("co2 value read");

        PAS_CO2_APP_LOG_MSG("stopping periodic measuring...");
        err = cotwo.stopMeasure();
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_MSG("periodic measuring stopped");

        /* Diagnosis Sensor Status */
        PAS_CO2_APP_LOG_MSG("reading sensor status...");
        Diag_t diag;
        err = cotwo.getDiagnosis(diag);
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_VAR("sensor ready flag          -> %i", diag.sensorRdy);
        PAS_CO2_APP_LOG_VAR("pwm pin enabled flag       -> %i", diag.pwmPinEnabled);
        PAS_CO2_APP_LOG_VAR("temp out of range flag     -> %i", diag.outOfRangeTempError);
        PAS_CO2_APP_LOG_VAR("ir power out of range flag -> %i", diag.outOfRange12VError);
        PAS_CO2_APP_LOG_VAR("comm error flag            -> %i", diag.commError);
        PAS_CO2_APP_LOG_MSG("sensor status read");

        /* Calibrate */
        PAS_CO2_APP_LOG_MSG("calibrating...");
        err = cotwo.calibrate(ABOC_ENABLED_PERIODIC, 100, 0x300);
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_MSG("calibrated");

        /* Reset */
        PAS_CO2_APP_LOG_MSG("reseting...");
        err = cotwo.reset();
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_MSG("reset completed");
        
        /* Periodic with Alarm and Interrupt */
        timer.delay(2000);
        PAS_CO2_APP_LOG_MSG("start periodic meas interrupt...");
        err = cotwo.startMeasure(8, 0, isr);
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_MSG("Started periodic meas interrupt");

        PAS_CO2_APP_LOG_MSG("reading co2 values... ");
        
        for (uint8_t i = 0; i < 3; i++)
        {
            co2ppm = 0;
            uint32_t elapsed = 0;
            timer.start();

            /* Wait until interrupt occurs*/
            while(false == intFlag){};

            timer.elapsed(elapsed);
            PAS_CO2_APP_LOG_VAR("isr time (ms) -> %i",elapsed);
            timer.stop();

            intFlag = false;

            err = cotwo.getCO2(co2ppm);

            ASSERT_ERROR_RET(err);
            PAS_CO2_APP_LOG_VAR("co2 ppm value -> %i",co2ppm);
        }
        PAS_CO2_APP_LOG_MSG("co2 value read");

        PAS_CO2_APP_LOG_MSG("stopping periodic measuring...");
        err = cotwo.stopMeasure();
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_MSG("periodic measuring stopped");

    } while (0);

    /* Disable error */
    PAS_CO2_APP_LOG_MSG("disabling sensor... ");
    err = cotwo.disable();
    PAS_CO2_APP_LOG_RETURN(err);

    PAS_CO2_APP_LOG_MSG("sensor disabled");

    /* Deinitialize timer */
    timer.deinit();
}
#endif /* PAS_CO2_SERIAL_ENABLED */