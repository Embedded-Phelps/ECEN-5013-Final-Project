#include "includes.h"

void task_Init(void)
{
	/* Enable the clock gate to ALL the Port module */
	PORT_ENABLE_CLK(A);		
	PORT_ENABLE_CLK(B);
	PORT_ENABLE_CLK(C);
	PORT_ENABLE_CLK(D);
	PORT_ENABLE_CLK(E);
	
	/* Configure low power setting */
	const power_manager_user_config_t VLPRConfig = {.mode = powerManagerVLPR,
													.sleepOnExit = false};
	power_manager_user_config_t RunConfig = VLPRConfig;
	power_manager_user_config_t WaitConfig = VLPRConfig;
	power_manager_user_config_t StopConfig = VLPRConfig;
	power_manager_user_config_t VLPWConfig = VLPRConfig;
	
	power_manager_user_config_t const *powerConfig[] = {
		&RunConfig,
		&WaitConfig,
		&StopConfig,
		&VLPRConfig,
		&VLPWConfig
	};
	
	/* Enable system interrupt */
	__enable_irq();
	app_ADCInit();
}
int main(void){
	int32_t temp;

	while (1){
		temp = read_OnChipTemperature();
		temp = temp;
	}
}
