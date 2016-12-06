
#ifndef __LPTMR_HAL_H__
#define __LPTMR_HAL_H__

#include "includes.h"


/*!
 * @addtogroup lptmr_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 *******************************************************************************/

/*! @brief LPTMR pin selection, used in pulse counter mode.*/
typedef enum _lptmr_pin_select {
    lptmrPinSelectInput0 = 0x0U, /*!< Pulse counter input 0 is selected. @internal gui name="Input0"*/
    lptmrPinSelectInput1 = 0x1U, /*!< Pulse counter input 1 is selected. @internal gui name="Input1"*/
    lptmrPinSelectInput2 = 0x2U, /*!< Pulse counter input 2 is selected. @internal gui name="Input2"*/
    lptmrPinSelectInput3 = 0x3U  /*!< Pulse counter input 3 is selected. @internal gui name="Input3"*/
} lptmr_pin_select_t;

/*! @brief LPTMR pin polarity, used in pulse counter mode.*/
typedef enum _lptmr_pin_polarity {
    lptmrPinPolarityActiveHigh = 0x0U, /*!< Pulse Counter input source is active-high. @internal gui name="Active-high"*/
    lptmrPinPolarityActiveLow  = 0x1U  /*!< Pulse Counter input source is active-low. @internal gui name="Active-low"*/
} lptmr_pin_polarity_t;

/*! @brief LPTMR timer mode selection.*/
typedef enum _lptmr_timer_mode {
    lptmrTimerModeTimeCounter  = 0x0U, /*!< Time Counter mode. @internal gui name="Time Counter mode"*/
    lptmrTimerModePulseCounter = 0x1U  /*!< Pulse Counter mode. @internal gui name="Pulse Counter mode"*/
} lptmr_timer_mode_t;

/*! @brief LPTMR prescaler value.*/
typedef enum _lptmr_prescaler_value {
    lptmrPrescalerDivide2                      = 0x0U, /*!< Prescaler divide 2, glitch filter invalid. @internal gui name="2/invalid"*/
    lptmrPrescalerDivide4GlitchFilter2         = 0x1U, /*!< Prescaler divide 4, glitch filter 2. @internal gui name="4/2"*/
    lptmrPrescalerDivide8GlitchFilter4         = 0x2U, /*!< Prescaler divide 8, glitch filter 4. @internal gui name="8/4"*/
    lptmrPrescalerDivide16GlitchFilter8        = 0x3U, /*!< Prescaler divide 16, glitch filter 8. @internal gui name="16/8"*/
    lptmrPrescalerDivide32GlitchFilter16       = 0x4U, /*!< Prescaler divide 32, glitch filter 16. @internal gui name="32/16"*/
    lptmrPrescalerDivide64GlitchFilter32       = 0x5U, /*!< Prescaler divide 64, glitch filter 32. @internal gui name="64/32"*/
    lptmrPrescalerDivide128GlitchFilter64      = 0x6U, /*!< Prescaler divide 128, glitch filter 64. @internal gui name="128/64"*/
    lptmrPrescalerDivide256GlitchFilter128     = 0x7U, /*!< Prescaler divide 256, glitch filter 128. @internal gui name="256/128"*/
    lptmrPrescalerDivide512GlitchFilter256     = 0x8U, /*!< Prescaler divide 512, glitch filter 256. @internal gui name="512/256"*/
    lptmrPrescalerDivide1024GlitchFilter512    = 0x9U, /*!< Prescaler divide 1024, glitch filter 512. @internal gui name="1024/512"*/
    lptmrPrescalerDivide2048GlitchFilter1024   = 0xAU, /*!< Prescaler divide 2048 glitch filter 1024. @internal gui name="2048/1024"*/
    lptmrPrescalerDivide4096GlitchFilter2048   = 0xBU, /*!< Prescaler divide 4096, glitch filter 2048.@internal gui name="4096/2048"*/
    lptmrPrescalerDivide8192GlitchFilter4096   = 0xCU, /*!< Prescaler divide 8192, glitch filter 4096. @internal gui name="8192/4096"*/
    lptmrPrescalerDivide16384GlitchFilter8192  = 0xDU, /*!< Prescaler divide 16384, glitch filter 8192. @internal gui name="16384/8192"*/
    lptmrPrescalerDivide32768GlitchFilter16384 = 0xEU, /*!< Prescaler divide 32768, glitch filter 16384. @internal gui name="32768/16384"*/
    lptmrPrescalerDivide65536GlitchFilter32768 = 0xFU  /*!< Prescaler divide 65536, glitch filter 32768. @internal gui name="65535/32768"*/
} lptmr_prescaler_value_t;

/*! @brief LPTMR prescaler/glitch filter clock select. */
typedef enum _lptmr_prescaler_clock_select{
    lptmrPrescalerClock0 = 0x0U, /*!< Prescaler/glitch filter clock 0 selected. */
    lptmrPrescalerClock1 = 0x1U, /*!< Prescaler/glitch filter clock 1 selected. */
    lptmrPrescalerClock2 = 0x2U, /*!< Prescaler/glitch filter clock 2 selected. */
    lptmrPrescalerClock3 = 0x3U, /*!< Prescaler/glitch filter clock 3 selected. */
} lptmr_prescaler_clock_select_t;

