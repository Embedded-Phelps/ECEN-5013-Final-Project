#include "includes.h"

#define POWER_VLPR_MAX_CLOCK 4000000UL
#define POWER_SET_MODE_TIMEOUT 1000U
static power_manager_state_t powerManagerState;

power_manager_error_code_t system_PowerManagerInit(power_manager_mode_user_config_t const **powerConfigPtr,
												   uint8_t configNumber,
												   power_manager_callback_user_config_t **callbackPtr,
												   uint8_t callbackNumber
{
	if((powerConfigPtr == NULL) || (configNumber == 0)
	{
		return powerManagerError;
	}
	
}

power_manager_error_code_t system_PowerManagerDeInit(void);

power_manager_error_code_t system_PowerManagerSetMode(uint8_t powerModeIndex, 
													  power_manager_policy_t policy);
						
power_manager_error_code_t system_PowerMangerGetLastMode(uint8_t *powerModeIndexPtr);

power_manager_error_code_t POWER_SYS_GetLastModeConfig(power_manager_user_config_t const ** powerModePtr);