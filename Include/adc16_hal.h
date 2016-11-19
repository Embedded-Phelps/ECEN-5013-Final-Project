/***************************************************************************
 *
 *	Filename: 		adc16_hal.h
 *  Description:  	adc hardware abstraction layer header file
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/
#ifndef __ADC16_HAL_H
#define __ADC16_HAL_H

#include "includes.h"
#define ADC16_DIFF_MODE_ENABLE			(0)
#define ADC16_HARDWARE_AVERAGE_ENABLE	(1)
#define ADC16_DMA_ENABLE				(0)
#define ADC_CALIBRATION_ENABLE			(1)

/******************************************************************************
 * Enumerations.
 *****************************************************************************/

/*!
 * @brief ADC16 status return codes.
 */
 
typedef enum _adc16_status
{
	status_ADC16_Success			=	0U,	
	status_ADC16_InValidArgument	=	1U,
	status_ADC16_Failed				=	2U
}adc16_Status_t;

typedef enum _adc16_clk_divider
{
	adc16_ClkDividerOf1	=	0U,			// The divide ratio of input clock is 1
	adc16_ClkDividerOf2	=	1U,			// The divide ratio of input clock is 2 
	adc16_ClkDividerOf4	=	2U,			// The divide ratio of input clock is 4
	adc16_ClkDividerOf8	=	3U			// The divide ratio of input clock is 8	
}adc16_Clk_Divider_t;

typedef enum _adc16_resolution
{
	adc16_ResolutionOf8or9 	 =	0U,
	adc16_ResolutionOf12or13 =	1U,
	adc16_ResolutionOf10or11 =	2U,
	adc16_ResolutionOf16 	 =	3U,
#if ADC16_DIFF_MODE_ENABLE
	adc16_ResolutionOf9ForDiffMode = adc16_ResolutionOf8or9,
	adc16_ResolutionOf13ForDiffMode = adc16_ResolutionOf12or13,
	adc16_ResolutionOf11ForDiffMode = adc16_ResolutionOf10or11,
	adc16_ResolutionOf16ForDiffMode = adc16_ResolutionOf16,
#endif /* ADC16_DIFF_MODE_ENABLE */
	adc16_ResolutionOf8ForSingleEnded = adc16_ResolutionOf8or9,
	adc16_ResolutionOf12ForSingleEnded = adc16_ResolutionOf12or13,
	adc16_ResolutionOf10ForSingleEnded = adc16_ResolutionOf10or11,
	adc16_ResolutionOf16ForSingleEnded = adc16_ResolutionOf16
}adc16_Resolution_Mode_t;

typedef enum _adc16_clk_src
{
	adc16_ClkSrcOfBusClk		=	0U,
	adc16_ClkSrcOfBusClkOver2 	=	1U,
	adc16_ClkSrcOfAltClk		=	2U,
	adc16_ClkSrcOfAdaClk		=	3U
}adc16_Clk_Src_t;

typedef enum _adc16_long_sample_cycle
{
	adc16_LongSampleCycleOf24	=	0U,
	adc16_LongSampleCycleOf16	=	1U,
	adc16_LongSampleCycleOf10	=	2U,
	adc16_LongSampleCycleOf6	=	3U,
}adc16_Long_Sample_Cycle_t;

typedef enum _adc16_vref_src
{
	adc16_VrefOfDefaultVref		=	0U,
	adc16_VrefOfAlternativeValt	=	1U
}adc16_Vref_Src_t;

#if ADC16_HARDWARE_AVERAGE_ENABLE

typedef enum _adc16_hw_average_sample
{
	adc16_HWAverageSampleOf4 	=	0U,
	adc16_HWAverageSampleOf8 	=	1U,
	adc16_HWAverageSampleOf16	=	2U,
	adc16_HWAverageSampleOf32	=	3U
}adc16_HW_Average_Samples_t;

#endif /* ADC16_HARDWARE_AVERAGE_ENABLE */

typedef enum _adc16_channel
{
	adc16_Ch0	=	0U,
	adc16_Ch1	=	1U,
	adc16_Ch2	=	2U,
	adc16_Ch3	=	3U,
	adc16_Ch4	=	4U,
	adc16_Ch5	=	5U,
	adc16_Ch6	=	6U,
	adc16_Ch7	=	7U,
	adc16_Ch8	=	8U,
	adc16_Ch9	=	9U,
	adc16_Ch10	=	10U,
	adc16_Ch11	=	11U,
	adc16_Ch12	=	12U,
	adc16_Ch13	=	13U,
	adc16_Ch14	=	14U,
	adc16_Ch15	=	15U,
	adc16_Ch16	=	16U,
	adc16_Ch17	=	17U,
	adc16_Ch18	=	18U,
	adc16_Ch19	=	19U,
	adc16_Ch20	=	20U,
	adc16_Ch21	=	21U,
	adc16_Ch22	=	22U,
	adc16_Ch23	=	23U,
	adc16_Ch24	=	24U,
	adc16_Ch25	=	25U,
	adc16_Ch26	=	26U,
	adc16_Ch27	=	27U,
	adc16_Ch28	=	28U,
	adc16_Ch29	=	29U,
	adc16_Ch30	=	30U,
	adc16_Ch31	=	31U,
	adc16_Dadp0 =	adc16_Ch0,
	adc16_Dadp1	=	adc16_Ch1,
	adc16_Dadp2	=	adc16_Ch2,
	adc16_Dadp3	=	adc16_Ch3,
	adc16_TempSensor = adc16_Ch26,
	adc16_Bandgap	 = adc16_Ch27,
	adc16_Vrefh		 = adc16_Ch29,
	adc16_Vrefl		 = adc16_Ch30,
	adc16_Ch_Disable = adc16_Ch31
}adc16_Ch_t;


