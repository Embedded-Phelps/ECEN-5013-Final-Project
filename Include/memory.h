/***************************************************************************
 *
 *	Filename: 		memory.h
 *  Description:  	memory manipulation functions prototype header file
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/
 
#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>

#define SUCCESS 0;
#define ERROR   1;

/****************************************************
* @name: my_memmove
*
* @description: copy data from one memory to the other 
*               
* @param: src -- pointer to the source address
*         dst -- pointer to the destination address
*         length -- lenth of memory bytes to copy
*
* @return:SUCCESS/ERROR 
*/
int8_t my_memmove(uint8_t *src, uint8_t *dst, int32_t length);
int8_t my_memmove_word(uint8_t * src, uint8_t * dst, int32_t length);

/***************************************************************************
* @name: dma_memmove / dma_memmove_word
*
* @description: copy data from one memory to the other using KL25Z DMA
*               _word moves data in words instead of bytes
*               
* @param: src -- pointer to the source address
*         dst -- pointer to the destination address
*         length -- lenth of memory bytes to copy
*		  ch -- DMA channel
*
* @return:SUCCESS/ERROR 
*/
int8_t dma_memmove(uint8_t * src, uint8_t * dst, uint32_t length, uint8_t ch);
int8_t dma_memmove_word(uint8_t * src, uint8_t * dst, uint32_t length, uint8_t ch);

/****************************************************
* @name: my_memzero
*
* @description: zero out memory 
*               
* @param: src -- pointer to the memory address
*         length -- lenth of memory bytes to zero out
*
* @return:SUCCESS/ERROR 
*/
int8_t my_memzero(uint8_t *src, uint32_t length);
int8_t my_memzero_word(uint8_t *src, uint32_t length);
/****************************************************
* @name: dma_memzero
*
* @description: zero out memory using DMA
*				for KL25Z only 
*               
* @param: src -- pointer to the memory address
*         length -- lenth of memory bytes to zero out
*		  ch -- DMA channel
*
* @return:SUCCESS/ERROR 
*/
int8_t dma_memzero(uint8_t * src, uint32_t length, uint8_t ch);
int8_t dma_memzero_word(uint8_t * src, uint32_t length, uint8_t ch);
/****************************************************
* @name: my_reverse
*
* @description: reverse the order of certain bytes 
*               starting from a memory location 
*               
* @param: src -- pointer to the starting memory address
*         length -- lenth of memory bytes to reverse
*
* @return:SUCCESS/ERROR 
*/
int8_t my_reverse(uint8_t *src, uint32_t length);

#endif /* __MEMORY_H__ */
