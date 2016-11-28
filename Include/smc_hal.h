/***************************************************************************
 *
 *	Filename: 		smc_hal.h
 *  Description:  	primary header file for the project
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/
 
#ifndef  __SMC_HAL_H
#define  __SMC_HAL_H

#include "includes.h"

typedef enum power_modes
{
	PowerModeRun 	= (0x01U << 0U),
	PowerModeWait 	= (0x01U << 1U),
	PowerModeStop 	= (0x01U << 2U),
	PowerModeVLPR 	= (0x01U << 3U),
	PowerModeVLPW 	= (0x01U << 4U),
	PowerModeVLPS 	= (0x01U << 5U),
	PowerModeLLS 	= (0x01U << 6U),
	PowerModeVLLS 	= (0x01U << 7U),
}power_modes_t;

typedef enum smc_error
{
	smcSuccess,
	smcNoSuchMode,
	smcAlreadyInTheState,
	smcFailed
}smc_error_t;

typedef enum power_mode_status
{
	statusRun 	= (0x01U << 0U),
	statusWait 	= (0x01U << 1U),
	statusStop 	= (0x01U << 2U),
	statusVLPR 	= (0x01U << 3U),
	statusVLPW 	= (0x01U << 4U),
	statusVLPS 	= (0x01U << 5U),
	statusLLS 	= (0x01U << 6U),
	statusVLLS 	= (0x01U << 7U),
}power_mode_status_t;

typedef enum power_modes_protect
{
	allowPowerModeVLP	=	SMC_PMPROT_AVLP_MASK,
	allowPowerModeLLS	=	SMC_PMPROT_ALLS_MASK,
	allowPowerModeVLLS	=	SMC_PMPROT_AVLLS_MASK,
	allowPowerModeAll	=	SMC_PMPROT_AVLP_MASK | SMC_PMPROT_ALLS_MASK | SMC_PMPROT_AVLLS_MASK
}power_modes_protect_t;

typedef enum smc_run_mode
{
	smcRun	=	0U,
	smcVLPR	=	2U
}smc_run_mode_t;

typedef enum smc_stop_mode
{
	smcStop	=	0U,
	smcVLPS	=	2U,
	smcLLS	=	3U,
	smcVLLS	=	4U
}smc_stop_mode_t;

typedef enum _smc_stop_submode {
    kSmcStopSub0,          /*!< Stop submode 0, for VLLS0/LLS0. */
    kSmcStopSub1,          /*!< Stop submode 1, for VLLS1/LLS1. */
    kSmcStopSub2,          /*!< Stop submode 2, for VLLS2/LLS2. */
    kSmcStopSub3           /*!< Stop submode 3, for VLLS3/LLS3. */
} smc_stop_submode_t;

/*! @brief Partial STOP option*/
typedef enum _smc_pstop_option {
    kSmcPstopStop,                          /*!< STOP - Normal Stop mode*/
    kSmcPstopStop1,                         /*!< Partial Stop with both system and bus clocks disabled*/
    kSmcPstopStop2,                         /*!< Partial Stop with system clock disabled and bus clock enabled*/
    kSmcPstopReserved
} smc_pstop_option_t;

typedef enum _smc_por_option {
    kSmcPorEnabled,                        /*!< POR detect circuit is enabled in VLLS0. @internal gui name="Enabled" */
    kSmcPorDisabled                        /*!< POR detect circuit is disabled in VLLS0. @internal gui name="Disabled" */
} smc_por_option_t;

/*! @brief RAM2 power option*/
typedef enum _smc_ram2_option {
    kSmcRam2DisPowered,                    /*!< RAM2 not powered in LLS2/VLLS2. @internal gui name="Not Powered" */
    kSmcRam2Powered                        /*!< RAM2 powered in LLS2/VLLS2. @internal gui name="Powered" */
} smc_ram2_option_t;

/*! @brief LPO power option*/
typedef enum _smc_lpo_option {
    kSmcLpoEnabled,                        /*!< LPO clock is enabled in LLS/VLLSx. @internal gui name="Enabled" */
    kSmcLpoDisabled                        /*!< LPO clock is disabled in LLS/VLLSx. @internal gui name="Disabled" */
} smc_lpo_option_t;


typedef struct smc_power_mode_config
{
	power_modes_t powerModeName;
}smc_power_mode_config_t;

#ifdef __cplusplus
extern "C" {
#endif

smc_error_t smc_Hal_SetPowerMode(const smc_power_mode_config_t *powerModeConfig);

__STATIC_INLINE void smc_Hal_SetPowerModeProtection(uint8_t allowedMode)
{
	SMC_PMPROT = allowedMode;
}

__STATIC_INLINE uint8_t smc_Hal_GetPowerModeProtection(uint8_t mode)
{
	return (uint8_t) (SMC_PMPROT & mode);
}

power_mode_status_t smc_Hal_GetCurrentPowerMode(void);

#ifdef __cplusplus
}
#endif

#endif


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
