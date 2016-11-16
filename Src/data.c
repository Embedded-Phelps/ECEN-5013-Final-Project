/***************************************************************************
 *
 *	Filename: 		data.c
 *  Description:  	data manipulation functions implementation
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/

#include "includes.h"

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
uint8_t * my_itoa(uint8_t *str, int32_t data, int32_t base){
    int32_t pw, temp;
    uint8_t * result = str;
		if (str == NULL)
        return NULL;
    if((base == 10)&&(data < 0)){
        *str++ = '-';
        data = 0 - data;
				temp = data;
    }
    temp = data;
    for(pw=1;temp>=base;temp/=base)
        pw *=base;
    for(;pw>0;pw/=base){
        temp = data/pw;
        if (temp>9)
        temp += 7;
        *str++ = '0'+temp;
        data %= pw;
    }
    *str = '\0';
    return result;
}

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
uint8_t * my_ftoa(uint8_t *str, float fdata){	
	int32_t int_part;
	float float_part;
	
	uint32_t fdigit=0;
	
	uint8_t str_float[32];
	uint8_t i=0;
	uint8_t * p = str;
	my_memzero(str,32);
	my_memzero(str_float,32);
	
	int_part = (int32_t)fdata;
	if((fdata>-1.000000)&&(fdata<0.000001)){
	
		*str='-';
		*(str+1)='0';
		*(str+2)='\0';
	}
	else{
	my_itoa(str, int_part, 10);
	}
	
	if (fdata >=0.000001)  
		float_part = fdata - int_part;
	else
		float_part = int_part - fdata;
	while(float_part != (int32_t)(float_part))
	{
		float_part *= 10;
		fdigit++;
	}
	if (fdigit>0){
		my_itoa(str_float, (int32_t)float_part, 10);
	}
	while(*p++ != '\0'){
		i++;
	}
	*(str + i)='.';
	my_memmove(str_float, (str + i + 1), 3);
	*(str+i+3+1)='\0';
	return str;
}

/****************************************************
* @name: my_atoi
*
* @description: Converts an ASCII string to an integer
*
* @param: str -- pointer to the string
* 
* @return: the integer converted from the string
*/
int32_t my_atoi(uint8_t *str){
    uint8_t neg=1;
    uint32_t n;
		if (str == NULL)
        return 0;
    while (*str == ' ')			//skip spaces
        str ++;
    if ((*str != '-')&&((*str < '0')||(*str > '9')))
        return 0;
    if (*str == '-'){
        neg = 0;
        str ++;
    }
    for (n=0;((*str >= '0')&&(*str <= '9'));str++){
        n=10*n+(*str-'0');
    }
    return neg?n:-n;
}
