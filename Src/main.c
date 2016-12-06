#include "includes.h"

void task_Init(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
	/* Configure low power setting */
	//const power_manager_user_config_t VLPRConfig = {.mode = powerManagerWait,
	//												.sleepOnExit = false};
	
	//power_manager_user_config_t const *powerConfig[] = {
	//	&WaitConfig
	//};
	PORT_Hal_SetMuxMode(PORTB,16u,portPinDisabled);
	PORT_Hal_SetMuxMode(PORTB,17u,portPinDisabled);
	/* Enable system interrupt */
	__enable_irq();
	//app_ADCInit();
}

int main(void){
	lptmr_state_t lptmrState;
	lptmr_user_config_t lptmrUserConfig =
    {
        .timerMode            = lptmrTimerModeTimeCounter, /*! Use LPTMR in Time Counter mode */
        .freeRunningEnable    = true, /*! When hit compare value, set counter back to zero */
        .prescalerEnable      = false, /*! bypass prescaler */
        .prescalerClockSource = clockLptmrSrcLpoClk, /*! use 1kHz Low Power Clock */
        .isInterruptEnabled   = false
    };
	
	uint16_t tsi_MeasureResult[BOARD_TSI_ELECTRODE_CNT];
	uint8_t tsi_Channel[BOARD_TSI_ELECTRODE_CNT];
	uint32_t i, j, avg_Untouch = 0U;
	uint32_t sum_Untouch = 0U;
	uint32_t avg_Measure;
	tsi_status_t tsi_Status;
	
	task_Init();
	LPTMR_Init(&lptmrState, &lptmrUserConfig);
	LPTMR_Start();
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
	LED1_SELECT;
	LED1_EN;
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
				LED1_ON;
		}
		else
		{
				LED1_OFF;
		}
		SYS_TimeDelay(100);
	}
}