/******************************************************************************
 * Definitions.
 *****************************************************************************/
 
typedef struct adc16_Ch_Config
{
	adc16_Ch_t ch_Idx;
	bool convCompleteIntEnable;
#if ADC16_DIFF_MODE_ENABLE
	bool diffModeEnable;
#endif /* ADC16_DIFF_MODE_ENABLE */
 }adc16_Ch_Config_t;

typedef struct adc16_Conv_Config
{
	bool lowPowerEnable;
	adc16_Clk_Divider_t clkDivider;
	bool longSampleEnable;
	adc16_Resolution_Mode_t resolution;
	adc16_Clk_Src_t clkSrc;
	bool asyncClkEnable;
	bool highSpeedEnable;
	adc16_Long_Sample_Cycle_t longSampleCycle;
	bool hwTriggerEnable;
	adc16_Vref_Src_t vref_Src;
	bool continuousConvEnable;
#if ADC16_HARDWARE_AVERAGE_ENABLE
	bool hwAverageEnable;
	adc16_HW_Average_Samples_t hwAverageSamples;
#endif /* ADC16_HARDWARE_AVERAGE_ENABLE */
#if ADC16_DMA_ENABLE
	bool dmaEnable;
#endif /* ADC16_DMA_ENABLE */
}adc16_Conv_Config_t;

typedef struct adc16_hw_cmp_config
{
	bool hwCmpEnable;
	bool hwCmpGreaterThanEnable;
	bool hwCmpRangeEnable;
	uint16_t cmpValue1;
	uint16_t cmpValue2;
}adc16_Hw_Cmp_Config_t;

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
__STATIC_INLINE void adc16_Hal_EnableClock(void)
{
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
}

__STATIC_INLINE void adc16_Hal_DisableClock(void)
{
	SIM_SCGC6 &= ~SIM_SCGC6_ADC0_MASK;
}

__STATIC_INLINE void adc16_Hal_EnableNVICInterrupt(void)
{
	NVIC_EnableIRQ(ADC0_IRQn);
}

__STATIC_INLINE void adc16_Hal_DisableNVICInterrupt(void)
{
	NVIC_DisableIRQ(ADC0_IRQn);
}

void adc16_Hal_Init(void);

void adc16_Hal_ConfigCh(const adc16_Ch_Config_t *configPtr);

void adc16_Hal_ConfigConv(const adc16_Conv_Config_t *configPtr);

__STATIC_INLINE bool adc16_Hal_GetConvCompletedFlag(void)
{
	return (ADC_SC1_COCO_MASK == (ADC0_SC1A & ADC_SC1_COCO_MASK)); 
}

__STATIC_INLINE uint16_t adc16_Hal_GetConvValue(void)
{
	return (uint16_t)(ADC0_RA);					
}

__STATIC_INLINE bool adc16_Hal_GetConvActiveFlag(void){
	return (ADC_SC2_ADACT_MASK == (ADC0_SC2 & ADC_SC2_ADACT_MASK));	
}

void adc16_Hal_ConfigHwCmp(const adc16_Hw_Cmp_Config_t *configPtr);

#if ADC_CALIBRATION_ENABLE
__STATIC_INLINE void adc16_Hal_StartCalibration(bool enable){
	ADC0_SC3 |= (enable << ADC_SC3_CAL_SHIFT);
}

__STATIC_INLINE bool adc16_Hal_GetCalActiveFlag(void){
	return (ADC_SC3_CAL_MASK == (ADC0_SC3 & ADC_SC3_CAL_MASK));
}

__STATIC_INLINE bool adc16_Hal_GetCalFailedFlag(void){
	return (ADC_SC3_CALF_MASK == (ADC0_SC3 & ADC_SC3_CALF_MASK));
}
				
uint16_t adc16_Hal_GetPlusSideGainValue(void);

__STATIC_INLINE void adc16_Hal_SetPlusSideGainValue(uint16_t value){
	ADC0_PG = value; 
}

#if ADC16_DIFF_MODE_ENABLE
uint16_t adc16_Hal_GetMinusSideGainValue(void);

__STATIC_INLINE void adc16_Hal_SetMinusSideGainValue(uint16_t value){
	ADC0_MG = value;
}

#endif /* ADC16_DIFF_MODE_ENABLE */

#endif /* ADC_CALIBRATION_ENABLE */

 
#ifdef __cplusplus
}
#endif


#endif /* __ADC16_HAL_H */
