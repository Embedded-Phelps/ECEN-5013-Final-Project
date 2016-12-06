#ifndef __TSI_HAL_H__
#define __TSI_HAL_H__

#include "includes.h"

#define MAX_TSI_CHANNEL_INDEX		(16)

typedef enum tsi_status
{
	status_TSI_Success = 0U,
	status_TSI_Busy,
	status_TSI_Lowpower,
	status_TSI_Recalibration,
	status_TSI_InvalidChannel,
	status_TSI_InvalidMode,
	status_TSI_Initialized,
	status_TSI_Error
}tsi_status_t;
 
typedef enum tsi_scan_time
{
	TSI_Scan_1time = 0U,
	TSI_Scan_2time = 1U,
	TSI_Scan_3time = 2U,
	TSI_Scan_4time = 3U,
	TSI_Scan_5time = 4U,
	TSI_Scan_6time = 5U,
	TSI_Scan_7time = 6U,
	TSI_Scan_8time = 7U,
	TSI_Scan_9time = 8U,
	TSI_Scan_10time = 9U,
	TSI_Scan_11time = 10U,
	TSI_Scan_12time = 11U,
	TSI_Scan_13time = 12U,
	TSI_Scan_14time = 13U,
	TSI_Scan_15time = 14U,
	TSI_Scan_16time = 15U,
	TSI_Scan_17time = 16U,
	TSI_Scan_18time = 17U,
	TSI_Scan_19time = 18U,
	TSI_Scan_20time = 19U,
	TSI_Scan_21time = 20U,
	TSI_Scan_22time = 21U,
	TSI_Scan_23time = 22U,
	TSI_Scan_24time = 23U,
	TSI_Scan_25time = 24U,
	TSI_Scan_26time = 25U,
	TSI_Scan_27time = 26U,
	TSI_Scan_28time = 27U,
	TSI_Scan_29time = 28U,
	TSI_Scan_30time = 29U,
	TSI_Scan_31time = 30U,
	TSI_Scan_32time = 31U,
}tsi_scan_time_t;

typedef struct tsi_scan_time_limit
{
	tsi_scan_time_t upper;
	tsi_scan_time_t lower;
}tsi_scan_time_limit_t;

typedef enum tsi_electrode_osc_prescaler {
    TSIElecOscPrescaler_1div = 0,          /*!< Electrode oscillator frequency divided by 1 */
    TSIElecOscPrescaler_2div = 1,          /*!< Electrode oscillator frequency divided by 2 */
    TSIElecOscPrescaler_4div = 2,          /*!< Electrode oscillator frequency divided by 4 */
    TSIElecOscPrescaler_8div = 3,          /*!< Electrode oscillator frequency divided by 8 */
    TSIElecOscPrescaler_16div = 4,         /*!< Electrode oscillator frequency divided by 16 */
    TSIElecOscPrescaler_32div = 5,         /*!< Electrode oscillator frequency divided by 32 */
    TSIElecOscPrescaler_64div = 6,         /*!< Electrode oscillator frequency divided by 64 */
    TSIElecOscPrescaler_128div = 7,        /*!< Electrode oscillator frequency divided by 128 */
} tsi_electrode_osc_prescaler_t;

typedef enum tsi_analog_mode_select {
    TSIAnalogModeSel_Capacitive = 0,     /*!< Active TSI capacitive sensing mode */
    TSIAnalogModeSel_NoiseNoFreqLim = 4, /*!< TSI works in single threshold noise detection mode and the freq. limitation
is disabled */
    TSIAnalogModeSel_NoiseFreqLim = 8,   /*!< TSI analog works in single threshold noise detection mode and the freq. limitation
is enabled */
    TSIAnalogModeSel_AutoNoise = 12,     /*!/ Active TSI analog in automatic noise detection mode */
} tsi_analog_mode_select_t;

typedef enum tsi_reference_osc_charge_current {
    TSIRefOscChargeCurrent_500nA = 0, /*!< Reference oscillator charge current is 500nA */
    TSIRefOscChargeCurrent_1uA = 1,   /*!< Reference oscillator charge current is 1uA */
    TSIRefOscChargeCurrent_2uA = 2,   /*!< Reference oscillator charge current is 2uA */
    TSIRefOscChargeCurrent_4uA = 3,   /*!< Reference oscillator charge current is 4uA */
    TSIRefOscChargeCurrent_8uA = 4,   /*!< Reference oscillator charge current is 8uA */
    TSIRefOscChargeCurrent_16uA = 5,  /*!< Reference oscillator charge current is 16uA */
    TSIRefOscChargeCurrent_32uA = 6,  /*!< Reference oscillator charge current is 32uA */
    TSIRefOscChargeCurrent_64uA = 7,  /*!< Reference oscillator charge current is 64uA */
} tsi_reference_osc_charge_current_t;

