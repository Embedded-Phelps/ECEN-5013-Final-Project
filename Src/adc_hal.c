#include "includes.h"

void adc16_Hal_Init(void){
	ADC0_CFG1	=	0U;
	ADC0_CFG2	=	0U;
	ADC0_CV1	=	0U;
	ADC0_CV2	=	0U;
	ADC0_SC2	=	0U;
	ADC0_SC3	=	0U;
}

void adc16_Hal_ConfigCh(const adc16_Ch_Config_t *configPtr){
	uint16_t sc1;
	if(configPtr->convCompleteIntEnable){
		sc1 |= ADC_SC1_AIEN_MASK;
	}
#if ADC16_DIFF_MODE_ENABLE
	if(configPtr->diffModeEnable){
		sc1 |= ADC_SC1_DIFF_MASK;
	}
#endif /* ADC16_DIFF_MODE_ENABLE */
	sc1 |= ADC_SC1_ADCH((uint32_t)(configPtr->ch_Idx));
	ADC0_SC1A = sc1;
}

void adc16_Hal_ConfigConv(const adc16_Conv_Config_t *configPtr){
	uint16_t cfg1, cfg2, sc2, sc3;
	cfg1  = ADC0_CFG1;
    cfg1 &= ~(  ADC_CFG1_ADLPC_MASK
              | ADC_CFG1_ADIV_MASK
              | ADC_CFG1_ADLSMP_MASK
              | ADC_CFG1_MODE_MASK
              | ADC_CFG1_ADICLK_MASK );

    /* Low power mode. */
    if (configPtr->lowPowerEnable)
    {
        cfg1 |= ADC_CFG1_ADLPC_MASK;
    }
    /* Clock divider. */
    cfg1 |= ADC_CFG1_ADIV(configPtr->clkDivider);
    /* Long sample time. */
    if (configPtr->longSampleEnable)
    {
        cfg1 |= ADC_CFG1_ADLSMP_MASK;
    }
    /* Sample resolution mode. */
    cfg1 |= ADC_CFG1_MODE(configPtr->resolution);
    /* Clock source input. */
    cfg1 |= ADC_CFG1_ADICLK(configPtr->clkSrc);

    cfg2 = ADC0_CFG2;
    cfg2 &= ~( ADC_CFG2_ADACKEN_MASK
             | ADC_CFG2_ADHSC_MASK
             | ADC_CFG2_ADLSTS_MASK );
    /* Asynchronous clock output enable. */
    if (configPtr->asyncClkEnable)
    {
        cfg2 |= ADC_CFG2_ADACKEN_MASK;
    }
    /* High speed configuration. */
    if (configPtr->highSpeedEnable)
    {
        cfg2 |= ADC_CFG2_ADHSC_MASK;
    }
    /* Long sample time select. */
    cfg2 |= ADC_CFG2_ADLSTS(configPtr->longSampleCycle);

    sc2 = ADC0_SC2;
    sc2 &= ~( ADC_SC2_ADTRG_MASK
            | ADC_SC2_REFSEL_MASK
#if ADC16_DMA_ENABLE
            | ADC_SC2_DMAEN_MASK
#endif /* ADC16_DMA_ENABLE */
			);
    /* Conversion trigger select. */
    if (configPtr->hwTriggerEnable)
    {
        sc2 |= ADC_SC2_ADTRG_MASK;
    }
    /* Voltage reference selection. */
    sc2 |= ADC_SC2_REFSEL(configPtr->vref_Src);
#if ADC16_DMA_ENABLE
    /* DMA. */
    if (configPtr->dmaEnable)
    {
        sc2 |= ADC_SC2_DMAEN_MASK;
    }
#endif /* ADC16_DMA_ENABLE */

    sc3 = ADC0_SC3;
    sc3 &= ~( ADC_SC3_ADCO_MASK
            | ADC_SC3_CALF_MASK
#if ADC16_HARDWARE_AVERAGE_ENABLE
			| ADC_SC3_AVGE_MASK
            | ADC_SC3_AVGS_MASK
#endif /* ADC16_HARDWARE_AVERAGE_ENABLE */
			);
    /* Continuous conversion enable. */
    if (configPtr->continuousConvEnable)
    {
        sc3 |= ADC_SC3_ADCO_MASK;
    }
#if ADC16_HARDWARE_AVERAGE_ENABLE
	/* Hardware average enable. */
	if (configPtr->hwAverageEnable)
    {
        sc3 |= ADC_SC3_AVGE_MASK;
    }
	sc3 |= ADC_SC3_AVGS(configPtr->hwAverageSamples);
#endif /* ADC16_HARDWARE_AVERAGE_ENABLE */
    ADC0_CFG1 = cfg1;
    ADC0_CFG2 = cfg2;
    ADC0_SC2  = sc2;
    ADC0_SC3  = sc3;
}

void adc16_Hal_ConfigHwCmp(const adc16_Hw_Cmp_Config_t *configPtr){
	uint16_t sc2;

    sc2 = ADC0_SC2;
    sc2 &= ~( ADC_SC2_ACFE_MASK
            | ADC_SC2_ACFGT_MASK
            | ADC_SC2_ACREN_MASK );
    /* Compare Function Enable. */
    if (configPtr->hwCmpEnable)
    {
        sc2 = ADC_SC2_ACFE_MASK;
    }
    /* Compare Function Greater Than Enable. */
    if (configPtr->hwCmpGreaterThanEnable)
    {
        sc2 |= ADC_SC2_ACFGT_MASK;
    }
    /* Compare Function Range Enable. */
    if (configPtr->hwCmpRangeEnable)
    {
        sc2 |= ADC_SC2_ACREN_MASK;
    }

    ADC0_SC2 =  sc2;
    ADC0_CV1 = ADC_CV1_CV(configPtr->cmpValue1);
    ADC0_CV2 = ADC_CV2_CV(configPtr->cmpValue2);
}

#if ADC_CALIBRATION_ENABLE
			
uint16_t adc16_Hal_GetPlusSideGainValue(void){
    uint16_t cal_var;
    /* Calculate plus-side calibration */
    cal_var = 0U;
    cal_var += ADC0_CLP0;
    cal_var += ADC0_CLP1;
    cal_var += ADC0_CLP2;
    cal_var += ADC0_CLP3;
    cal_var += ADC0_CLP4;
    cal_var += ADC0_CLPS;
    cal_var = 0x8000U | (cal_var>>1U);
    return cal_var;
}	

#if ADC16_DIFF_MODE_ENABLE
uint16_t adc16_Hal_GetMinusSideGainValue(void){
    uint16_t cal_var;
    /* Calculate minus-side calibration. */
    cal_var = 0U;
    cal_var += ADC0_CLM0;
    cal_var += ADC0_CLM1;
    cal_var += ADC0_CLM2;
    cal_var += ADC0_CLM3;
    cal_var += ADC0_CLM4;
    cal_var += ADC0_CLMS;
    cal_var = 0x8000U | (cal_var>>1U);
    return cal_var;
}
#endif /* ADC16_DIFF_MODE_ENABLE */

#endif /* ADC_CALIBRATION_ENABLE */

