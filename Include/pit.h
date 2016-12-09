
#ifndef __PIT_H__
#define __PIT_H__

#include "includes.h"

/*!
 * @addtogroup pit_driver
 * @{
 */

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief PIT timer configuration structure
 *
 * Defines a structure PitConfig and uses the PIT_InitChannel() function to make necessary
 * initializations. The remaining functions can be used for PIT configuration.
 *
 * @note The timer chain feature is not valid in all devices. Check the
 * fsl_pit_features.h for accurate settings. If it's not valid, the value set here
 * is bypassed inside the PIT_InitChannel() function.
 * @internal gui name="PIT configuration" id="pitCfg"
 */
 
typedef void (*pit_callback_t)(void);

typedef struct pit_user_config 
{
    bool isInterruptEnabled; 
    uint32_t periodUs;  
		pit_callback_t callbackFunc;
} pit_user_config_t;

extern pit_user_config_t * pitConfigPtr;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name Initialization and Shutdown
 * @{
 */

/*!
 * @brief Initializes the PIT module.
 *
 * Call this function before calling any other PIT driver functions.
 * This function un-gates the PIT clock and enables the PIT module. The isRunInDebug
 * passed into function affects all timer channels.
 *
 * @param instance PIT module instance number.
 * @param isRunInDebug Timers run or stop in debug mode.
 *        - true:  Timers continue to run in debug mode.
 *        - false: Timers stop in debug mode.
 * @return Error or success status returned by API.
 */
pit_status_t PIT_Init(bool isRunInDebug);

/*!
 * @brief Disables the PIT module and gate control.
 *
 * This function disables all PIT interrupts and PIT clock. It then gates the
 * PIT clock control. PIT__Init must be called to use PIT again.
 *
 * @param instance PIT module instance number.
 * @return Error or success status returned by API.
 */
pit_status_t PIT_Deinit(void);

/*!
 * @brief Initializes the PIT channel.
 *
 * This function initializes the PIT timers by using a channel. Pass in the timer number and its
 * configuration structure. Timers do not start counting by default after calling this
 * function. The function PIT_StartTimer must be called to start the timer counting.
 * Call the PIT_SetTimerPeriodByUs to re-set the period.
 *
 * This is an example demonstrating how to define a PIT channel configuration structure:
   @code
   pit_user_config_t pitTestInit = {
        .isInterruptEnabled = true,
        // In unit of microseconds.
        .periodUs = 1000,
   };
   @endcode
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number.
 * @param config PIT channel configuration structure.
 */
void PIT_InitChannel(uint32_t channel, const pit_user_config_t * config);

/* @} */

/*!
 * @name Timer Start and Stop
 * @{
 */

/*!
 * @brief Starts the timer counting.
 *
 * After calling this function, timers load period value, count down to 0 and
 * then load the respective start value again. Each time a timer reaches 0,
 * it generates a trigger pulse and sets the timeout interrupt flag.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number.
 */
void PIT_StartTimer(uint32_t channel);

/*!
 * @brief Stops the timer counting.
 *
 * This function stops every timer counting. Timers reload their periods
 * respectively after the next time they call the PIT_StartTimer.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number.
 */
void PIT_StopTimer(uint32_t channel);

/* @} */

/*!
 * @name Timer Period
 * @{
 */

/*!
 * @brief Sets the timer period in microseconds.
 *
 * The period range depends on the frequency of the PIT source clock. If the required period
 * is out of range, use the lifetime timer if applicable.
 * This function is only valid for one channel. If channels are chained together,
 * the period makes no sense.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number.
 * @param us Timer period in microseconds.
 */
void PIT_SetTimerPeriodByUs(uint32_t channel, uint32_t us);

/*!
 * @brief Gets the timer period in microseconds for a single channel.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number.
 * @return Timer period in microseconds.
 */
uint32_t PIT_GetTimerPeriodByUs(uint32_t channel);

/*!
 * @brief Reads the current timer value in microseconds.
 *
 * This function returns an absolute time stamp in microseconds.
 * One common use of this function is to measure the running time of a part of
 * code. Call this function at both the beginning and end of code. The time
 * difference between these two time stamps is the running time. Make sure the
 * running time does not exceed the timer period. The time stamp returned is
 * up-counting.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number.
 * @return Current timer value in microseconds.
 */
uint32_t PIT_ReadTimerUs(uint32_t channel);

/*!
 * @brief Sets the timer period in units of count.
 *
 * Timers begin counting from the value set by this function.
 * The counter period of a running timer can be modified by first stopping
 * the timer, setting a new load value, and  starting the timer again. If
 * timers are not restarted, the new value is loaded after the next trigger
 * event.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number
 * @param count Timer period in units of count
 */
void PIT_SetTimerPeriodByCount(uint32_t channel, uint32_t count);

/*!
 * @brief Returns the current timer period in units of count.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number
 * @return Timer period in units of count
 */
uint32_t PIT_GetTimerPeriodByCount(uint32_t channel);

/*!
 * @brief Reads the current timer counting value.
 *
 * This function returns the real-time timer counting value in a range from 0 to a
 * timer period.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number
 * @return Current timer counting value
 */
uint32_t PIT_ReadTimerCount(uint32_t channel);

/*!
 * @brief Sets the lifetime timer period.
 *
 * Timer 1 must be chained with timer 0 before using the lifetime timer. The period
 * range is restricted by "period * pitSourceClock < max of an uint64_t integer",
 * or it may cause an overflow and be unable to set the correct period.
 *
 * @param instance PIT module instance number.
 * @param us Lifetime timer period in microseconds.
 */
void PIT_SetLifetimeTimerPeriodByUs( uint64_t us);

/*!
 * @brief Reads the current lifetime value in microseconds.
 *
 * This feature returns an absolute time stamp in microseconds. The time stamp
 * value does not exceed the timer period. The timer is up-counting.
 *
 * @param instance PIT module instance number.
 * @return Current lifetime timer value in microseconds.
 */
uint64_t PIT_ReadLifetimeTimerUs(void);

/* @} */

/*!
 * @name Microseconds
 * @{
 */

/*!
 * @brief Initializes two PIT channels to serve as a microseconds unit.
 *
 * Because this function is in parallel with the PIT_InitChannel function, the two functions overwrite each other.
 * The PIT_Init function must be called before calling this function.
 * If the device has a dedicated lifetime timer, it is more effective than the set of timers.
 * The microseconds unit uses two chained channels to simulate a lifetime timer. The
 * channel number passed in and the "channel -1" channel are used.
 * @note
 * 1. These two channels are occupied and could not be used with other purposes.
 * 2. The channel number passed in must be greater than 0.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number which is chained with the former channel. Must
 *        be greater than 0.
 */
void PIT_InitUs(uint32_t channel);

/*!
 * @brief Gets an absolute time stamp.
 *
 * This function gets the elapsed time in time A
 * and calls it in time B. The elapsed time can be obtained by B - A. The result may have
 * 3-5 microsecond error depending on the system clock frequency.
 *
 * @return Absolute time stamp from the chained lifetime timers in microsecond units.
 */
uint32_t PIT_GetUs(void);

/*!
 * @brief Delays the specific microseconds.
 *
 * The delay may have a 3-5 microsecond error depending on the system clock frequency.
 *
 * @param us Number of microseconds to delay.
 */
void PIT_DelayUs(uint32_t us);

/* @} */


/*!
 * @name Interrupt
 * @{
 */

/*!
 * @brief Clears the timer interrupt flag.
 *
 * This function clears the timer interrupt flag after a timeout event
 * occurs.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number
 */
void PIT_ClearIntFlag(uint32_t channel);

/*!
 * @brief Reads the current timer timeout flag.
 *
 * This flag is set every time the timer counts to 0.
 *
 * @param instance PIT module instance number.
 * @param channel Timer channel number
 * @return Current status of the timeout flag
 *         - true:  Timeout has occurred.
 *         - false: Timeout has not yet occurred.
 */
bool PIT_IsIntPending(uint32_t channel);

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif 
/*******************************************************************************
 * EOF
 ******************************************************************************/

