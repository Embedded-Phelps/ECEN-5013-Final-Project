#include "includes.h"
///////////////////////////////////////////////////////////////////////////////
// Variables
///////////////////////////////////////////////////////////////////////////////
uint32_t adcValue = 0;               // ADC value
uint32_t adcrTemp25 = 0;             // Calibrated ADCR_TEMP25
uint32_t adcr100m = 0;               // calibrated conversion value of 100mV

adc16_Conv_Config_t adcUserConfig;

void app_ADCInit(void)
{
	ADC_ConfigDefaultMode(&adcUserConfig);
#if ADC16_HARDWARE_AVERAGE_ENABLE
	adcUserConfig.hwAverageEnable = true;	
#endif
	ADC_Init(&adcUserConfig);
	temp_CalibrateParam();
}

void temp_CalibrateParam(void)
{
#if ADC_CALIBRATION_ENABLE
	adc_calibration_param_t adcCalibrationParam;
#endif
	uint32_t bandgapValue = 0;
	uint32_t vdd = 0;
	adc16_Ch_Config_t adcChConfig;
	pmc_bandgap_buffer_config_t pmcBandgapConfig;
	pmcBandgapConfig.enable = true;
#if FSL_FEATURE_PMC_HAS_BGEN
  pmcBandgapConfig.enableInLowPower = false,
#endif
#if FSL_FEATURE_PMC_HAS_BGBDS
  pmcBandgapConfig.drive = kPmcBandgapBufferDriveLow,
#endif
#if ADC_CALIBRATION_ENABLE
	ADC_GetCalibrationParam(&adcCalibrationParam);
	ADC_SetCalibrationParam(&adcCalibrationParam);
#endif
	// Enable BANDGAP reference voltage
    PMC_HAL_BandgapBufferConfig(&pmcBandgapConfig);
	adcChConfig.ch_Idx	= adc16_Bandgap;
#if ADC16_DIFF_MODE_ENABLE
	adcChConfig.diffModeEnable = false;
#endif
	adcChConfig.convCompleteIntEnable = false;
	ADC_ConfigCh(&adcChConfig);
	ADC_WaitForConvComplete();
	bandgapValue = ADC_GetConvValueSigned();
	ADC_PauseConv();
	// Get VDD value measured in mV
    // VDD = (ADCR_VDD x V_BG) / ADCR_BG
    vdd = ADCR_VDD * V_BG / bandgapValue;
    // Calibrate ADCR_TEMP25
    // ADCR_TEMP25 = ADCR_VDD x V_TEMP25 / VDD
    adcrTemp25 = ADCR_VDD * V_TEMP25 / vdd;
    // Calculate conversion value of 100mV.
    // ADCR_100M = ADCR_VDD x 100 / VDD
    adcr100m = ADCR_VDD*100/ vdd;

    // Disable BANDGAP reference voltage
    pmcBandgapConfig.enable = false;
    PMC_HAL_BandgapBufferConfig(&pmcBandgapConfig);
}

int32_t read_OnChipTemperature(void)
{
	uint32_t adcValue = 0;
	int32_t currentTemp = 0;
	adc16_Ch_Config_t adcChConfig;
	adcChConfig.ch_Idx	= adc16_TempSensor;
#if ADC16_DIFF_MODE_ENABLE
	adcChConfig.diffModeEnable = false;
#endif
	adcChConfig.convCompleteIntEnable = false;
	ADC_ConfigCh(&adcChConfig);
	ADC_WaitForConvComplete();
	adcValue = ADC_GetConvValueSigned();
	currentTemp = (int32_t)(STANDARD_TEMP - ((int32_t)adcValue - (int32_t)adcrTemp25) * 100000 /(int32_t)(adcr100m*M));
	ADC_PauseConv();
	return currentTemp;
}
