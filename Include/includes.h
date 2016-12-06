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


/*********************************************************************************************************
  Standard header files 
*********************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/*********************************************************************************************************
  Common header files   
*********************************************************************************************************/
#include "MKL25Z4.h"
#include "system_MKL25Z4.h"

/*********************************************************************************************************
  Driver header files  
*********************************************************************************************************/
#include "system.h"
#include "gpio.h"
#include "gpio_hal.h"
#include "port_hal.h"
#include "system.h"
#include "lptmr_hal.h"
#include "lptmr_driver.h"
#include "board.h"
#include "clock.h"
#include "tsi.h"
#include "tsi_hal.h"

//#include "power_manager.h"

/*********************************************************************************************************
  User's header files 
*********************************************************************************************************/

/*********************************************************************************************************
  Macro 
*********************************************************************************************************/

#define TSI_HIGHTHRESHOLD	(100U)
#define TSI_LOWTHRESHOLD	(200U)
#define TSI_THRESHOLD_SAMPLING      (100U)

#endif


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
