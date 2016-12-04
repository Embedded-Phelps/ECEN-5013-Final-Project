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
	//const power_manager_user_config_t VLPRConfig = {.mode = powerManagerWait,
	//												.sleepOnExit = false};
	
	//power_manager_user_config_t const *powerConfig[] = {
	//	&WaitConfig
	//};
	
	/* Enable system interrupt */
	__enable_irq();
	//app_ADCInit();
}

int main(void){
	int32_t temp;
	
	uint16_t tsi_MeasureResult[BOARD_TSI_ELECTRODE_CNT];
	uint8_t tsi_Channel[BOARD_TSI_ELECTRODE_CNT];
	uint32_t i, j, avg_Untouch = 0U;
	uint32_t sum_Untouch = 0U;
	uint32_t avg_Measure;
	tsi_status_t tsi_Status;
	
	task_Init();
	
	static const tsi_config_t tsiHwConfig =
	{
		.ps 							= TSIElecOscPrescaler_2div,
		.exchargecurrent 	= TSIExtOscChargeCurrent_8uA,
		.refchargecurrent = TSIRefOscChargeCurrent_8uA,
		.nscan 						= TSI_Scan_8time,
		.mode 						= TSIAnalogModeSel_Capacitive,
		.dvolt 						= TSIOscVolRails_Dv_103,
		.thresh 					= TSI_HIGHTHRESHOLD,
		.thresl 					= TSI_LOWTHRESHOLD,
	};
	
	tsi_state_t myTSIState;
	
	const tsi_user_config_t tsi_Config = 
	{
		.config = (tsi_config_t *)&tsiHwConfig,
		.callback = NULL,
		.usrData = 0,
	};
	
	tsi_Channel[0] = BOARD_TSI_ELECTRODE_1;
	tsi_Channel[1] = BOARD_TSI_ELECTRODE_2; //might want to improve
	
	tsi_Status = TSI_Init(&myTSIState, &tsi_Config);
	if(tsi_Status != status_TSI_Success)
	{
		return -1;
	}
	for (i=0; i < BOARD_TSI_ELECTRODE_CNT; i++)
	{
		tsi_Status = TSI_EnableElectrode(tsi_Channel[i], true);
		if(tsi_Status != status_TSI_Success)
		{
			return -1;
		}
	}
	tsi_Status = TSI_MeasureBlocking();
	if(tsi_Status != status_TSI_Success)
	{
		return -1;
	}
	for (i=0; i < TSI_THRESHOLD_SAMPLING; i++)
	{
		for(j = 0; j < BOARD_TSI_ELECTRODE_CNT; j++)
		{
			tsi_Status = TSI_GetCounter(tsi_Channel[j], &tsi_MeasureResult[j]);
			if(tsi_Status != status_TSI_Success)
			{
				return -1;
			}
			sum_Untouch += tsi_MeasureResult[j];
		}
	}
	avg_Untouch = sum_Untouch / (TSI_THRESHOLD_SAMPLING * BOARD_TSI_ELECTRODE_CNT);
	
	
	
	while (1){
		//temp = read_OnChipTemperature();
		//temp = temp;
		tsi_Status = TSI_MeasureBlocking();
		if(tsi_Status != status_TSI_Success)
		{
			return -1;
		}
		avg_Measure = 0;
		for(i=0; i < BOARD_TSI_ELECTRODE_CNT; i++)
		{
			tsi_Status = TSI_GetCounter(tsi_Channel[i], &tsi_MeasureResult[i]);
			if(tsi_Status != status_TSI_Success)
			{
				return -1;
			}
			avg_Measure += tsi_MeasureResult[i];
		}
		avg_Measure /= BOARD_TSI_ELECTRODE_CNT;
		if(avg_Measure > avg_Untouch + 10)
		{ 
			 temp =100;
			temp=temp;
		}
		else
		{
				temp = 0;
			temp=temp;
		}
		
	}
}
