/***************************************************************************
 *
 *	Filename: 		data.h
 *  Description:  	data manipulation functions prototype header file
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/

#ifndef __DATA_H__
#define __DATA_H__

#include <stdint.h>

/****************************************************
* @name: my_itoa
*
* @description: Converts a integer to an ASCII string
*
* @param: str -- pointer to the string
*         data -- the integer to be converted
*         base -- base of the integer
* 
* @return: pointer to the string
*/
uint8_t * my_itoa(uint8_t *str, int32_t data, int32_t base);

/****************************************************
* @name: my_ftoa
*
* @description: Converts a float value to an ASCII string
*
* @param: str -- pointer to the string
* 				fdata -- the float value to be converted
*
* @return: the integer converted from the string
*/
uint8_t * my_ftoa(uint8_t *str, float fdata);

/****************************************************
* @name: my_atoi
*
* @description: Converts an ASCII string to an integer
*
* @param: str -- pointer to the string
* 
* @return: the integer converted from the string
*/
int32_t  my_atoi(uint8_t *str);

#endif /* __DATA_H__ */
