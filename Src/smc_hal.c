#include "includes.h"

static void smc_Hal_SetPowerModeRun(void)
{
	SMC_PMCTRL = SMC_PMCTRL_RUNM(smcRun); 
	while (statusRun != smc_Hal_GetCurrentPowerMode()){}
}

static void smc_Hal_SetPowerModeVLPR(void)
{
	SMC_PMCTRL = SMC_PMCTRL_RUNM(smcVLPR); 
	while (statusVLPR != smc_Hal_GetCurrentPowerMode()){}
}

smc_error_t smc_Hal_SetPowerMode(const smc_power_mode_config_t *powerModeConfig)
{
	volatile uint32_t dummy;
	power_mode_status_t currentPowerMode = smc_Hal_GetCurrentPowerMode();
	
	switch(currentPowerMode)
	{
		case statusRun:
			if (powerModeVLPW & powerModeConfig->powerModeName)
			{
				smc_Hal_SetPowerModeVLPR();
			}
			break;
		case statusVLPR:
			if ((powerModeStop | powerModeWait) & powerModeConfig->powerModeName)
			{
				smc_Hal_SetPowerModeRun();
			}
			break;
		default:;
	}
	
	switch(powerModeConfig->powerModeName)
	{
		case powerModeRun:
			smc_Hal_SetPowerModeRun();
			break;
		case powerModeVLPR:
			smc_Hal_SetPowerModeVLPR();
			break;
		case powerModeWait:
		case powerModeVLPW:
			SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
			dummy = SMC_PMCTRL;
			dummy = dummy;
			__WFI();
			break;
		default:;
	}
	return smcSuccess;
}

power_mode_status_t smc_Hal_GetCurrentPowerMode(void)
{
	return (power_mode_status_t) SMC_PMSTAT;
}

