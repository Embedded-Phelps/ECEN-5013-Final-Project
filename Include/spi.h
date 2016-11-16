/***************************************************************************
 *
 *	Filename: 		spi.h
 *  Description:  	spi driver function prototype header file
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/
#ifndef __SPI_H
#define __SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "includes.h"

#define MASTER	(1)
#define SLAVE		(0)

#define SPI_SELECT		(FGPIOC_PCOR|= (1<<12))
#define SPI_DESELECT	(FGPIOC_PSOR|= (1<<12))

extern void spi0_Init(uint8_t mode);

/*********************************************************************
* @name: spi0_Sendbyte
*
* @description: send out 1 byte of data via SPI interface 
*               
* @param: data -- data to send
*         
* @return: data received from slave device while sending the data
*/
extern uint8_t spi0_Sendbyte(uint8_t data);

/*********************************************************************
* @name: spi0_getbyte
*
* @description: received 1 byte of data via SPI interface from slave device
*         
* @return: data received from slave device
*/
extern uint8_t spi0_getbyte(void);
 
#ifdef __cplusplus
}
#endif


#endif
