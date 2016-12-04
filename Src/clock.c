#include "includes.h"

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetLptmrFreq
 * Description   : Gets LPTMRx pre-scaler/glitch filter clock frequency.
 * This function gets the LPTMRx pre-scaler/glitch filter clock frequency.
 *
 *END**************************************************************************/
uint32_t CLOCK_SYS_GetLptmrFreq(clock_lptmr_src_t lptmrSrc)
{
    uint32_t freq;

    switch (lptmrSrc)
    {
        case clockLptmrSrcMcgIrClk:        /* MCG out clock  */
            freq = CLOCK_SYS_GetInternalRefClk(MCG);
            break;
        case clockLptmrSrcLpoClk:             /* LPO clock     */
            freq = CLOCK_SYS_GetLpoClockFreq();
            break;
        case clockLptmrSrcEr32kClk:        /* ERCLK32K clock */
            freq = CLOCK_SYS_GetExternalRefClock32kFreq();
            break;
        case clockLptmrSrcOsc0erClk:        /* OSCERCLK clock */
            freq = CLOCK_SYS_GetOsc0ExternalRefClockFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_HAL_GetInternalRefClk
 * Description   : Get the current mcg internal reference clock (MCGIRCLK)
 * This function will return the mcgirclk value in frequency(hz) based
 * on current mcg configurations and settings. It will not check if the
 * mcgirclk is enabled or not, just calculate and return the value.
 *
 *END**************************************************************************/
uint32_t CLOCK_SYS_GetInternalRefClk(MCG_Type * base)
{
    uint32_t mcgirclk;

    if (!MCG_BRD_C1_IRCLKEN(base))
    {
        return 0U;
    }

    if (MCG_BRD_C2_IRCS(base) == kMcgIrcSlow)
    {
        /* Slow internal reference clock selected*/
        mcgirclk = g_slowInternalRefClkFreq;
    }
    else
    {
        mcgirclk = g_fastInternalRefClkFreq >> MCG_BRD_SC_FCRDIV(base);
    }
    return mcgirclk;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetExternalRefClockFreq
 * Description   : Gets the ERCLK32K clock frequency.
 * This function gets the external reference (ERCLK32K) clock frequency.
 *
 *END**************************************************************************/
uint32_t CLOCK_SYS_GetExternalRefClock32kFreq(void)
{
    clock_er32k_src_t src;
    uint32_t freq;

    src = CLOCK_HAL_GetExternalRefClock32kSrc(SIM);

    switch (src)
    {
        case kClockEr32kSrcOsc0:      /* OSC 32k clock  */
            freq = (32768U == g_xtal0ClkFreq) ? 32768U : 0U;
            break;
        case kClockEr32kSrcRtc:      /* RTC 32k clock  */
            freq = g_xtalRtcClkFreq;
            break;
        case kClockEr32kSrcLpo:         /* LPO clock      */
            freq = CLOCK_SYS_GetLpoClockFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : CLOCK_SYS_GetOsc0ExternalRefClockFreq
 * Description   : Gets OSC0ERCLK.
 * This function gets the OSC0 external reference frequency.
 *
 *END**************************************************************************/
uint32_t CLOCK_SYS_GetOsc0ExternalRefClockFreq(void)
{
    if (OSC_HAL_GetExternalRefClkCmd(g_oscBase[0]))
    {
        return g_xtal0ClkFreq;
    }
    else
    {
        return 0U;
    }
}