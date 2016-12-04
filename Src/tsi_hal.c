#include "includes.h"

void TSI_Hal_EnableLowPower(void)
{
	TSI_Hal_EnableStop();
}

void TSI_Hal_DisableLowPower(void)
{
	TSI_Hal_DisableStop();
}

void TSI_Hal_Init(void)
{
	TSI0_GENCS = 0U;
	TSI0_DATA = 0U;
	TSI0_TSHD = 0U;
}

void TSI_Hal_SetConfiguration(tsi_config_t *config)
{
	uint32_t module_enabled_flag = TSI_Hal_IsModuleEnabled();
	uint32_t int_enabled_flag = TSI_Hal_IsInterruptEnabled();
	assert(config != NULL);

	if(module_enabled_flag)
	{
		TSI_Hal_DisableModule();
	}
	if(int_enabled_flag)
	{
		TSI_Hal_DisableInterrupt();
	}
	TSI_Hal_SetPrescaler(config->ps);
	TSI_Hal_SetNumberOfScan(config->nscan);
	TSI_Hal_SetRefChargeCurrent(config->refchargecurrent);
	TSI_Hal_SetElectrodeChargeCurrent(config->exchargecurrent);
	TSI_Hal_SetMode(config->mode);
	TSI_Hal_SetOscilatorVoltRails(config->dvolt);
	TSI_Hal_SetLowThreshold(config->thresl);
	TSI_Hal_SetHighThreshold(config->thresh);
	
	if(module_enabled_flag)
	{
		TSI_Hal_EnableModule();
	}
	if(int_enabled_flag)
	{
		TSI_Hal_EnableInterrupt();
	}
}


