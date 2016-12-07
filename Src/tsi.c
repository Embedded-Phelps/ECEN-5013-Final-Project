#include "includes.h"

tsi_state_t * tsiStatePtr = NULL;

extern void TSI0_IRQHandler(void);

tsi_status_t TSI_Init(tsi_state_t *tsiState, const tsi_user_config_t *tsiUserConfig)
{
    tsi_state_t * tsiSt = tsiStatePtr;
	
	SYS_EnterCritical();
	if(tsiSt)
	{
		SYS_ExitCritical();
		return status_TSI_Initialized;
	}
	tsiSt = tsiStatePtr = tsiState;
	memset(tsiSt, 0, sizeof(tsi_state_t));
	SYS_MutexCreate(&tsiSt->lock);
	SYS_MutexCreate(&tsiSt->lockChangeMode);
	if(status_SYS_Success != SYS_MutexLock(&tsiSt->lock, SYS_WAIT_FOREVER))
	{
		SYS_ExitCritical(); 
		return status_TSI_Error;
	}
	SYS_ExitCritical();
	tsiSt->operationMode = tsi_OpModeNormal;
	tsiSt->opModeData[tsiSt->operationMode].config = *tsiUserConfig->config;
	tsiSt->pCallbackFunc = tsiUserConfig->callback;
	tsiSt->usrData = tsiUserConfig->usrData;
	tsiSt->isBlockingMeasure = false;
	
	TSI_Hal_EnableClock(); 
	
	SYS_SemaphoreCreate(&tsiSt->irqSync, 0); 
	
	TSI_Hal_Init();
	TSI_Hal_SetConfiguration(&tsiSt->opModeData[tsiSt->operationMode].config);
	TSI_Hal_EnableInterrupt();
	TSI_Hal_EnableEndOfScanInterrupt();
	TSI_Hal_EnableSoftwareTriggerScan();
	
	tsiState->opModeData[tsiState->operationMode].enabledElectrodes = 0;
	TSI_Hal_EnableNVICInterrupt(); 
	tsiSt->status = status_TSI_Initialized;
	SYS_MutexUnlock(&tsiSt->lock); 
	return status_TSI_Success;
}

tsi_status_t TSI_DeInit(void)
{
	tsi_state_t *tsiState = tsiStatePtr;
	if(tsiState == NULL)
	{
		return status_TSI_Error;
	}
	TSI_Hal_DisableInterrupt();
	tsiState->opModeData[tsiState->operationMode].enabledElectrodes = 0;
	TSI_Hal_ClearOutOfRangeFlag();
	TSI_Hal_ClearEndOfScanFlag();
	TSI_Hal_DisableModule();
	TSI_Hal_DisableNVICInterrupt();
	
	//SYS_SemaphoreDestroy(&tsiState->irqSync); 
	tsiState = NULL;
	TSI_Hal_DisableClock(); 
	return status_TSI_Success;
}

 tsi_status_t TSI_SetCallbackFunc(const tsi_callback_t pCallback, void *usrData)
 {
	 tsi_state_t *tsiState = tsiStatePtr;
	 if(status_SYS_Success != SYS_MutexLock(&tsiState->lock, SYS_WAIT_FOREVER))
	 {
		 return status_TSI_Error;
	 }
	 if(tsiStatePtr->status != status_TSI_Initialized)
	 {
		 SYS_MutexUnlock(&tsiState->lock);
		 return tsiStatePtr->status;
	 }
	 tsiStatePtr->pCallbackFunc = pCallback;
	 tsiStatePtr->usrData = usrData;
	 SYS_MutexUnlock(&tsiState->lock);
	 return status_TSI_Success;
 }
 
tsi_status_t TSI_Measure(void)
{
	tsi_state_t *tsiState = tsiStatePtr;
	uint32_t first_pen, pen;
	if(status_SYS_Success != SYS_MutexLock(&tsiState->lock, SYS_WAIT_FOREVER))
	{
		 return status_TSI_Error;
	}
	if(tsiStatePtr->status != status_TSI_Initialized)
	{
		 SYS_MutexUnlock(&tsiState->lock);
		 return tsiStatePtr->status;
	} 
	if(!tsiState->opModeData[tsiState->operationMode].enabledElectrodes)
	{
		SYS_MutexUnlock(&tsiState->lock);
		return status_TSI_InvalidChannel;
	}
	tsiState->status = status_TSI_Busy;
	first_pen = 0U;
	pen = tsiState->opModeData[tsiState->operationMode].enabledElectrodes;
	while (((pen >> first_pen) & 0x1U) == 0U){
		first_pen ++;
	}
	SYS_MutexUnlock(&tsiState->lock);
	TSI_Hal_DisableModule();
	TSI_Hal_SetMeasureChannelNumber(first_pen);
	TSI_Hal_EnableSoftwareTriggerScan();
	TSI_Hal_EnableModule();
	TSI_Hal_StartSoftwareTrigger();
	
	return status_TSI_Success;
}

tsi_status_t TSI_MeasureBlocking(void)
{
	system_status_t syncStatus;
	tsi_status_t tsiStatus;
	tsi_state_t *tsiState = tsiStatePtr;
	if((tsiStatus = TSI_Measure()) != status_TSI_Success)
	{
		return tsiStatus;
	}		
	tsiState->isBlockingMeasure = true;
	do
	{
		syncStatus = SYS_SemaWait(&tsiState->irqSync, 1000); //?????
	}while(syncStatus == status_SYS_Idle);
	if (syncStatus != status_SYS_Success)
	{
		TSI_AbortMeasure();
		return status_TSI_Error;
	}
	return status_TSI_Success;
}	

