#ifndef __ADC16_DRIVER_H__
#define __ADC16_DRIVER_H__

#include "includes.h"

typedef enum _adc_flag
{
	adcConvActiveFlag	=	0U,
#if ADC_CALIBRATION_ENABLE
	adcCalFailedFlag	=	1U,
	adcCalActiveFlag	=	2U,
#endif /* ADC_CALIBRATION_ENABLE */
	adcConvCompleteFlag	=	3U
}adc_flag_t;

#if ADC_CALIBRATION_ENABLE
typedef struct adc_calibration_param
{
	uint16_t plusSideGainValue;
#if ADC16_DIFF_MODE_ENABLE
	uint16_t minusSideGainValue;
#endif /* ADC16_DIFF_MODE_ENABLE */
}adc_calibration_param_t;
#endif /* ADC_CALIBRATION_ENABLE */

/*! @brief Bandgap Buffer configuration. */
typedef struct _pmc_bandgap_buffer_config
{
    bool enable;                             /*!< Enable bandgap buffer.                   */
#if FSL_FEATURE_PMC_HAS_BGEN
    bool enableInLowPower;                   /*!< Enable bandgap buffer in low power mode. */
#endif
#if FSL_FEATURE_PMC_HAS_BGBDS
    pmc_bandgap_buffer_drive_select_t drive; /*!< Bandgap buffer drive select.             */
#endif
} pmc_bandgap_buffer_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/******************************************************************************
 * API
 *****************************************************************************/
//fill the default configuration for a one-time trigger mode
adc16_Status_t ADC_ConfigDefaultMode(adc16_Conv_Config_t *userConfigPtr);
 
//Initialize the ADC converter module
adc16_Status_t ADC_Init(const adc16_Conv_Config_t *userConfigPtr);
 
//Disable the ADC module
adc16_Status_t ADC_Disable(void);
 
//Configure the convertion channel by software
adc16_Status_t ADC_ConfigCh(adc16_Ch_Config_t *configPtr);
 
void ADC_WaitForConvComplete(void);
 
void ADC_PauseConv(void);

uint16_t ADC_GetConvValueRaw(void);
 
int16_t ADC_GetConvValueSigned(void);
 
bool ADC_GetConvFlag(adc_flag_t flag);
 
#if ADC_CALIBRATION_ENABLE
adc16_Status_t ADC_GetCalibrationParam(adc_calibration_param_t *paramPtr);

adc16_Status_t ADC_SetCalibrationParam(adc_calibration_param_t *paramPtr);

#endif

__STATIC_INLINE void PMC_HAL_BandgapBufferConfig(pmc_bandgap_buffer_config_t *config)
{
	PMC_REGSC = PMC_REGSC_BGBE(config->enable)
#if FSL_FEATURE_PMC_HAS_BGEN
                     | PMC_REGSC_BGEN(config->enableInLowPower)
#endif
#if FSL_FEATURE_PMC_HAS_BGBDS
                     | PMC_REGSC_BGBDS(config->drive)
#endif
                     ;
}


#ifdef __cplusplus
}
#endif

#endif
