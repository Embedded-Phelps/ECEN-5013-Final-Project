/***************************************************************************
 *
 *	Filename: 		log.c
 *  Description:  	log functions implementation
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/

#include "includes.h"

extern CircBuf_t * tx_buf; 
extern CircBuf_t * rx_buf;

#ifndef DEBUG
void log_Str(uint8_t *str){} 
void log_Mem(uint8_t * source, uint32_t num){}
void log_Int(uint8_t * str, int32_t data){}
void log_Float(uint8_t * str, float fdata){}

#else
/****************************************************
* @name: log_Str
*
* @description: Log a string
*               
* @param: src -- pointer to the string to be logged
*
*/	
 void log_Str(uint8_t *str){
#ifdef _BBB
	printf("%s", str);
#else
	while(*str != '\0')
			cb_Enqueue(tx_buf, *str++);
	if((cb_IsEmpty(tx_buf)!=EMPTY)&&(!(UART0_C2 & UART0_C2_TIE_MASK)))
		UART0_C2 |= UART0_C2_TIE_MASK;
#endif
}

/****************************************************
* @name: log_Mem
*
* @description: Log a memory segment
*               
* @param: src -- pointer to the memory location
* 		  num -- number of bytes memory to be logged
*
*/
void log_Mem(uint8_t * source, uint32_t num){

#ifdef _BBB
	while (num>0){
		printf("%c",*source++);
		num--;
	}
#else
	while(num>0){
		cb_Enqueue(tx_buf, *source++);
		num--;
	}
	if((cb_IsEmpty(tx_buf)!=EMPTY)&&(!(UART0_C2 & UART0_C2_TIE_MASK)))
		UART0_C2 |= UART0_C2_TIE_MASK;
#endif
		
}

/****************************************************
* @name: log_Int
*
* @description: Log a string with an integer
*               
* @param: str -- pointer to the string to be logged
* 		  data -- integer to be logged
*
*/
void log_Int(uint8_t * str, int32_t data){
		
#ifdef _BBB
	printf("%s%d", str, data);
#else
	uint8_t temp[12];
	my_itoa(temp, data, 10);
	log_Str(str);
	log_Str(temp);
#endif		
	
}

/****************************************************
* @name: log_Float
*
* @description: Log a string with a float
*               
* @param: str -- pointer to the string to be logged
* 		  fdata -- float to be logged
*
*/
void log_Float(uint8_t * str, float fdata){

#ifdef _BBB
	printf("%s%f",str, fdata);
#else
	uint8_t temp[32];	
	my_ftoa(temp, fdata);
	log_Str(str);
	log_Str(temp);
#endif
}

#endif