typedef struct tsi_reference_osc_charge_current_limits
{
  tsi_reference_osc_charge_current_t    upper;  /*!< Reference oscillator charge current upper limit */
  tsi_reference_osc_charge_current_t    lower;  /*!< Reference oscillator charge current lower limit */
}tsi_reference_osc_charge_current_limits_t;

typedef enum tsi_external_osc_charge_current {
    TSIExtOscChargeCurrent_500nA = 0,  /*!< External oscillator charge current is 500nA */
    TSIExtOscChargeCurrent_1uA = 1,    /*!< External oscillator charge current is 1uA */
    TSIExtOscChargeCurrent_2uA = 2,    /*!< External oscillator charge current is 2uA */
    TSIExtOscChargeCurrent_4uA = 3,    /*!< External oscillator charge current is 4uA */
    TSIExtOscChargeCurrent_8uA = 4,    /*!< External oscillator charge current is 8uA */
    TSIExtOscChargeCurrent_16uA = 5,   /*!< External oscillator charge current is 16uA */
    TSIExtOscChargeCurrent_32uA = 6,   /*!< External oscillator charge current is 32uA */
    TSIExtOscChargeCurrent_64uA = 7,   /*!< External oscillator charge current is 64uA */
} tsi_external_osc_charge_current_t;

typedef struct tsi_external_osc_charge_current_limits
{
  tsi_external_osc_charge_current_t    upper;  /*!< External oscillator charge current upper limit */
  tsi_external_osc_charge_current_t    lower;  /*!< External oscillator charge current lower limit */
}tsi_external_osc_charge_current_limits_t;

typedef enum tsi_oscilator_voltage_rails {
    TSIOscVolRails_Dv_103 = 0,        /*!< DV = 1.03 V; VP = 1.33 V; Vm = 0.30 V */
    TSIOscVolRails_Dv_073 = 1,        /*!< DV = 0.73 V; VP = 1.18 V; Vm = 0.45 V */
    TSIOscVolRails_Dv_043 = 2,        /*!< DV = 0.43 V; VP = 1.03 V; Vm = 0.60 V */
    TSIOscVolRails_Dv_029 = 3,        /*!< DV = 0.29 V; VP = 0.95 V; Vm = 0.67 V */
} tsi_oscilator_voltage_rails_t;

typedef enum tsi_channel_number {
    TSIChannelNumber_0 = 0,       /*!< Channel Number 0 */
    TSIChannelNumber_1 = 1,       /*!< Channel Number 1 */
    TSIChannelNumber_2 = 2,       /*!< Channel Number 2 */
    TSIChannelNumber_3 = 3,       /*!< Channel Number 3 */
    TSIChannelNumber_4 = 4,       /*!< Channel Number 4 */
    TSIChannelNumber_5 = 5,       /*!< Channel Number 5 */
    TSIChannelNumber_6 = 6,       /*!< Channel Number 6 */
    TSIChannelNumber_7 = 7,       /*!< Channel Number 7 */
    TSIChannelNumber_8 = 8,       /*!< Channel Number 8 */
    TSIChannelNumber_9 = 9,       /*!< Channel Number 9 */
    TSIChannelNumber_10 = 10,     /*!< Channel Number 10 */
    TSIChannelNumber_11 = 11,     /*!< Channel Number 11 */
    TSIChannelNumber_12 = 12,     /*!< Channel Number 12 */
    TSIChannelNumber_13 = 13,     /*!< Channel Number 13 */
    TSIChannelNumber_14 = 14,     /*!< Channel Number 14 */
    TSIChannelNumber_15 = 15,     /*!< Channel Number 15 */
} tsi_channel_number_t;

typedef struct tsi_config
{
	tsi_electrode_osc_prescaler_t ps;
	tsi_external_osc_charge_current_t exchargecurrent;
	tsi_reference_osc_charge_current_t refchargecurrent;
	tsi_scan_time_t nscan;
	tsi_analog_mode_select_t mode;
	tsi_oscilator_voltage_rails_t dvolt;
	uint16_t thresh;
	uint16_t thresl;
}tsi_config_t;