tsi_status_t TSI_AbortMeasure(void)
{
	tsi_status_t tsiStatus = status_TSI_Success;
	tsi_state_t *tsiState = tsiStatePtr;
	if(status_SYS_Success != SYS_MutexLock(&tsiState->lock, SYS_WAIT_FOREVER))
	{
		return status_TSI_Error;
	}
	if(tsiState->status == status_TSI_Recalibration)
	{
		tsiStatus = status_TSI_Recalibration;
	}
	else if(tsiState->status != status_TSI_Initialized)
	{
		TSI_Hal_ClearOutOfRangeFlag();
		TSI_Hal_ClearEndOfScanFlag();
		TSI_Hal_DisableModule();
		if(tsiState->isBlockingMeasure)
		{
			SYS_SemaPost(&tsiState->irqSync);//?????
			tsiState->isBlockingMeasure =false;
		}
		tsiState->status = status_TSI_Initialized;
	}
	SYS_MutexUnlock(&tsiState->lock);
	return tsiStatus;
}

tsi_status_t TSI_GetStatus(void)
{
	return tsiStatePtr->status;
}

tsi_status_t TSI_EnableElectrode(const uint32_t channel, const bool enable)
{
	tsi_state_t *tsiState = tsiStatePtr;
	assert(channel < MAX_TSI_CHANNEL_INDEX);
	
	if(status_SYS_Success != SYS_MutexLock(&tsiState->lock, SYS_WAIT_FOREVER))
	{
		return status_TSI_Error;
	}
	if(tsiState->status != status_TSI_Initialized)
	{
		SYS_MutexUnlock(&tsiState->lock);
		return tsiState->status;
	}
	if(enable)
	{
		tsiState->opModeData[tsiState->operationMode].enabledElectrodes |= (1U << channel);
	}
	else
	{
		tsiState->opModeData[tsiState->operationMode].enabledElectrodes &= ~(1U << channel);
	}
	SYS_MutexUnlock(&tsiState->lock);
	return status_TSI_Success;
}

uint32_t TSI_GetEnableElectrode(void)
{
	tsi_state_t *tsiState =tsiStatePtr;
	return tsiState->opModeData[tsiState->operationMode].enabledElectrodes;
}

tsi_status_t TSI_GetCounter(const uint32_t channel, uint16_t *counter)
{
	tsi_state_t *tsiState = tsiStatePtr;
	assert(channel < MAX_TSI_CHANNEL_INDEX);
	assert(counter);
	
	if(!((1U << channel) & (tsiState->opModeData[tsiState->operationMode].enabledElectrodes)))  
    {
        return status_TSI_InvalidChannel;
    }

    *counter = tsiState->counters[channel];

    return status_TSI_Success;
}

uint32_t TSI_GetUnTouchBaseline(uint8_t * tsiChn)
{
	uint8_t i, j;
	uint16_t tsi_MeasureResult[BOARD_TSI_ELECTRODE_CNT];
	uint32_t sum_Untouch = 0U;
	uint32_t avg_Untouch = 0U;
	
	tsi_status_t tsi_Status;
	
	tsi_Status = TSI_MeasureBlocking();
	if(tsi_Status != status_TSI_Success)
	{
		return -1;
	}
	for (i=0; i < TSI_THRESHOLD_SAMPLING; i++)
	{
		for(j = 0; j < BOARD_TSI_ELECTRODE_CNT; j++)
		{
			tsi_Status = TSI_GetCounter(tsiChn[j], &tsi_MeasureResult[j]);
			if(tsi_Status != status_TSI_Success)
			{
				return -1;
			}
			sum_Untouch += tsi_MeasureResult[j];
		}
	}
	avg_Untouch = sum_Untouch / (TSI_THRESHOLD_SAMPLING * BOARD_TSI_ELECTRODE_CNT);
	return avg_Untouch;
}

void TSI0_IRQHandler(void)
{
	tsi_state_t *tsiState = tsiStatePtr;
	uint32_t next_pen, pen;
	uint32_t channels = tsiState->opModeData[tsiState->operationMode].enabledElectrodes;
	uint32_t current_channel = TSI_Hal_GetMeasuredChannelNumber();
	
	TSI_Hal_ClearOutOfRangeFlag();
	TSI_Hal_ClearEndOfScanFlag();
	
	if((uint32_t)(1<<current_channel) & channels)
	{
		tsiState->counters[current_channel] = TSI_Hal_GetCounter();
	}
	next_pen = current_channel + 1;
	pen = channels;
	while((((pen >> next_pen) & 0x1U) == 0U) && (next_pen <16))
	{
		next_pen ++;
	}
	if(next_pen < 16)
	{
		TSI_Hal_SetMeasureChannelNumber(next_pen);
		TSI_Hal_StartSoftwareTrigger();
		return;
	}
	if(tsiState->isBlockingMeasure)
	{
		SYS_SemaPost(&tsiState->irqSync); //???
		tsiState->isBlockingMeasure = false;
	}
	else if(tsiState->pCallbackFunc)
  {
		tsiState->pCallbackFunc(tsiState->usrData);
  }
	if(tsiState->status != status_TSI_Lowpower)
	{
		tsiState->status = status_TSI_Initialized;
	}	
}
