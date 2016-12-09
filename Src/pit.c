
#include "includes.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* pit source clock variable which will be updated in PIT_Init */
uint64_t g_pitSourceClock;

/* pit channel number used by microseconds functions */
uint8_t g_pitUsChannel;

pit_user_config_t * pitConfigPtr = NULL;
/*******************************************************************************
 * Code
 ******************************************************************************/

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_Init
 * Description   : Initialize PIT module.
 * This function must be called before calling all the other PIT driver functions.
 * This function un-gates the PIT clock and enables the PIT module. The
 * isRunInDebug passed into function will affect all timer channels.
 *
 *END**************************************************************************/
pit_status_t PIT_Init(bool isRunInDebug)
{

    /* Un-gate pit clock*/
	PIT_Hal_EnableClock(); 

    /* Enable PIT module clock*/
    PIT_Hal_Enable();

    /* Set timer run or stop in debug mode*/
    PIT_Hal_SetTimerRunInDebugCmd(isRunInDebug);

    /* Finally, update pit source clock frequency.*/
    g_pitSourceClock = 12000000;

    return status_PIT_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_InitChannel
 * Description   : Initialize PIT channel.
 * This function initialize PIT timers by channel. Pass in timer number and its
 * config structure. Timers do not start counting by default after calling this
 * function. Function PIT_StartTimer must be called to start timer counting.
 * Call PIT_SetTimerPeriodByUs to re-set the period.
 *
 *END**************************************************************************/
void PIT_InitChannel(uint32_t channel, const pit_user_config_t * config)
{
    /* Set timer period.*/
		pitConfigPtr = config;
    PIT_SetTimerPeriodByUs(channel, config->periodUs);

    /* Enable or disable interrupt.*/
    PIT_Hal_SetIntCmd(channel, config->isInterruptEnabled);

    /* Configure NVIC*/
    if (config->isInterruptEnabled)
    {
        /* Enable PIT interrupt.*/
        PIT_Hal_EnableNVICInterrupt();
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_Deinit
 * Description   : Disable PIT module and gate control
 * This function will disable all PIT interrupts and PIT clock. Then gate the
 * PIT clock control. pit_init must be called in order to use PIT again.
 *
 *END**************************************************************************/
pit_status_t PIT_Deinit(void)
{
    uint32_t i;

    /* Exit if current instance is gated.*/
    if (!PIT_Hal_isClockGateEnable())	
    {
        return status_PIT_Fail;
    }

    /* Disable all PIT interrupts. Clear the chain bit if available */
    for (i=0; i < PIT_TIMER_CNT; i++)
    {
        PIT_Hal_SetIntCmd(i, false);
        PIT_Hal_DisableNVICInterrupt();
        PIT_Hal_SetTimerChainCmd(i, false);
    }

    /* Disable PIT module clock*/
    PIT_Hal_Disable();

    /* Gate PIT clock control*/
    PIT_Hal_DisableClock();
    return status_PIT_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_StartTimer
 * Description   : Start timer counting.
 * After calling this function, timers load period value, count down to 0 and
 * then load the respective start value again. Each time a timer reaches 0,
 * it will generate a trigger pulse and set the timeout interrupt flag.
 *
 *END**************************************************************************/
void PIT_StartTimer(uint32_t channel)
{
    PIT_Hal_StartTimer(channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_StopTimer
 * Description   : Stop timer counting.
 * This function will stop every timer counting. Timers will reload their periods
 * respectively after calling PIT_StartTimer next time.
 *
 *END**************************************************************************/
void PIT_StopTimer(uint32_t channel)
{
    PIT_Hal_StopTimer(channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_SetTimerPeriodByUs
 * Description   : Set timer period in microseconds unit.
 * The period range depends on the frequency of PIT source clock. If required
 * period is out the range, try to use lifetime timer if applicable.
 * This function is only valid for one single channel. If channels are chained together,
 * the period here will make no sense.
 *
 *END**************************************************************************/
void PIT_SetTimerPeriodByUs(uint32_t channel, uint32_t us)
{
    /* Calculate the count value, assign it to timer counter register.*/
    uint32_t count = (uint32_t)(us * g_pitSourceClock / 1000000U - 1U);
    PIT_Hal_SetTimerPeriodByCount(channel, count);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_GetTimerPeriodByUs
 * Description   : Gets the timer period in microseconds for one single channel.
 *
 *END**************************************************************************/
uint32_t PIT_GetTimerPeriodByUs(uint32_t channel)
{
    /* Get current timer period by count.*/
    uint64_t currentPeriod = PIT_Hal_GetTimerPeriodByCount(channel);

    /* Convert count numbers to microseconds unit.*/
    currentPeriod = (currentPeriod + 1U) * 1000000U / g_pitSourceClock;
    return (uint32_t)currentPeriod ;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_ReadTimerUs
 * Description   : Read current timer value in microseconds unit.
 * This function will return an absolute time stamp in the unit of microseconds.
 * One common use of this function is to measure the running time of part of
 * code. Just call this function at both the beginning and end of code, the time
 * difference between these two time stamp will be the running time (Need to
 * make sure the running time will not exceed the timer period). Also, the time
 * stamp returned is up-counting.
 *
 *END**************************************************************************/
uint32_t PIT_ReadTimerUs(uint32_t channel)
{
    /* Get current timer count, and reverse it to up-counting.*/
    uint64_t currentTime = (~PIT_Hal_ReadTimerCount(channel));

    /* Convert count numbers to microseconds unit.*/
    currentTime = (currentTime * 1000000U) / g_pitSourceClock;
    return (uint32_t)currentTime;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_SetTimerPeriodByCount
 * Description   : Sets the timer period in units of count.
 * Timers begin counting from the value set by this function.
 * The counter period of a running timer can be modified by first stopping
 * the timer, setting a new load value, and  starting the timer again. If
 * timers are not restarted, the new value is loaded after the next trigger
 * event.
 *
 *END**************************************************************************/
void PIT_SetTimerPeriodByCount(uint32_t channel, uint32_t count)
{
    PIT_Hal_SetTimerPeriodByCount(channel, count);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_GetTimerPeriodByCount
 * Description   : Returns the current timer period in units of count.
 *
 *END**************************************************************************/
uint32_t PIT_GetTimerPeriodByCount(uint32_t channel)
{
    return PIT_Hal_GetTimerPeriodByCount(channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_ReadTimerCount
 * Description   : Reads the current timer counting value.
 * This function returns the real-time timer counting value, in a range from 0
 * to a timer period.
 *
 *END**************************************************************************/
uint32_t PIT_ReadTimerCount(uint32_t channel)
{
    return PIT_Hal_ReadTimerCount(channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_SetLifetimeTimerPeriodByUs
 * Description   : Set lifetime timer period (Timers must be chained).
 * Timer 1 must be chained with timer 0 before using lifetime timer. The period
 * range is restricted by "period * g_pitSourceClock < max of an uint64_t integer",
 * or it may cause a overflow and is not able to set correct period.
 *
 *END**************************************************************************/
void PIT_SetLifetimeTimerPeriodByUs(uint64_t us)
{
    uint64_t lifeTimeCount;

    /* Calculate the counter value.*/
    lifeTimeCount = us * g_pitSourceClock / 1000000U - 1U;

    /* Assign to timers.*/
    PIT_Hal_SetTimerPeriodByCount(0U, (uint32_t)lifeTimeCount);
    PIT_Hal_SetTimerPeriodByCount(1U, (uint32_t)(lifeTimeCount >> 32U));
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_ReadLifetimeTimerUs
 * Description   : Read current lifetime value in microseconds unit.
 * Return an absolute time stamp in the unit of microseconds. The time stamp
 * value will not exceed the timer period. Also, the timer is up-counting.
 *
 *END**************************************************************************/
uint64_t PIT_ReadLifetimeTimerUs(void)
{
    /* Get current lifetime timer count, and reverse it to up-counting.*/
    uint64_t currentTime = (~PIT_Hal_ReadLifetimeTimerCount());

    /* Convert count numbers to microseconds unit.*/
    /* Note: using currentTime * 1000 rather than 1000000 to avoid short time overflow. */
    return currentTime = (currentTime * 1000U) / (g_pitSourceClock / 1000U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_InitUs
 * Description   : Initializes two PIT channels to serve as a microseconds unit.
 * This function is in parallel with PIT_InitChannel, they will overwrite
 * each other. PIT_Init must be called before calling this function.
 * The microseconds unit will use two chained channels to simulate a lifetime
 * timer, the channel number passed in and the "channel -1" channel will be used.
 * Note:
 * 1. These two channels will be occupied and could not be used with other purposes.
 * 2. The channel number passed in must be greater than 0.

 *END**************************************************************************/
void PIT_InitUs(uint32_t channel)
{

    assert(channel > 0U);
    g_pitUsChannel = channel;

    PIT_Hal_SetTimerChainCmd(channel, true);
    PIT_Hal_SetTimerPeriodByCount(channel, 0xFFFFFFFFU);
    PIT_Hal_SetTimerPeriodByCount(channel - 1U, 0xFFFFFFFFU);

    PIT_Hal_StartTimer(channel);
    PIT_Hal_StartTimer(channel - 1U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_GetUs
 * Description   : Get an absolute time stamp.
 * This function is useful to get elapsed time through calling this function in
 * time A, and then call it in time B. The elapsed time could be get by B-A, the
 * result may have 3-5 microseconds error depends on system clock frequency.
 *
 *END**************************************************************************/
uint32_t PIT_GetUs(void)
{
    /* Get current timer count, and reverse it to up-counting.*/
    uint64_t currentTime = ~(((uint64_t)PIT_Hal_ReadTimerCount(g_pitUsChannel) << 32U) +
                           PIT_Hal_ReadTimerCount(g_pitUsChannel - 1U));

    /* Convert count numbers to microseconds unit.*/
    return currentTime = (currentTime * 1000U) / (g_pitSourceClock / 1000U);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_DelayUs
 * Description   : Delay specific microseconds.
 * The delay will have a 3-5 microseconds error depends on system clock frequency.
 *
 *END**************************************************************************/
void PIT_DelayUs(uint32_t us)
{
    uint64_t x = us * g_pitSourceClock / 1000000;
    uint64_t timeToBe = ((uint64_t)PIT_Hal_ReadTimerCount(g_pitUsChannel) << 32U) +
                           PIT_Hal_ReadTimerCount(g_pitUsChannel - 1U) - x;

    while (((uint64_t)PIT_Hal_ReadTimerCount(g_pitUsChannel) << 32U) +
                      PIT_Hal_ReadTimerCount(g_pitUsChannel - 1U)
                      >= timeToBe)
    {}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_ClearIntFlag
 * Description   : Clears the timer interrupt flag.
 *
 *END**************************************************************************/
void PIT_ClearIntFlag(uint32_t channel)
{
    PIT_Hal_ClearIntFlag(channel);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_IsIntPending
 * Description   : Reads the current timer timeout flag.
 *
 *END**************************************************************************/
bool PIT_IsIntPending(uint32_t channel)
{
    return PIT_Hal_IsIntPending(channel);
}

void PIT_IRQHandler(void)
{
	PIT_Hal_ClearIntFlag(0U);
	if (pitConfigPtr->isInterruptEnabled)
	{
		(*(pitConfigPtr->callbackFunc))();
	}
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