#ifdef __cplusplus
extern "C" {
#endif

void TSI_Hal_Init(void);

void TSI_Hal_SetConfiguration(tsi_config_t *config);

//uint32_t TSI_Hal_Recalibration(tsi_config_t *config, const uint32_t electrodes, const tsi_parameter_limit_t parameterLimits);

void TSI_Hal_EnableLowPower(void);

void TSI_Hal_DisableLowPower(void);


__STATIC_INLINE uint32_t TSI_Hal_IsModuleEnabled(void)
{
	return (TSI0_GENCS & TSI_GENCS_TSIEN_MASK);
}

__STATIC_INLINE uint32_t TSI_Hal_GetScanTriggerMode(void)
{
	return (TSI0_GENCS & TSI_GENCS_STM_MASK);
}

__STATIC_INLINE uint32_t TSI_Hal_IsScanInProgress(void)
{
	return (TSI0_GENCS & TSI_GENCS_SCNIP_MASK);
}

__STATIC_INLINE uint32_t TSI_Hal_GetEndOfScanFlag(void)
{
	return (TSI0_GENCS & TSI_GENCS_EOSF_MASK);
}

__STATIC_INLINE uint32_t TSI_Hal_GetOutOfRangeFlag(void)
{
	return (TSI0_GENCS & TSI_GENCS_OUTRGF_MASK);
}

__STATIC_INLINE uint32_t TSI_Hal_IsInterruptEnabled(void)
{
	return (TSI0_GENCS & TSI_GENCS_TSIIEN_MASK);
}

__STATIC_INLINE uint32_t TSI_Hal_GetEnableStop(void)
{
	return (TSI0_GENCS & TSI_GENCS_STPE_MASK);
}

__STATIC_INLINE uint32_t TSI_Hal_IsCurrentSourcePairSwapped(void)
{
	return (TSI0_GENCS & TSI_GENCS_CURSW_MASK);
}

__STATIC_INLINE uint32_t TSI_Hal_GetMeasuredChannelNumber(void)
{
	return (uint32_t)((TSI0_DATA & TSI_DATA_TSICH_MASK) >> TSI_DATA_TSICH_SHIFT);
}

__STATIC_INLINE uint32_t TSI_Hal_IsDmaTransferEnable(void)
{
	return (TSI0_DATA & TSI_DATA_DMAEN_MASK);
}

__STATIC_INLINE uint32_t TSI_Hal_GetCounter(void)
{
	return (TSI0_DATA & TSI_DATA_TSICNT_MASK);
}

__STATIC_INLINE tsi_analog_mode_select_t TSI_Hal_GetMode(void)
{
	return (tsi_analog_mode_select_t) (TSI0_GENCS & TSI_GENCS_MODE_MASK);
}

__STATIC_INLINE tsi_reference_osc_charge_current_t TSI_Hal_GetRefChargeCurrent(void)
{
	return (tsi_reference_osc_charge_current_t) (TSI0_GENCS & TSI_GENCS_REFCHRG_MASK);
}

__STATIC_INLINE tsi_oscilator_voltage_rails_t TSI_Hal_GetOscilatorVoltRails(void)
{
	return (tsi_oscilator_voltage_rails_t) (TSI0_GENCS & TSI_GENCS_DVOLT_MASK);
}

__STATIC_INLINE tsi_external_osc_charge_current_t TSI_Hal_GetElectrodeChargeCurrent(void)
{
	return (tsi_external_osc_charge_current_t) (TSI0_GENCS & TSI_GENCS_EXTCHRG_MASK);
}

__STATIC_INLINE void TSI_Hal_EnableClock(void)
{
	SIM_SCGC5 |= SIM_SCGC5_TSI_MASK; 
}

__STATIC_INLINE void TSI_Hal_DisableClock(void)
{
	SIM_SCGC5 &= ~SIM_SCGC5_TSI_MASK; 
}

__STATIC_INLINE void TSI_Hal_EnableOutOfRangeInterrupt(void)
{
	TSI0_GENCS  &= ~TSI_GENCS_ESOR_MASK;
}

__STATIC_INLINE void TSI_Hal_EnableEndOfScanInterrupt(void)
{
	TSI0_GENCS  |= TSI_GENCS_ESOR_MASK;
}

__STATIC_INLINE void TSI_Hal_EnableModule(void)
{
	TSI0_GENCS  |= TSI_GENCS_TSIEN_MASK;
}

__STATIC_INLINE void TSI_Hal_DisableModule(void)
{
	TSI0_GENCS  &= ~TSI_GENCS_TSIEN_MASK;
}

__STATIC_INLINE void TSI_Hal_EnableNVICInterrupt(void)
{
	NVIC_EnableIRQ(TSI0_IRQn);
}

__STATIC_INLINE void TSI_Hal_DisableNVICInterrupt(void)
{
	NVIC_DisableIRQ(TSI0_IRQn);
}

__STATIC_INLINE void TSI_Hal_EnableInterrupt(void)
{
	TSI0_GENCS  |= TSI_GENCS_TSIIEN_MASK;
}


__STATIC_INLINE void TSI_Hal_DisableInterrupt(void)
{
	TSI0_GENCS  &= ~TSI_GENCS_TSIIEN_MASK;
}

__STATIC_INLINE void TSI_Hal_EnableStop(void)
{
	TSI0_GENCS  |= TSI_GENCS_STPE_MASK;
}

__STATIC_INLINE void TSI_Hal_DisableStop(void)
{
	TSI0_GENCS  &= ~TSI_GENCS_STPE_MASK;
}

__STATIC_INLINE void TSI_Hal_EnableHardwareTriggerScan(void)
{
	TSI0_GENCS  |= TSI_GENCS_STM_MASK;
}

__STATIC_INLINE void TSI_Hal_EnableSoftwareTriggerScan(void)
{
	TSI0_GENCS  &= ~TSI_GENCS_STM_MASK;
}

__STATIC_INLINE void TSI_Hal_CurrentSourcePariSwapped(void)
{
	TSI0_GENCS  |= TSI_GENCS_CURSW_MASK;
}

__STATIC_INLINE void TSI_Hal_CurrentSourcePariNotSwapped(void)
{
	TSI0_GENCS  &= ~TSI_GENCS_CURSW_MASK;
}

__STATIC_INLINE void TSI_Hal_ClearOutOfRangeFlag(void)
{
	TSI0_GENCS  |= TSI_GENCS_OUTRGF_MASK;
}

__STATIC_INLINE void TSI_Hal_ClearEndOfScanFlag(void)
{
	TSI0_GENCS  |= TSI_GENCS_EOSF_MASK;
}

__STATIC_INLINE void TSI_Hal_SetPrescaler(tsi_electrode_osc_prescaler_t prescaler)
{
	TSI0_GENCS  &= ~TSI_GENCS_PS_MASK;
	TSI0_GENCS  |= TSI_GENCS_PS(prescaler);
}

__STATIC_INLINE void TSI_Hal_SetNumberOfScan(tsi_scan_time_t nscan)
{
	TSI0_GENCS  &= ~TSI_GENCS_NSCN_MASK;
	TSI0_GENCS  |= TSI_GENCS_NSCN(nscan);
}

__STATIC_INLINE void TSI_Hal_SetMeasureChannelNumber(uint32_t channel)
{
	assert(channel < MAX_TSI_CHANNEL_INDEX);
	TSI0_DATA  &= 0x0FFFFFFF;
	TSI0_DATA  |= TSI_DATA_TSICH(channel);
}

__STATIC_INLINE void TSI_Hal_DmaTransferEnable(void)
{
	TSI0_DATA  |= TSI_DATA_DMAEN_MASK;
}

__STATIC_INLINE void TSI_Hal_DmaTransferDisable(void)
{
	TSI0_DATA  &= ~TSI_DATA_DMAEN_MASK;
}

__STATIC_INLINE void TSI_Hal_StartSoftwareTrigger(void)
{
	TSI0_DATA  |= TSI_DATA_SWTS_MASK;
}

__STATIC_INLINE void TSI_Hal_SetLowThreshold(uint32_t thresl)
{
	assert(thresl <65535U);
	TSI0_TSHD |= TSI_TSHD_THRESL(thresl); 
}

__STATIC_INLINE void TSI_Hal_SetHighThreshold(uint32_t thresh)
{
	assert(thresh <65535U);
	TSI0_TSHD |= TSI_TSHD_THRESH(thresh); 
}

__STATIC_INLINE void TSI_Hal_SetMode(tsi_analog_mode_select_t mode)
{
	TSI0_GENCS &= ~TSI_GENCS_MODE_MASK;
	TSI0_GENCS |= TSI_GENCS_MODE(mode);
}

__STATIC_INLINE void TSI_Hal_SetRefChargeCurrent(tsi_reference_osc_charge_current_t current)
{
	TSI0_GENCS &= ~TSI_GENCS_REFCHRG_MASK;
	TSI0_GENCS |= TSI_GENCS_REFCHRG(current);
}

__STATIC_INLINE void TSI_Hal_SetOscilatorVoltRails(tsi_oscilator_voltage_rails_t dvolt)
{
	TSI0_GENCS &= ~TSI_GENCS_DVOLT_MASK;
	TSI0_GENCS |= TSI_GENCS_DVOLT(dvolt);
}

__STATIC_INLINE void TSI_Hal_SetElectrodeChargeCurrent(tsi_external_osc_charge_current_t current)
{
	TSI0_GENCS &= ~TSI_GENCS_EXTCHRG_MASK;
	TSI0_GENCS |= TSI_GENCS_EXTCHRG(current);
}


#ifdef __cplusplus
}
#endif

#endif