/*! @brief LPTMR status return codes.*/
typedef enum _lptmr_status {
    status_LPTMR_Success                       = 0x0U, /*!< Succeed. */
    status_LPTMR_NotInitlialized               = 0x1U, /*!< LPTMR is not initialized yet. */
    status_LPTMR_NullArgument                  = 0x2U, /*!< Argument is NULL.*/
    status_LPTMR_InvalidPrescalerValue         = 0x3U, /*!< Value 0 is not valid in pulse counter mode. */
    status_LPTMR_InvalidInTimeCounterMode      = 0x4U, /*!< Function cannot be called in time counter mode. */
    status_LPTMR_InvalidInPulseCounterMode     = 0x5U, /*!< Function cannot be called in pulse counter mode. */
    status_LPTMR_TcfNotSet                     = 0x6U, /*!< If LPTMR is enabled, compare register can only altered when TCF is set. */
    status_LPTMR_TimerPeriodUsTooSmall         = 0x7U, /*!< Timer period time is too small for current clock source. */
    status_LPTMR_TimerPeriodUsTooLarge         = 0x8U  /*!< Timer period time is too large for current clock source. */
 } lptmr_status_t;
 
/*! @brief Define LPTMR prescaler user configure. */
typedef struct LptmrPrescalerUserConfig {
    bool prescalerBypass;                                /*!< Set this value will by pass the prescaler or glitch filter. */
    lptmr_prescaler_clock_select_t prescalerClockSelect; /*!< Selects the clock to be used by the LPTMR prescaler/glitch filter. */
    lptmr_prescaler_value_t prescalerValue;              /*!< Configures the size of the prescaler in time counter mode 
                                                              or width of the glitch filter in pulse counter mode. */
} lptmr_prescaler_user_config_t;

/*! @brief Define LPTMR working mode user configure. */
typedef struct LptmrWorkingModeUserConfig {
    lptmr_timer_mode_t timerModeSelect; /*!< Selects the LPTMR working mode: Timer or Pulse Counter. */
    bool freeRunningEnable;             /*!< Enables or disables the LPTMR free running. */
    lptmr_pin_polarity_t pinPolarity;   /*!< Specifies LPTMR pulse input pin polarity. */
    lptmr_pin_select_t pinSelect;       /*!< Specifies LPTMR pulse input pin select. */
} lptmr_working_mode_user_config_t;

/*******************************************************************************
 ** Variables
 *******************************************************************************/

/*******************************************************************************
 * API
 *******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name LPTMR HAL.
 * @{
 */

/*!
 * @brief Enables the LPTMR module operation.
 *
 */
__STATIC_INLINE void LPTMR_Hal_Enable(void)
{
    LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;
}

/*!
 * @brief Disables the LPTMR module operation.
 */
__STATIC_INLINE void LPTMR_Hal_Disable(void)
{
    LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;
}

__STATIC_INLINE void LPTMR_Hal_EnableClock()
{
    SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
}

__STATIC_INLINE void LPTMR_Hal_DisableClock()
{
    SIM_SCGC5 &= ~SIM_SCGC5_LPTMR_MASK;
}

__STATIC_INLINE bool LPTMR_Hal_isClockEnabled()
{
    return (bool)(SIM_SCGC5 & SIM_SCGC5_LPTMR_MASK);
}

__STATIC_INLINE void LPTMR_Hal_EnableNVICInterrupt(void)
{
	NVIC_EnableIRQ(LPTMR0_IRQn);
}

__STATIC_INLINE void LPTMR_Hal_DisableNVICInterrupt(void)
{
	NVIC_DisableIRQ(LPTMR0_IRQn);
}

/*!
 * @brief Clears the LPTMR interrupt flag if set.
 *
 * @param base The LPTMR peripheral base address.
 */
__STATIC_INLINE void LPTMR_Hal_ClearIntFlag(void)
{
    LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
}

/*!
 * @brief Returns the current LPTMR interrupt flag.
 *
 * @return State of LPTMR interrupt flag
 * @retval true An interrupt is pending.
 * @retval false No interrupt is pending.
 */
__STATIC_INLINE bool LPTMR_Hal_IsIntPending(void)
{
    return (bool)(LPTMR0_CSR & LPTMR_CSR_TCF_MASK);
}

/*!
 * @brief Enables or disables the LPTMR interrupt.
 *
 * @param enable Pass true to enable LPTMR interrupt
 */
__STATIC_INLINE void LPTMR_Hal_SetIntCmd(bool enable)
{
    LPTMR0_CSR |= LPTMR_CSR_TIE_MASK;
}

/*!
 * @brief Configures the LPTMR working mode.
 *
 * @param timerMode Specifies LPTMR working mode configure, see #lptmr_working_mode_user_config_t
 */
void LPTMR_Hal_SetTimerWorkingMode(lptmr_working_mode_user_config_t timerMode);

/*!
 * @brief Sets the LPTMR prescaler mode.
 *
 * @param prescaler_config Specifies LPTMR prescaler configure, see #lptmr_prescaler_user_config_t
 */
void LPTMR_Hal_SetPrescalerMode(lptmr_prescaler_user_config_t prescaler_config);

/*!
 * @brief Sets the LPTMR compare value.
 *
 * @param compareValue Specifies LPTMR compare value, less than 0xFFFFU
 */
__STATIC_INLINE void LPTMR_Hal_SetCompareValue(uint32_t compareValue)
{
    LPTMR0_CMR =  compareValue;
}

/*!
 * @brief Gets the LPTMR compare value.
 *
 * @return The LPTMR compare value.
 */
__STATIC_INLINE uint32_t LPTMR_Hal_GetCompareValue(void)
{
    return (uint32_t)LPTMR0_CMR;
}

/*!
 * @brief Gets the LPTMR counter value.
 *
 * @return Current LPTMR counter value
 */
uint32_t LPTMR_Hal_GetCounterValue(void);

/*!
 * @brief Restores the LPTMR module to reset state.
 *
 */
void LPTMR_Hal_Init(void);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_LPTMR_HAL_H__*/
/*******************************************************************************
 * EOF
 *******************************************************************************/

