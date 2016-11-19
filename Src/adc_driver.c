#include "includes.h"
 
 //fill the default configuration for a one-time trigger mode
adc16_Status_t ADC_ConfigDefaultMode(adc16_Conv_Config_t *userConfigPtr)
{
	if(!userConfigPtr)
	{
		return status_ADC16_InValidArgument;
	}
	userConfigPtr->lowPowerEnable = true;
	userConfigPtr->clkDivider = adc16_ClkDividerOf8;
	userConfigPtr->longSampleEnable = true;
	userConfigPtr->resolution = adc16_ResolutionOf12ForSingleEnded;
	userConfigPtr->clkSrc = adc16_ClkSrcOfAdaClk;
	userConfigPtr->asyncClkEnable = true;
	userConfigPtr->highSpeedEnable = false;
	userConfigPtr->longSampleCycle = adc16_LongSampleCycleOf24;
	userConfigPtr->hwTriggerEnable = false;
	userConfigPtr->vref_Src = adc16_VrefOfDefaultVref;
	userConfigPtr->continuousConvEnable = false;
#if ADC16_HARDWARE_AVERAGE_ENABLE
	userConfigPtr->hwAverageEnable = false;
	userConfigPtr->hwAverageSamples = adc16_HWAverageSampleOf32;
#endif
#if ADC16_DMA_ENABLE
	userConfigPtr->dmaEnable = false;
#endif
	return status_ADC16_Success;
} 
 
//Initialize the ADC converter module
adc16_Status_t ADC_Init(const adc16_Conv_Config_t *userConfigPtr)
{
	if(!userConfigPtr)
	{
		return status_ADC16_InValidArgument;
	}
	adc16_Hal_EnableClock();
	adc16_Hal_Init();
	adc16_Hal_ConfigConv(userConfigPtr);
	adc16_Hal_EnableNVICInterrupt(); //??
	//adc16_Hal_SetNVICInterruptPriority(2);
	return status_ADC16_Success;
}
 
//Disable the ADC module
adc16_Status_t ADC_Disable(void)
{
	adc16_Hal_DisableNVICInterrupt();
	adc16_Hal_Init();
	adc16_Hal_DisableClock();
	return status_ADC16_Success;
}
 
//Configure the convertion channel by software
adc16_Status_t ADC_ConfigCh(adc16_Ch_Config_t *configPtr)
{
	if(!configPtr)
	{
		return status_ADC16_InValidArgument;
	}
	adc16_Hal_ConfigCh(configPtr);
	return status_ADC16_Success;
}
 
void ADC_WaitForConvComplete(void)
{
	while(!adc16_Hal_GetConvCompletedFlag())
	{}
}
 
void ADC_PauseConv(void)
{
	adc16_Ch_Config_t config;
	config.ch_Idx = adc16_Ch31;
	config.convCompleteIntEnable = false;
#if ADC16_DIFF_MODE_ENABLE
	config.diffModeEnable =false;
#endif
	ADC_ConfigCh(&config);
}
 
uint16_t ADC_GetConvValueRaw(void)
{
	return adc16_Hal_GetConvValue();
}
 
int16_t ADC_GetConvValueSigned(void)
{
	return (int16_t)adc16_Hal_GetConvValue();
}

bool ADC_GetConvFlag(adc_flag_t flag)
{
    bool bRet = false;
    switch (flag)
    {
    case adcConvActiveFlag:
        bRet = adc16_Hal_GetConvActiveFlag();
        break;
#if ADC_CALIBRATION_ENABLE
    case adcCalFailedFlag:
        bRet = adc16_Hal_GetCalFailedFlag();
        break;
    case adcCalActiveFlag:
        bRet = adc16_Hal_GetCalActiveFlag();
        break;
#endif /* ADC_CALIBRATION_ENABLE */
	case adcConvCompleteFlag:
        bRet = adc16_Hal_GetConvCompletedFlag();
        break;
    default:
        break;
    }
    return bRet;
}
#if ADC_CALIBRATION_ENABLE
adc16_Status_t ADC_GetCalibrationParam(adc_calibration_param_t *paramPtr)
{
	volatile uint16_t dummy;
	if (!paramPtr)
    {
        return status_ADC16_InValidArgument;
    }
	adc16_Hal_StartCalibration(true);
	while(!adc16_Hal_GetConvCompletedFlag())
	{
		if(adc16_Hal_GetCalFailedFlag())
		{
			adc16_Hal_StartCalibration(false);
			return status_ADC16_Failed;	
		}
	}
	paramPtr->plusSideGainValue = adc16_Hal_GetPlusSideGainValue();
#if ADC16_DIFF_MODE_ENABLE
	paramPtr->minusSideGainValue = adc16_Hal_GetMinusSideGainValue();
#endif
	dummy = adc16_Hal_GetConvCompletedFlag();
	dummy = dummy;
	adc16_Hal_StartCalibration(false);
	return status_ADC16_Success;
}

adc16_Status_t ADC_SetCalibrationParam(adc_calibration_param_t *paramPtr)
{
	if (!paramPtr)
    {
        return status_ADC16_InValidArgument;
    }
	adc16_Hal_SetPlusSideGainValue(paramPtr->plusSideGainValue);
#if ADC16_DIFF_MODE_ENABLE
	adc16_Hal_SetMinusSideGainValue(paramPtr->minusSideGainValue);
#endif	
	return status_ADC16_Success;
}


#endif





