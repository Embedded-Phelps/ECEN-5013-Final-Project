/***************************************************************************
 *
 *	Filename: 		log.h
 *  Description:  	log functions prototype header file
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/

#ifndef __LOG_H
#define __LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "includes.h"

/****************************************************
* @name: log_Str
*
* @description: Log a string
*               
* @param: src -- pointer to the string to be logged
*
*/
extern void log_Str(uint8_t *str);

/****************************************************
* @name: log_Mem
*
* @description: Log a memory segment
*               
* @param: src -- pointer to the memory location
* 		  num -- number of bytes memory to be logged
*
*/
extern void log_Mem(uint8_t * source, uint32_t num);

/****************************************************
* @name: log_Int
*
* @description: Log a string with an integer
*               
* @param: str -- pointer to the string to be logged
* 		  data -- integer to be logged
*
*/
extern void log_Int(uint8_t * str, int32_t data);

/****************************************************
* @name: log_Float
*
* @description: Log a string with a float
*               
* @param: str -- pointer to the string to be logged
* 		  fdata -- float to be logged
*
*/
extern void log_Float(uint8_t * str, float fdata);


#ifdef __cplusplus
}
#endif


#endif
