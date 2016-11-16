/***************************************************************************
 *
 *	Filename: 		gpio.c
 *  Description:  	KL25Z GPIO functions prototype header file
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/
#ifndef __GPIO__H
#define __GPIO__H

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************************************************
  PORT Configuration
********************************************************************************************************/
#define PT_A               A
#define PT_B               B
#define PT_C               C
#define PT_D               D
#define PT_E               E

/*
 * Clock configuration for Port module
 */ 
#define V_PORT_ENABLE_CLK(V_PTO)    SIM_SCGC5 |=  SIM_SCGC5_PORT##V_PTO##_MASK 
#define V_PORT_DISABLE_CLK(V_PTO)   SIM_SCGC5 &= ~SIM_SCGC5_PORT##V_PTO##_MASK

#define PORT_ENABLE_CLK(PTO)        V_PORT_ENABLE_CLK(PTO)
#define PORT_DISABLE_CLK(PTO)       V_PORT_DISABLE_CLK(PTO)

extern void led_Gpio_Init(void);
extern void spi_Gpio_Init(void);
extern void nrf_Gpio_Init(void);
#ifdef __cplusplus
}
#endif


#endif


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/  

