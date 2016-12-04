#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "includes.h"

typedef enum _clock_lptmr_src_t
{
    clockLptmrSrcMcgIrClk,        /* MCG out clock  */
    clockLptmrSrcLpoClk,             /* LPO clock      */
    clockLptmrSrcEr32kClk,        /* ERCLK32K clock */
    clockLptmrSrcOsc0erClk,       /* OSCERCLK clock */
} clock_lptmr_src_t;


#if defined(__cplusplus)
extern "C" {
#endif

uint32_t CLOCK_SYS_GetLptmrFreq(clock_lptmr_src_t lptmrSrc);

uint32_t CLOCK_SYS_GetInternalRefClk(MCG_Type * base);

uint32_t CLOCK_SYS_GetLpoClockFreq(void);

uint32_t CLOCK_SYS_GetExternalRefClock32kFreq(void);

uint32_t CLOCK_SYS_GetOsc0ExternalRefClockFreq(void);

__STATIC_INLINE uint32_t CLOCK_SYS_GetLpoClockFreq(void)
{
    return CPU_LPO_CLK_HZ;
}
#if defined(__cplusplus)
}
#endif

#endif