
#include "includes.h"

lptmr_state_t * lptmrStatePtr = NULL;

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_Init
 * Description   : initializes the LPTMR driver.
 * This function will initialize the LPTMR driver according to user configure
 * strcuture.
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_Init(lptmr_state_t *userStatePtr, const lptmr_user_config_t* userConfigPtr)
{
    lptmr_prescaler_user_config_t prescalerUserConfig;
    lptmr_working_mode_user_config_t workingModeUserConfig;

    if ((!userConfigPtr) || (!userStatePtr))
    {
        return status_LPTMR_NullArgument;
    }

    /* prescaler value 0 is invalid while working as pulse counter */
    if ((lptmrTimerModePulseCounter == userConfigPtr->timerMode) &&
         (true == userConfigPtr->prescalerEnable) &&
         (lptmrPrescalerDivide2 == userConfigPtr->prescalerValue))
    {
        return status_LPTMR_InvalidPrescalerValue;
    }

    /* Enable clock for lptmr */
    LPTMR_Hal_EnableClock(); 

    /* Disable lptmr and reset lptmr logic */
    LPTMR_Hal_Disable();

    /* LPTMR prescaler configure */
    prescalerUserConfig.prescalerClockSelect = (lptmr_prescaler_clock_select_t)userConfigPtr->prescalerClockSource;
    prescalerUserConfig.prescalerBypass = (uint8_t)(userConfigPtr->prescalerEnable == true);
    prescalerUserConfig.prescalerValue = userConfigPtr->prescalerValue;
    LPTMR_Hal_SetPrescalerMode(prescalerUserConfig);

    /* Working Mode configure */
    workingModeUserConfig.timerModeSelect = userConfigPtr->timerMode;
    workingModeUserConfig.freeRunningEnable = userConfigPtr->freeRunningEnable;
    workingModeUserConfig.pinPolarity = userConfigPtr->pinPolarity;
    workingModeUserConfig.pinSelect = userConfigPtr->pinSelect;
    LPTMR_Hal_SetTimerWorkingMode(workingModeUserConfig);

    /* Internal context */
    lptmrStatePtr = userStatePtr;

    userStatePtr->userCallbackFunc = NULL;

    /* LPTMR interrupt */
    if (userConfigPtr->isInterruptEnabled)
    {
        LPTMR_Hal_SetIntCmd(true);
        LPTMR_Hal_EnableNVICInterrupt();
    }
    else
    {
        LPTMR_Hal_SetIntCmd(false);
        LPTMR_Hal_DisableNVICInterrupt();
    }

    /* Caculate prescaler clock frequency */
    if (lptmrTimerModeTimeCounter == userConfigPtr->timerMode)
    {
        userStatePtr->prescalerClockHz = 1000; ///????

        if (userConfigPtr->prescalerEnable)
        {
            userStatePtr->prescalerClockHz = (userStatePtr->prescalerClockHz >> ((uint32_t)(userConfigPtr->prescalerValue+1)));
        }
    }

    return status_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_Deinit
 * Description   : Deinit the LPTMR driver.
 * This function will deinit the LPTMR driver, disable LPTMR clock,
 * and disable LPTMR interrupt.
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_Deinit(void)
{
    assert(LPTMR_Hal_isClockEnabled());

    /* Turn off lptmr hal */
    LPTMR_Hal_Disable();

    /* Reset all register to reset value */
    LPTMR_Hal_Init();

    /* Disable the interrupt */
    LPTMR_Hal_DisableNVICInterrupt(); 

    /* Disable clock for lptmr */
    LPTMR_Hal_DisableClock();

    /* Cleared state pointer */
    lptmrStatePtr = NULL;

    return status_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_Start
 * Description   : Start LPTMR counter
 * This function will start LPTMR internal counter to count the time or pulse
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_Start(void)
{
    assert(LPTMR_Hal_isClockEnabled()); 

    LPTMR_Hal_Enable();

    return status_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_Stop
 * Description   : Stop LPTMR counter
 * This function will stop LPTMR internal counter
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_Stop(void)
{
    assert(LPTMR_Hal_isClockEnabled());

    LPTMR_Hal_Disable();

    return status_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_SetTimerPeriodUs
 * Description   : Set LPTMR timer counter period with unit microsecond
 * This function is used to set LPTMR timer counter period with unit microsecond
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_SetTimerPeriodUs(uint32_t us)
{
    assert(us > 0);
    assert(LPTMR_Hal_isClockEnabled()); 

    uint32_t tick_count;

    if (lptmrStatePtr->prescalerClockHz < 1000000U)
    {
        tick_count = (us/(1000000U/lptmrStatePtr->prescalerClockHz));
    }
    else
    {
        tick_count = (us*(lptmrStatePtr->prescalerClockHz/1000000U));
    }

    /* CMR register is 16 Bits */
    if (tick_count > 0xFFFFU)
    {
        return status_LPTMR_TimerPeriodUsTooLarge;
    }

    /* CMR of 0 leaves the hardware trigger asserted */
    if (tick_count <= 1)
    {
        return status_LPTMR_TimerPeriodUsTooSmall;
    }

    /* We have to reduce by 1 as interrupt occurs when the CNR register equals the value of
     * of CMR register and then increments
     */
    LPTMR_Hal_SetCompareValue(tick_count - 1);

    return status_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_GetCurrentTimeUs
 * Description   : Get LPTMR current time with unit microsecond
 * This function is used to get LPTMR current time with unit microsecond
 *
 *END**************************************************************************/
uint32_t LPTMR_GetCurrentTimeUs(void)
{
    assert(LPTMR_Hal_isClockEnabled());

    uint32_t us;

    if (lptmrStatePtr->prescalerClockHz < 1000000U)
    {
        us = LPTMR_Hal_GetCounterValue()*(1000000U/lptmrStatePtr->prescalerClockHz);
    }
    else
    {
        us = LPTMR_Hal_GetCounterValue()/(lptmrStatePtr->prescalerClockHz/1000000U);
    }

    return us;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_SetPulsePeriodCount
 * Description   : Set the pulse period value while LPTMR working in pulse counter mode
 * This function is used to set the pulse period value while LPTMR working in pulse counter mode
 *
 *END**************************************************************************/
lptmr_status_t LPTMR_SetPulsePeriodCount(uint32_t pulsePeriodCount)
{
    assert(pulsePeriodCount > 0);
    assert(LPTMR_Hal_isClockEnabled());

    LPTMR_Hal_SetCompareValue(pulsePeriodCount);

    return status_LPTMR_Success;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_DRV_GetCurrentPulseCount
 * Description   : Get current pulse count captured in the pulse input pin
 * This function is used to get current pulse count captured in the pulse input pin
 *
 *END**************************************************************************/
uint32_t LPTMR_GetCurrentPulseCount(void)
{
    assert(LPTMR_Hal_isClockEnabled()); 
    uint32_t count;
    count = LPTMR_Hal_GetCounterValue();

    return count;
}


/*FUNCTION*********************************************************************
 *
 * Function Name : LPTMR_DRV_InstallCallback
 * Description   : Install the user-defined callback in LPTMR module.
 * When an LPTMR interrupt request is served, the callback will be executed
 * inside the ISR.
 *
 *END*************************************************************************/
lptmr_status_t LPTMR_InstallCallback(lptmr_callback_t userCallback)
{
    if (!lptmrStatePtr)
    {
        return status_LPTMR_NotInitlialized;
    }
    /* Fill callback function into state structure. */
    lptmrStatePtr->userCallbackFunc = userCallback;

    return status_LPTMR_Success;
}


/*FUNCTION*********************************************************************
 *
 * Function Name : LPTMR_DRV_IRQHandler
 * Description   : The driver-defined ISR in LPTMR module.
 * It includes the process for interrupt mode defined by driver. Currently, it
 * will be called inside the system-defined ISR.
 *
 *END*************************************************************************/
void LPTMR0_IRQHandler(void)
{
    assert(LPTMR_Hal_isClockEnabled());

    /* Clear interrupt flag */
    LPTMR_Hal_ClearIntFlag();

    if (lptmrStatePtr)
    {
        if (lptmrStatePtr->userCallbackFunc)
        {
            /* Execute user-defined callback function. */
            (*(lptmrStatePtr->userCallbackFunc))();
        }
    }
}

/*******************************************************************************
 * EOF
 *******************************************************************************/

