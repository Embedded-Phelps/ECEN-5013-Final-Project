#ifndef __POWER_MANAGER_H__
#define __POWER_MANAGER_H__

#include "includes.h"

typedef enum power_manager_mode
{
	powerManagerRun,
	powerManagerVLPR,
	powerManagerWait,
	powerManagerVLPW,
	powerManagerStop,
	powerManagerVLPS,
	powerManagerVLLS1,
	powerManagerVLLS2,
	powerManagerVLLS3
}power_manager_mode_t;

typedef enum power_manager_error_code
{
	powerManagerSuccess,
	powerManagerError,
	powerManagerErrorOutOfRange,
	powerManagerErrorSwitch,
	powerManagerErrorNotificatonBefore,
	powerManagerErrorNotificatonAfter,
	powerManagerErrorClock
}power_manager_error_code_t;

typedef enum power_manager_policy
{
	powerManagerPolicyAgreement,
	powerManagerPolicyForcible
}power_manager_policy_t;

typedef power_manager_notify
{
	powerManagerNotifyRecover = 0x00U;
	powerManagerNotifyBefore = 0x01U,
	powerManagerNotifyAfter = 0x02U
}power_manager_notify_t;

typedef power_manager_callback_type
{
	powerManagerCallBackBefore = 0x01U,
	powerManagerCallBackAfter = 0x02U,
	powerManagerCallBackBeforeAfter = 0x03U
}power_manager_callback_type_t;

typedef void power_manager_callback_data_t;

typedef struct power_manager_mode_user_config
{
	power_manager_mode_t mode;
	bool sleepOnExit;
}power_manager_mode_user_config_t;

typedef struct power_manager_notify_struct
{
	uint8_t targetPowerConfigIndex;
	power_manager_mode_user_config_t const *powerConfigPtr;
	power_manager_policy_t policy;
	power_manager_notify_t notifyType;
}power_manager_notify_struct_t;

typedef power_manager_error_code_t (* power_manager_callback_t)(
    power_manager_notify_struct_t * notify,
    power_manager_callback_data_t * dataPtr
);

typedef struct   
{
    power_manager_callback_t callback;
    power_manager_callback_type_t callbackType;
    power_manager_callback_data_t * callbackData;
}power_manager_callback_user_config_t;

typedef struct power_manager_state
{
	power_manager_mode_user_config_t const **config;
	uint8_t configNumber;
	power_manager_callback_user_config_t **staticCallbacks;
	uint8_t staticCallbackNumber;
	uint8_t errorCallbackIndex;
	uint8_t currentConfig;
}power_manager_state_t;

#if defined(__cplusplus)
extern "C" {
#endif

power_manager_error_code_t system_PowerManagerInit(power_manager_mode_user_config_t const **powerConfigPtr,
												   uint8_t configNumber,
												   power_manager_callback_user_config_t **callbackPtr,
												   uint8_t callbackNumber);

power_manager_error_code_t system_PowerManagerDeInit(void);

power_manager_error_code_t system_PowerManagerSetMode(uint8_t powerModeIndex, 
													  power_manager_policy_t policy);
						
power_manager_error_code_t system_PowerMangerGetLastMode(uint8_t *powerModeIndexPtr);

power_manager_error_code_t POWER_SYS_GetLastModeConfig(power_manager_user_config_t const ** powerModePtr);


#if defined(__cplusplus)
}
#endif

#endif