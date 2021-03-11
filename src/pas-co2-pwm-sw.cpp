/** 
 * @file        pas-co2-pwm-sw.cpp
 * @brief       PAS CO2 PWM Input Software
 * @date        July 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "pas-co2-pwm-sw.hpp"

#if IS_INTF(PAS_CO2_INTF_PWM_SW)

#include "pas-co2-logger.hpp"

using namespace pasco2;

/**
 * @brief      PAS CO2 Software PWM Constructor
 * 
 * @param[in]   *pwmIn          Input GPIO instance reading the PWM sensor signal
 * @param[in]   *timer          Timer instance
 * @pre         None
 */
PWMSW::PWMSW(GPIO  * const pwmIn, 
             Timer * const timer,
             Mode_t        mode)
: pwmIn(pwmIn), timer(timer), mode(mode)
{

}

/**
 * @brief       PAS CO2 Software PWM Destructor
 * @pre         None
 */                                
PWMSW::~PWMSW()
{

}

/**
 * @brief       Initializes the PWM
 * @return      PAS CO2 error code
 * @retval      OK if success
 * @retval      INTF_ERROR if error
 */
Error_t PWMSW::init()
{
    Error_t err = OK;

    PAS_CO2_PWM_LOG_MSG(__FUNCTION__);
    do
    {
        err = pwmIn->init();
        if (OK != err)
            break;

        err = timer->init();
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_PWM_LOG_RETURN(err);

    return err;
}

/**
     * @brief       Deinitializes the PWM
     * @return      PAS CO2 error code
     * @retval      OK if success
     * @retval      INTF_ERROR if error
     */
Error_t PWMSW::deinit()
{
    Error_t err = OK;

    PAS_CO2_PWM_LOG_MSG(__FUNCTION__);
    do
    {
        err = pwmIn->deinit();
        if (OK != err)
            break;

        err = timer->deinit();
        if (OK != err)
            break;

    } while (0);
    PAS_CO2_PWM_LOG_RETURN(err);

    return err;
}

/**
     * @brief       Gets the PWM input duty cycle in percentage
     * 
     * @details     The duty will return 0.00 if no duty is read (e.g.
     *              polling mode). In case of error, the duty value will be
     *              -1.00, and the function will return the corresponding
     *              return code for diagnosis. 
     *
     * @note        The read out of PWM pulses needs to be synched with the
     *              incoming PWM signals. This synchronization needs to be
     *              handled in the PAL.
     *
     * @param[out]  duty PWM input duty cycle value from PWM
     *                   single or train pulse average (00.00 % to 
     *                   100.00 %)
     * @return      PAS CO2 error code
     * @retval      OK if success
     * @retval      INTF_ERROR if error
     */
Error_t PWMSW::getDuty(double & duty)
{
    Error_t  err                    = OK;
    uint32_t rt,                                /* Rising edge timestamp */
             ct,                                /* Current time */
             t0,                                /* Timeout start timestamp */
             t1,                                /* Pulse start timestamp */
             t2,                                /* Pulse falling edge timestamp */
             t3,                                /* Pulse end timestamp */
             tout = 0;                          /* Timeout timer */
    bool     pulseStart             = false;    /* Pulse start flag */
    uint8_t  pulseCnt               = 0;        /* Pulse counter */
    double   duties[pulseTrainLen]  = {0};      /* Duty cycles values */

    PAS_CO2_PWM_LOG_MSG(__FUNCTION__);
    do
    {
        duty = -1;

        if(MODE_INTERRUPT == mode)
        {
            err = pwmIn->enableInt((cback_t)isrRegister(this));
            if (OK != err)
                break;
        }

        err = timer->start();
        if (OK != err)
            break;

        err = timer->elapsedMicro(t0);
        if (OK != err)
            break;

        tout = 0;
        risingEdgeEvent  = false;
        fallingEdgeEvent = false;

        while( (pulseCnt < pulseTrainLen) && (tout < 3000000) )
        {
            if(MODE_POLLING == mode)
            {
                poll();
            }

            if(risingEdgeEvent)
            {
                /* Clear rise event flag */
                risingEdgeEvent = false;

                /* Rising event timestamp */
                err = timer->elapsedMicro(rt);
                if (OK != err)
                    break;

                /* If the pulse has already started
                   the rising edge is the end of the period */
                if(pulseStart)
                {
                    /* Clear pulse start flag */
                    pulseStart = false;

                    t3 = rt;   
                    /* Calculate duty for this pulse */
                    duties[pulseCnt++]  = ((double)(t2 - t1))/((double)(t3 - t1));
                    /* Resets the timeout timer reference */
                    t0 = rt;
                }
                /* If the pulse has not started the rising edge
                   is the start of the period */
                else
                {
                    t1 = rt; 
                    pulseStart = true;
                }
            }

            if(fallingEdgeEvent)
            {
                /* Clear fall event flag */
                fallingEdgeEvent = false;

                /* Falling event timestamp */
                err = timer->elapsedMicro(t2);
                if (OK != err)
                    break;
            }

            /* Update the timeout timer */
            err = timer->elapsedMicro(ct);
            if (OK != err)
                break;

            tout = ct - t0;
        }

        if(OK != err)
        {
            break;
        }
        else if(pulseCnt > 0)
        {
            /* Calculate average value */
            double dutyAux = 0;

            for(uint8_t i = 0; i < pulseCnt; i++)
            {
                dutyAux += (double)(100*duties[i]);
            }

            duty = (double)((double)dutyAux/(double)pulseCnt);
        }
        else 
        {
            duty = 0;
        }

        err = timer->stop();
        if(OK != err)
        {
            duty = -1;
        }

    } while (0);
    PAS_CO2_PWM_LOG_RETURN(err);

    return err;
}

void PWMSW::callback()
{
    GPIO::IntEvent_t event = pwmIn->intEvent();

    if(GPIO::IntEvent_t::INT_FALLING_EDGE == event)
    {
        fallingEdgeEvent = true;
    }
    else if(GPIO::IntEvent_t::INT_RISING_EDGE == event)
    {
        risingEdgeEvent = true;
    }
}

void PWMSW::poll()
{
    static GPIO::VLevel_t level = GPIO::VLevel_t::GPIO_LOW;

    GPIO::VLevel_t newLevel = pwmIn->read();

    if(level != newLevel)
    {
        if(GPIO::VLevel_t::GPIO_LOW == level)
        {
            risingEdgeEvent = true;
        }
        else if(GPIO::VLevel_t::GPIO_HIGH == level)
        {
            fallingEdgeEvent = true;
        }
        
        level = newLevel;
    }   
}

PWMSW      * PWMSW::objPtrVector[maxGPIOObjs] = { nullptr };
uint8_t      PWMSW::idxNext    = 0;

/**
 * @brief   Interrupt 0 Handler
 */
void PWMSW::int0Handler() 
{
    objPtrVector[0]->callback();
}

/**
 * @brief   Interrupt 1 Handler
 */
void PWMSW::int1Handler()
{
    objPtrVector[1]->callback();
}

/**
 * @brief   Interrupt 2 Handler
 */
void PWMSW::int2Handler()
{
    objPtrVector[2]->callback();
}

/**
 * @brief   Interrupt 3 Handler
 */
void PWMSW::int3Handler()
{
    objPtrVector[3]->callback();
}


void * PWMSW::fnPtrVector[maxGPIOObjs] = {(void *)int0Handler,
                                          (void *)int1Handler,
                                          (void *)int2Handler,
                                          (void *)int3Handler};

/**
 * @brief       Register a hardware interrupt for the PWMSW object passed by
 *              argument
 * @param[in]   *objPtr PWMSW object pointer
 * @return      Pointer to allocate the interrupt function handler 
 */
void * PWMSW::isrRegister(PWMSW *objPtr) 
{
    void * fPtr = nullptr;

    if(idxNext < 4)
    {
        objPtrVector[idxNext] = objPtr;
        fPtr =  fnPtrVector[idxNext++];
    }

    return fPtr;
}


#endif /** PAS_CO2_INTF **/

