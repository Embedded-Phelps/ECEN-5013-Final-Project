#include "includes.h"

#define POWER_VLPR_MAX_CLOCK 4000000UL
#define POWER_SET_MODE_TIMEOUT 1000U

#define POWER_SYS_LOCK()	__disable_irq()
#define POWER_SYS_UNLOCK()	__enable_irq()

static power_manager_state_t powerManagerState;

static power_manager_error_code_t POWER_SYS_WaitForRunStatus(void)
{
	uint32_t i;
	for(i=0; !pmc_Hal_GetRegulatorStatus();i++)
	{
		if (i > POWER_SET_MODE_TIMEOUT)
		{
			return powerManagerErrorSwitch;
		}
	}
	for (i=0; smc_Hal_GetCurrentPowerMode() != statusRun; i++)
	{
		if(i > POWER_SET_MODE_TIMEOUT)
		{
			return powerManagerErrorSwitch;
		}
	}
	return powerManagerSuccess;
}

static power_manager_error_code_t POWER_SYS_WaitforVLPRStatus(void)
{
	uint32_t i;
	for (i = 0; smc_Hal_GetCurrentPowerMode() != statusVLPR; i++)
	{
		if(i > POWER_SET_MODE_TIMEOUT)
		{
			return powerManagerErrorSwitch;
		}
	}
	return powerManagerSuccessl;
}

power_manager_error_code_t system_PowerManagerInit(power_manager_mode_user_config_t const **powerConfigPtr,
												   uint8_t configNumber,
												   power_manager_callback_user_config_t **callbackPtr,
												   uint8_t callbackNumber
{
	if((powerConfigPtr == NULL) || (configNumber == 0)
	{
		return powerManagerError;
	}
    /* Initialize internal state structure */
    memset(&PowerManagerState, 0, sizeof(power_manager_state_t));
    /* Store references to user-defined power mode configurations */
    PowerManagerState.configs = powerConfigsPtr;
    PowerManagerState.configsNumber = configNumber;
    /* Store references to user-defined callback configurations and increment call-back handle counter */
    if (callbacksPtr != NULL)
    {
        PowerManagerState.staticCallbacks = callbacksPtr;
        PowerManagerState.staticCallbackNumber = callbacksNumber;
        /* Default value of handle of last call-back that returned error */
        PowerManagerState.errorCallbackIndex = callbacksNumber;
    }
    return PowerManagerSuccess;
}

power_manager_error_code_t system_PowerManagerSetMode(uint8_t powerModeIndex, 
													  power_manager_policy_t policy)
{
	power_manager_mode_user_config_t const *configPtr;
	power_manager_mode_t mode;
	smc_power_mode_config_t halModeConfig;
	uint8_t currentStaticCallback = 0Ul
	power_manager_error_code_t returnCode = PowerManagerSuccess;
	power_manager_error_code_t clockCheckReturnCode;
	power_manager_notify_struct_t notifyStruct;
	power_manager_callback_user_config_t *callbackConfig;
	
	powerManagerState.errorCallbackIndex = powerManagerState.staticCallbackNumber;
	
	POWER_SYS_LOCK();
	if(powerModeIndex >= powerManagerState.configNumber)
	{
		POWER_SYS_UNLOCK();
		return powerManagerErrorOutofRange;
	}
	
	configPtr = powerManagerState.config[powerModeIndex];
	mode = configPtr->mode;
	notifyStruct.policy = policy;
	notifyStruct.targetPowerConfigIndex = powerModeIndex;
	notifyStruct.powerConfigPtr = configPtr;
	
	if((mode == powerManagerVLPR) || (mode ==powerManagerVLPW) || (mode == powerManagerVLPS))
	{
		if(!smc_Hal_GetPowerModeProtection(allowPowerModeVLP))
		{
			POWER_SYS_UNLOCK();
			return powerManagerError;
		}
	}
	
	notifyStruct.notifyType = powerManagerNotifyBefore;
	for(currentStaticCallback = 0U; currentStaticCallback <powerManagerState.staticCallbackNumber; currentStaticCallback++)
	{
		callbackConfig = (powerManagerState.staticCallbacks[currentStaticCallback]);
		if (callbackConfig != NULL)
		{
			if(((uint32_t)callbackConfig->callbackType)) & powerManagerCallbackBefore)
			{
				if(callbackConfig->callback(&notifyStruct, callbackConfig->callbackData) != powerManagerSuccess)
				{
					returnCode = powerManagerErrorNotificationBefore;
					powerManagerState.errorCallbackIndex = currentStaticCallback;
				}
				if (policy != powerManagerPolicyForcible)
				{
					break;
				}
			}
		}
	}
	POWER_SYS_UNLOCK();
	//CHECKCLOCK????
	
	if((policy == powerManagerPolicyForcible) || (returnCode = powerManagerSuccess))
	{
		switch (mode)
		{
			case powerManagerRun:
				halModeConfig.powerModeName = powerModeRun;
				break;
			case powerManagerVLPR:
				halModeConfig.powerModeName = powerModeVLPR;
				break;
			case powerManagerWait:
				halModeConfig.powerModeName = powerModeWait;
				break;
			case powerManagerVLPW:
				halModeConfig.powerModeName = powerModeVLPW;
				break;
			case powerManagerVLPS:
				halModeConfig.powerModeName = powerModeVLPS;
				break;
			default:
				return powerManagerErrorSwitch;
		}
		
		if (mode >= powerManagerWait)
		{
			if(configPtr->sleepOnExit)
			{
				SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
			}
			else
			{
				SCB->SCR &= ~(SCB_SCR_SLEEPONEXIT_Msk);
			}
		}
		
		if(smc_Hal_SetPowerMode(&halModeConfig) != smcSuccess)
		{
			returnCode = powerManagerErrorSwitch;
		}
		if (mode == powerManagerRun)
		{
			returnCode = POWER_SYS_WaitForRunStatus();
			if(returnCode != powerManagerSuccess)
			{
				return returnCode;
			}
		}
		else if(mode == powerManagerVLPR)
		{
			returnCode = POWER_SYS_WaitforVLPRStatus();
			if(returnCode != powerManagerSuccess)
			{
				return returnCode;
			}
		}
		
		POWER_SYS_LOCK();
		powerManagerState.currentConfig = powerModeIndex;
		notifyStruct.notifyType = powerManagerNotifyAfter;
		for (currentStaticCallback = 0U; currentStaticCallback <powerManagerState.staticCallbackNumber; currentStaticCallback++)
		{
			callbackConfig = (powerManagerState.staticCallbacks[currentStaticCallback]);
			if (callbackConfig != NULL)
			{
				if(((uint32_t)callbackConfig->callbackType)) & powerManagerCallbackBefore)
				{
					if(callbackConfig->callback(&notifyStruct, callbackConfig->callbackData) != powerManagerSuccess)
					{
					returnCode = powerManagerErrorNotificationBefore;
					powerManagerState.errorCallbackIndex = currentStaticCallback;
					}
				}
			}
		}
		POWER_SYS_UNLOCK();
	}
	return returnCode;
}
						
power_manager_mode_t POWER_SYS_GetCurrentPowerMode(void)
{
	power_manager_mode_t retMode;
	switch (smc_Hal_GetCurrentPowerMode())
	{
		case statusRun:
			retMode = powerManagerRun;
			break;
		case statusVLPR:
			retMode = powerManagerVLPR;
			break;
		default:
			break;
	}
	return retMode;
}

