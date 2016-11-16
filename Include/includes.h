/***************************************************************************
 *
 *	Filename: 		includes.h
 *  Description:  	primary header file for the project
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/
 
#ifndef  __INCLUDES_H
#define  __INCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************************************************
  Standard header files 
*********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

/*********************************************************************************************************
  Common header files   
*********************************************************************************************************/
#ifndef _BBB
	#include "MKL25Z4.h"
	#include "system_MKL25Z4.h"
#endif

/*********************************************************************************************************
  Driver header files  
*********************************************************************************************************/
#ifndef _BBB
	#include "uart.h"
	#include "gpio.h"
	#include "timer.h"
	#include "dma.h"
	#include "spi.h"
	#include "message.h"
#endif

/*********************************************************************************************************
  User's header files 
*********************************************************************************************************/
#include "log.h"
#include "data.h"
#include "memory.h"
#include "profiler.h"
#include "circbuf.h"
#include "nRF24L01.h"

/*********************************************************************************************************
  Macro 
*********************************************************************************************************/
#define LOG_TEST			(1)					/* turn on/off logger test */
#define CIRCBUF_UNITTEST	(0)					/* turn on/off circular buffer unit test */
#define TIME_PROFILE		(0)					/* turn on/off time profiling of a series of functions */

#define SET_LED_RED(x)     (TPM2_C0V = (x))		/* Command to set the brightness of the red LED */
#define SET_LED_GREEN(x)   (TPM2_C1V = (x))		/* Command to set the brightness of the green LED */
#define SET_LED_BLUE(x)    (TPM0_C1V = (x))		/* Command to set the brightness of the blue LED */

#define TIME_PER_CYCLE		(41.6666666)

//#define _BBB									/* uncomment this when building for BBB */
#define DEBUG
#ifdef __cplusplus
}
#endif

#endif


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
