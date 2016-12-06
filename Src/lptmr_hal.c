
#include "includes.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_HAL_Init
 * Description   : Initialize LPTMR module to reset state.
 *
 *END**************************************************************************/
void LPTMR_Hal_Init(void)
{
    lptmr_working_mode_user_config_t working_mode_config;
    lptmr_prescaler_user_config_t prescaler_config;
    
    LPTMR_Hal_Disable();
    LPTMR_Hal_ClearIntFlag();

    working_mode_config.timerModeSelect = lptmrTimerModeTimeCounter;
    working_mode_config.freeRunningEnable = false;
    working_mode_config.pinPolarity = lptmrPinPolarityActiveHigh;
    working_mode_config.pinSelect = lptmrPinSelectInput0;
    LPTMR_Hal_SetTimerWorkingMode(working_mode_config);
    
    prescaler_config.prescalerValue = lptmrPrescalerDivide2;
    prescaler_config.prescalerBypass = true;
    prescaler_config.prescalerClockSelect = lptmrPrescalerClock0;
    LPTMR_Hal_SetPrescalerMode(prescaler_config);
    
    LPTMR_Hal_SetCompareValue(0U);
    LPTMR_Hal_SetIntCmd(false);
}

 /*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_HAL_SetTimerWorkingMode
 * Description   : Config the LPTMR working mode.
 *
 *END**************************************************************************/
void LPTMR_Hal_SetTimerWorkingMode(lptmr_working_mode_user_config_t timerMode)
{
    uint32_t csr;
    
    csr = LPTMR0_CSR;
    csr &= ~(LPTMR_CSR_TCF_MASK | LPTMR_CSR_TMS_MASK | LPTMR_CSR_TFC_MASK 
             | LPTMR_CSR_TPP_MASK | LPTMR_CSR_TPS_MASK);
    csr |= LPTMR_CSR_TMS(timerMode.timerModeSelect) 
        | LPTMR_CSR_TFC(timerMode.freeRunningEnable)
        | LPTMR_CSR_TPP(timerMode.pinPolarity) 
        | LPTMR_CSR_TPS(timerMode.pinSelect); 
    
    LPTMR0_CSR = csr;
}

 /*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_Hal_SetPrescalerMode
 * Description   : Set the LPTMR prescaler mode.
 *
 *END**************************************************************************/
void LPTMR_Hal_SetPrescalerMode(lptmr_prescaler_user_config_t prescaler_config)
{
    uint32_t psr;
    
    psr = LPTMR_PSR_PCS(prescaler_config.prescalerClockSelect)
        | LPTMR_PSR_PBYP(prescaler_config.prescalerBypass)
        | LPTMR_PSR_PRESCALE(prescaler_config.prescalerValue);

    LPTMR0_PSR = psr;
}

 /*FUNCTION**********************************************************************
 *
 * Function Name : LPTMR_HAL_GetCounterValue
 * Description   : Gets the LPTMR counter value..
 *
 *END**************************************************************************/
uint32_t LPTMR_Hal_GetCounterValue(void)
{
    LPTMR0_CNR = 0;  /* Must first write to the CNR with any value */
    return (uint32_t)(LPTMR0_CNR);
}
