
#include "includes.h"


/*******************************************************************************
 * Code
 ******************************************************************************/


/*FUNCTION**********************************************************************
 *
 * Function Name : PIT_Hal_ReadLifetimeTimerCount
 * Description   : Read current lifefime counter value.
 * Lifetime timer is 64-bit timer which chains timer 0 and timer 1 together.
 * So, timer 0 and 1 should by chained by calling PIT_HAL_SetTimerChainCmd
 * before using this timer. The period of lifetime timer equals to "period of
 * timer 0 * period of timer 1". For the 64-bit value, higher 32-bit will have
 * the value of timer 1, and lower 32-bit have the value of timer 0.
*
 *END**************************************************************************/
uint64_t PIT_Hal_ReadLifetimeTimerCount(void)
{
    uint32_t valueH = 0U, valueL = 0U;

    /* LTMR64H should be read before LTMR64L */
    valueH = PIT_LTMR64H;
    valueL = PIT_LTMR64L;
    return (((uint64_t)valueH << 32U) + (uint64_t)(valueL));
}

/*******************************************************************************
 * EOF
 ******************************************************************************/

