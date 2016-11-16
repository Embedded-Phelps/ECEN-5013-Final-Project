/***************************************************************************
 *
 *	Filename: 		dma.h
 *  Description:  	memory manipulation functions prototype header file
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/
 
#ifndef __DMA_H__
#define __DMA_H__

#include "includes.h"

#define DMA_ISR_ENABLE		(1)

uint8_t dma_Init_Mem2mem(uint8_t dma_ch, 
						 uint8_t * source_addr,
						 uint8_t * destination_addr,
						 uint32_t transfer_size);

uint8_t dma_Init_Mem2Per(uint8_t dma_ch,
						 uint8_t per_source,
						 uint8_t * source_addr,
						 uint8_t * destination_addr,
						 uint32_t transfer_size);
						 
uint8_t dma_Init_Per2Mem(uint8_t dma_ch,
						 uint8_t per_source,
						 uint8_t * source_addr,
						 uint8_t * destination_addr,
						 uint32_t transfer_size);
						 
#endif

