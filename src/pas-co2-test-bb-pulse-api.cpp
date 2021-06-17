/** 
 * @file        pas-co2-test-bb-serial-api.hpp
 * @brief       PAS CO2 Black Box Serial API Tests  
 * @date        September 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-test-bb.hpp"
#include "pas-co2-logger.hpp"

#if IS_INTF(PAS_CO2_INTF_PULSE)

volatile static bool intFlag = false;
static void isr(void *)
{
    intFlag = true;
    PAS_CO2_APP_LOG_MSG("isr ocurred");
}

void test_pulseAPI(PASCO2Pulse & cotwo, Timer & timer)
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
        /* Periodic Polling*/
        PAS_CO2_APP_LOG_MSG("starting periodic measuring... ");
        err = cotwo.startMeasure();
        PAS_CO2_APP_LOG_RETURN(err);
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_MSG("periodic measuring triggered");

        PAS_CO2_APP_LOG_MSG("reading co2 values... ");
        int16_t  co2ppm = 0;
        uint32_t etime;
        for (uint8_t i = 0; i < 3; i++)
        {
            timer.start();
            while(0 >= co2ppm)
            {
                /* Wait for 7 seconds */
                timer.delay(7000);
                /* Attempt value read if measurement has finished */
                err = cotwo.getCO2(co2ppm);
                /* Get CO2 timeout of 22 seconds*/
                timer.elapsed(etime);
                if(etime > 22000)
                    break;
            }
            timer.stop();
            ASSERT_ERROR_RET(err);
            PAS_CO2_APP_LOG_VAR("co2 ppm value -> %i",co2ppm);
        }
        PAS_CO2_APP_LOG_MSG("co2 value read");

        PAS_CO2_APP_LOG_MSG("stopping periodic measuring...");
        err = cotwo.stopMeasure();
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_MSG("periodic measuring stopped");

        /* Reset */
        PAS_CO2_APP_LOG_MSG("reseting...");
        err = cotwo.reset();
        ASSERT_ERROR_RET(err);
        PAS_CO2_APP_LOG_MSG("reset completed");
        
        /* Periodic Interrupt */
        PAS_CO2_APP_LOG_MSG("start periodic meas interrupt...");
        err = cotwo.startMeasure(isr);
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

#endif /* PAS_CO2_PULSE_ENABLED */