#ifndef __CLOCK_H__
#define __CLOCK_H__

typedef enum clock_lptmr_src
{
    clockLptmrSrcMcgIrClk,        /* MCG out clock  */
    clockLptmrSrcLpoClk,             /* LPO clock      */
    clockLptmrSrcEr32kClk,        /* ERCLK32K clock */
    clockLptmrSrcOsc0erClk,       /* OSCERCLK clock */
}clock_lptmr_src_t;


#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

#endif
