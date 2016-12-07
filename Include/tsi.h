#ifndef __TSI_H__
#define __TSI_H__

#include "includes.h"
#include "tsi_hal.h"

typedef void (*tsi_callback_t)(void* usrData);

typedef struct tsi_user_config
{
	tsi_config_t *config;
	tsi_callback_t callback;
	void *usrData;
}tsi_user_config_t;


typedef enum tsi_mode
{
  tsi_OpModeNormal = 0U,        /**< The normal mode of TSI. */
  tsi_OpModeProximity,         /**< The proximity sensing mode of TSI. */
  tsi_OpModeLowPower,          /**< The low power mode of TSI. */
  tsi_OpModeNoise,             /**< The noise mode of TSI. This mode is not valid with TSI HW, valid only for the TSIL HW. */
  tsi_OpModeCnt,               /**< Count of TSI modes - for internal use. */
  tsi_OpModeNoChange           /**< The special value of operation mode that allows call for example @ref TSI_DRV_DisableLowPower function without change of operation mode. */
}tsi_mode_t;

typedef struct tsi_operation_mode
{
	uint16_t enabledElectrodes;
	tsi_config_t config;
}tsi_operation_mode_t;

typedef struct tsi_state
{
	tsi_status_t status;
	tsi_callback_t pCallbackFunc;
	void *usrData;
	semaphore_t irqSync;
	mutex_t lock;
	mutex_t lockChangeMode;
	bool isBlockingMeasure;
	tsi_mode_t operationMode;
	tsi_operation_mode_t opModeData[tsi_OpModeCnt];
	uint16_t counters[MAX_TSI_CHANNEL_INDEX];
}tsi_state_t;

extern tsi_state_t *tsiStatePtr;

#if defined(__cplusplus)
extern "C" {
#endif

tsi_status_t TSI_Init(tsi_state_t *tsiState, const tsi_user_config_t *tsiUserConfig);

tsi_status_t TSI_DeInit(void);

tsi_status_t TSI_EnableElectrode(const uint32_t channel, const bool enable);

uint32_t TSI_GetEnableElectrode(void);

tsi_status_t TSI_Measure(void);

tsi_status_t TSI_MeasureBlocking(void);

tsi_status_t TSI_AbortMeasure(void);

tsi_status_t TSI_GetCounter(const uint32_t channel, uint16_t *counter);

tsi_status_t TSI_GetStatus(void);

//tsi_status_t TSI_EnbaleLowPower(uint32_t instance);

//tsi_status_t TSI_DisableLowPower(uint32_t instance, const tsi_mode_t mode);

//tsi_status_t TSI_Recalibrate(uint32_t instance, uint32_t *lowestSignal);

tsi_status_t TSI_SetCallbackFunc(const tsi_callback_t pCallbackFunc, void *usrData);

//tsi_status_t TSI_ChangeMode(uint32_t instance, const tsi_mode_t mode);

//tsi_mode_t TSI_GetMode(uint32_t instance);

//tsi_status_t TSI_LoadConfiguration(uint32_t instance, const tsi_mode_t mode, const tsi_operation_mode_t *operationMode);

//tsi_status_t TSI_SaveConfiguration(uint32_t instance, const tsi_mode_t mode, tsi_operation_mode_t *operationMode);

uint32_t TSI_GetUnTouchBaseline(uint8_t * tsiChn);

#if defined(__cplusplus)
}
#endif

#endif
