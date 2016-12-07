
#ifndef __PIT_HAL_H__
#define __PIT_HAL_H__

#include "includes.h"

/*!
 * @addtogroup pit_hal
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Error codes for PIT driver. */
typedef enum pit_status
{
    status_PIT_Success = 0x00U,
    status_PIT_Fail    = 0x01U
} pit_status_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization
 * @{
 */

/*!
 * @brief Enables the PIT module.
 *
 * This function enables the PIT timer clock (Note: this function does not un-gate
 * the system clock gating control). It should be called before any other timer
 * related setup.
 *
 * @param base Base address for current PIT instance.
 */
__STATIC_INLINE void PIT_Hal_Enable(void)
{
    PIT_MCR &= ~PIT_MCR_MDIS_MASK;
}

/*!
 * @brief Disables the PIT module.
 *
 * This function disables all PIT timer clocks(Note: it does not affect the
 * SIM clock gating control).
 *
 * @param base Base address for current PIT instance.
 */
__STATIC_INLINE void PIT_Hal_Disable(void)
{
    PIT_MCR |= PIT_MCR_MDIS_MASK;
}

__STATIC_INLINE void PIT_Hal_EnableClock(void)
{
    SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
}

__STATIC_INLINE void PIT_Hal_DisableClock(void)
{
    SIM_SCGC6 &= ~SIM_SCGC6_PIT_MASK;
}

__STATIC_INLINE bool PIT_Hal_isClockGateEnable(void)
{
    return (SIM_SCGC6 & SIM_SCGC6_PIT_MASK) ? true : false;
}

__STATIC_INLINE void PIT_Hal_EnableNVICInterrupt(void)
{
	NVIC_EnableIRQ(PIT_IRQn);
}

__STATIC_INLINE void PIT_Hal_DisableNVICInterrupt(void)
{
	NVIC_DisableIRQ(PIT_IRQn);
}

/*!
 * @brief Configures the timers to continue  running or to stop in debug mode.
 *
 * In debug mode, the timers may or may not be frozen, based on the configuration of
 * this function. This is intended to aid software development, allowing the developer
 * to halt the processor, investigate the current state of the system (for example,
 * the timer values), and  continue the operation.
 *
 * @param base Base address for current PIT instance.
 * @param timerRun Timers run or stop in debug mode.
 *        - true:  Timers continue to run in debug mode.
 *        - false: Timers stop in debug mode.
 */
__STATIC_INLINE void PIT_Hal_SetTimerRunInDebugCmd(bool timerRun)
{
    if (timerRun)
	{
		PIT_MCR &= ~PIT_MCR_FRZ_MASK;
	}
	else
	{
		PIT_MCR |= PIT_MCR_FRZ_MASK;
	}
}

/*!
 * @brief Enables or disables the timer chain with the previous timer.
 *
 * When a timer has a chain mode enabled, it  only counts after the previous
 * timer has expired. If the timer n-1 has counted down to 0, counter n
 * decrements the value by one. This allows the developers to chain timers together
 * and form a longer timer. The first timer (timer 0) cannot be chained to any
 * other timer.
 *
 * @param base Base address for current PIT instance.
 * @param channel  Timer channel number which is chained with the previous timer.
 * @param enable Enable or disable chain.
 *        - true:  Current timer is chained with the previous timer.
 *        - false: Timer doesn't chain with other timers.
 */
__STATIC_INLINE void PIT_Hal_SetTimerChainCmd(uint32_t channel, bool enable)
{
    assert(channel < PIT_TIMER_CNT);
	if(enable)
	{
		PIT_TCTRL(channel) |= PIT_TCTRL_CHN_MASK;
	}
	else
	{	
		PIT_TCTRL(channel) &= ~PIT_TCTRL_CHN_MASK;
	}
}


/* @} */

/*!
 * @name Timer Start and Stop
 * @{
 */

/*!
 * @brief Starts the timer counting.
 *
 * After calling this function, timers load the start value as specified by the function
 * PIT_Hal_SetTimerPeriodByCount(PIT_Type * base, uint32_t channel, uint32_t count), count down to
 * 0, and  load the respective start value again. Each time a timer reaches 0,
 * it generates a trigger pulse and sets the time-out interrupt flag.
 *
 * @param base Base address for current PIT instance.
 * @param channel Timer channel number
 */
__STATIC_INLINE void PIT_Hal_StartTimer(uint32_t channel)
{
    assert(channel < PIT_TIMER_CNT);
    PIT_TCTRL(channel) |= PIT_TCTRL_TEN_MASK;
}

/*!
 * @brief Stops the timer from counting.
 *
 * This function stops every timer from counting. Timers reload their periods
 * respectively after they call the PIT_Hal_StartTimer the next time.
 *
 * @param base Base address for current PIT instance.
 * @param channel Timer channel number
 */
__STATIC_INLINE void PIT_Hal_StopTimer(uint32_t channel)
{
    assert(channel < PIT_TIMER_CNT);
    PIT_TCTRL(channel) &= ~PIT_TCTRL_TEN_MASK;
}

/*!
 * @brief Checks to see whether the current timer is started or not.
 *
 * @param base Base address for current PIT instance.
 * @param channel Timer channel number
 * @return Current timer running status
 *         -true: Current timer is running.
 *         -false: Current timer has stopped.
 */
__STATIC_INLINE bool PIT_Hal_IsTimerRunning(uint32_t channel)
{
    assert(channel < PIT_TIMER_CNT);
    return ((PIT_TCTRL(channel) & PIT_TCTRL_TEN_MASK)) ? true : false;
}

/* @} */

/*!
 * @name Timer Period
 * @{
 */

/*!
 * @brief Sets the timer period in units of count.
 *
 * Timers begin counting from the value set by this function.
 * The counter period of a running timer can be modified by first stopping
 * the timer, setting a new load value, and  starting the timer again. If
 * timers are not restarted, the new value is loaded after the next trigger
 * event.
 *
 * @param base Base address for current PIT instance.
 * @param channel Timer channel number
 * @param count Timer period in units of count
 */
__STATIC_INLINE void PIT_Hal_SetTimerPeriodByCount(uint32_t channel, uint32_t count)
{
    assert(channel < PIT_TIMER_CNT);
    PIT_LDVAL(channel) = count;
}

/*!
 * @brief Returns the current timer period in units of count.
 *
 * @param base Base address for current PIT instance.
 * @param channel Timer channel number
 * @return Timer period in units of count
 */
__STATIC_INLINE uint32_t PIT_Hal_GetTimerPeriodByCount(uint32_t channel)
{
    assert(channel < PIT_TIMER_CNT);
    return PIT_LDVAL(channel);
}

/*!
 * @brief Reads the current timer counting value.
 *
 * This function returns the real-time timer counting value, in a range from 0 to a
 * timer period.
 *
 * @param base Base address for current PIT instance.
 * @param channel Timer channel number
 * @return Current timer counting value
 */
__STATIC_INLINE uint32_t PIT_Hal_ReadTimerCount(uint32_t channel)
{
    assert(channel < PIT_TIMER_CNT);
    return PIT_CVAL(channel);
}

/*!
 * @brief Reads the current lifetime counter value.
 *
 * The lifetime timer is a 64-bit timer which chains timer 0 and timer 1 together.
 * Timer 0 and 1 are chained by calling the PIT_Hal_SetTimerChainCmd
 * before using this timer. The period of lifetime timer is equal to the "period of
 * timer 0 * period of timer 1". For the 64-bit value, the higher 32-bit has
 * the value of timer 1, and the lower 32-bit has the value of timer 0.
 *
 * @param base Base address for current PIT instance.
 * @return Current lifetime timer value
 */
uint64_t PIT_Hal_ReadLifetimeTimerCount(void);

/* @} */

/*!
 * @name Interrupt
 * @{
 */

/*!
 * @brief Enables or disables the timer interrupt.
 *
 * If enabled, an interrupt happens when a timeout event occurs
 * (Note: NVIC should be called to enable pit interrupt in system level).
 *
 * @param base Base address for current PIT instance.
 * @param channel  Timer channel number
 * @param enable Enable or disable interrupt.
 *        - true:  Generate interrupt when timer counts to 0.
 *        - false: No interrupt is generated.
 */
__STATIC_INLINE void PIT_Hal_SetIntCmd(uint32_t channel, bool enable)
{
    assert(channel < PIT_TIMER_CNT);
	if(enable)
	{
		PIT_TCTRL(channel) |= PIT_TCTRL_TIE_MASK;
	}
	else
	{
		PIT_TCTRL(channel) &= ~PIT_TCTRL_TIE_MASK;
	}
    
}

/*!
 * @brief Checks whether the timer interrupt is enabled or not.
 *
 * @param base Base address for current PIT instance.
 * @param channel  Timer channel number
 * @return Status of enabled or disabled interrupt
 *        - true: Interrupt is enabled.
 *        - false: Interrupt is disabled.
 */
__STATIC_INLINE bool PIT_Hal_GetIntCmd(uint32_t channel)
{
    assert(channel < PIT_TIMER_CNT);
    return (PIT_TCTRL(channel) & PIT_TCTRL_TIE_MASK) ? true : false;
}

/*!
 * @brief Clears the timer interrupt flag.
 *
 * This function clears the timer interrupt flag after a timeout event
 * occurs.
 *
 * @param base Base address for current PIT instance.
 * @param channel Timer channel number
 */
__STATIC_INLINE void PIT_Hal_ClearIntFlag(uint32_t channel)
{
    assert(channel < PIT_TIMER_CNT);
    /* Write 1 will clear the flag. */
    PIT_TFLG(channel) = 1U;
}

/*!
 * @brief Reads the current timer timeout flag.
 *
 * Every time the timer counts to 0, this flag is set.
 *
 * @param base Base address for current PIT instance.
 * @param channel Timer channel number
 * @return Current status of the timeout flag
 *         - true:  Timeout has occurred.
 *         - false: Timeout has not yet occurred.
 */
__STATIC_INLINE bool PIT_Hal_IsIntPending(uint32_t channel)
{
    assert(channel < PIT_TIMER_CNT);
    return (PIT_TFLG(channel)) ? true : false;
}

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* __FSL_PIT_HAL_H__*/
/*******************************************************************************
* EOF
*******************************************************************************/

