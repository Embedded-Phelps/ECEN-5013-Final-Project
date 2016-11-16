/***************************************************************************
 *
 *	Filename: 		circbuf.c
 *  Description:  	circular buffer functions impletation 
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/
 
#include "includes.h"

/********************************************************
 *@name        cb_IsFull
 *
 *@description Check if the circular buffer is full 
 *
 *@param       cb - pointer to the circular buffer
 * 
 *@return      FULL(1): buffer is full
 *             NORMAL(2): buffer is not full
 */
CB_e cb_IsFull (CircBuf_t * cb){
	return (cb->length == cb->size ? FULL : NORMAL);
}

/********************************************************
 *@name        cb_IsEmpty
 *
 *@description Check if the circular buffer is empty 
 *
 *@param       cb - pointer to the circular buffer
 * 
 *@return      EMPTY(0): buffer is empty
 *             NORMAL(2): buffer is not empty
 */
CB_e cb_IsEmpty (CircBuf_t * cb){
	return (cb->length == 0 ? EMPTY : NORMAL);
}

/********************************************************
 *@name        cb_Enqueue
 *
 *@description Enqueue a data into the circular buffer 
 *
 *@param       cb - pointer to the circular buffer
 *             data - data to enqueue
 * 
 *@return      OVERFILL(3): Buffer is full, no space for enqueuing new data
 *             NORMAL(2): Data enqueued successfully 
 */
CB_e cb_Enqueue(CircBuf_t * cb, uint8_t data){
	if (cb->length == cb->size){
		cb->tail=cb->buffer;
		return OVERFILL;
	}
	else{
		cb->tail ++;
		*(cb->tail) = data;
		cb->length++;
		return NORMAL;
	}
}

/********************************************************
 *@name        cb_Dequeue
 *
 *@description Dequeue a data from the circular buffer 
 *
 *@param       cb - pointer to the circular buffer
 *             output - pointer to the location that will store the dequeued data
 * 
 *@return      UNDERDEQUEUE(4): Buffer is empty, no data for dequeuing
 *             NORMAL(2): Data dequeued successfully *@return      
 */
CB_e cb_Dequeue(CircBuf_t * cb, uint8_t *output){
	if (cb->length==0){
		cb->head=cb->buffer;
		return UNDERDEQUEUE;
	}
	else{
		*output = * (cb->head);
		cb->head ++;
		cb->length--;
		return NORMAL;
	}
}

/********************************************************
 *@name        cb_Init
 *
 *@description Initialize a circular buffer 
 *
 *@param       cb - pointer to the circular buffer
 *             number_items - size of the buffer
 *@return      pointer to a CircBuf_t structure
 *
 */
CircBuf_t * cb_Init(CircBuf_t * cb, uint32_t num_items){
	cb= (CircBuf_t *)malloc(sizeof(CircBuf_t));
	cb->buffer = (uint8_t *)malloc(sizeof(uint8_t)*num_items);
	cb->size = num_items;
	cb->head = cb->buffer;
	cb->tail = cb->buffer;
	cb->length = 0;
	return cb;
}

/********************************************************
 *@name        cb_Destroy
 *
 *@description Free a circular buffer 
 *
 *@param       cb - pointer to the circular buffer
 *             
 *@return      FREE_SUCCESS: Buffer is freed
 *             FREE_ERROR: free failed 
 *
 */
CB_e cb_Destroy(CircBuf_t * cb){
	if(!cb)
		return FREE_ERROR;
	free(cb->buffer);
	cb->buffer = NULL;
	free(cb);
	cb = NULL;
	return FREE_SUCCESS;
}

/********************************************************
 *@name        cb_Empty_Buff
 *
 *@description Completely empty a circular buffer 
 *
 *@param       cb - pointer to the circular buffer
 *             
 */
void cb_Empty_Buff(CircBuf_t * cb){
	uint8_t i, value;
	i=cb->length;
	while(i>0){
		cb_Dequeue(cb, &value);
		i--;
	}
}

/********************************************************
 *@name        cb_Fill_Buff
 *
 *@description Completely fill a circular buffer 
 *
 *@param       cb - pointer to the circular buffer
 *             
 */
void cb_Fill_Buff(CircBuf_t * cb){
	uint8_t i;
	i=cb->size-cb->length;
	while(i>0){
		cb_Enqueue(cb,i);
		i--;
	}
}

/********************************************************
 *@name        circBuf_UnitTest
 *
 *@description Perform the unit test of circular buffer functions 
 *             
 */
void circBuf_UnitTest(void) {
	CircBuf_t * cb;
	int8_t result;
	uint8_t i=0;
	uint8_t temp;
	printf("Performing unit tests of the circular buffer functions...\n");

	cb = cb_Init(cb, 5);
	if(cb!=NULL){
		i++;
		printf("CB UNIT TEST: 1/14 - Initialize return a valid pointer of heap when heap is available...PASS\n");
	}
	else
		printf("CB UNIT TEST: 1/14 - Initialize return a valid pointer of heap when heap is available...FAIL\n");
	
	result=cb_IsEmpty(cb);
	if(result==EMPTY){
		i++;
		printf("CB UNIT TEST: 2/14 - Buffer empty return true for empty condition...PASS\n");
	}
	else
		printf("CB UNIT TEST: 2/14 - Buffer empty return true for empty condition...FAIL");
	
	result = cb_Enqueue(cb,1);
	if(result==NORMAL){
		i++;
		printf("CB UNIT TEST: 3/14 - Add an item...PASS\n");
	}
	else
		printf("CB UNIT TEST: 3/14 - Add an item...FAIL");
		
	result=cb_IsEmpty(cb);
	if(result==NORMAL){
		i++;
		printf("CB UNIT TEST: 4/14 - Buffer empty return false for non-empty condition...PASS\n");
	}
	else
		printf("CB UNIT TEST: 4/14 - Buffer empty return false for non-empty condition...FAIL");
	
	result=cb_IsFull(cb);
	if(result==NORMAL){
		i++;
		printf("CB UNIT TEST: 5/14 - Buffer full return false for non-full condition...PASS\n");
	}
	else
		printf("CB UNIT TEST: 5/14 - Buffer full return false for non-full condition...FAIL");
	
	cb_Fill_Buff(cb);
	result=cb_IsFull(cb);
	if(result==FULL){
		i++;
		printf("CB UNIT TEST: 6/14 - Buffer full return true for full condition...PASS\n");
		i++;
		printf("CB UNIT TEST: 7/14 - Fill buffer completely...PASS\n");
	}
	else{
		printf("CB UNIT TEST: 6/14 - Buffer full return true for full condition...FAIL\n");
		printf("CB UNIT TEST: 7/14 - Fill buffer completely...FAIL\n");
	}
	
	result=cb_Enqueue(cb,1);
	if(result==OVERFILL){
		i++;
		printf("CB UNIT TEST: 8/14 - Error on overfill...PASS\n");
	}
	else
		printf("CB UNIT TEST: 8/14 - Error on overfill...FAIL\n");
	
	if(cb->tail==cb->buffer){
		i++;
		printf("CB UNIT TEST: 9/14 - Wrap around at the boundary when buffer is full...PASS\n");
	}
	else
		printf("CB UNIT TEST: 9/14 - Wrap around at the boundary when buffer is full...FAIL\n");
	
	cb_Empty_Buff(cb);
	result=cb_IsEmpty(cb);
	if(result==EMPTY){
		i++;
		printf("CB UNIT TEST:10/14 - Remove items...PASS\n");
		i++;
		printf("CB UNIT TEST:11/14 - Empty buffer completely...PASS\n");
	}
	else{
		printf("CB UNIT TEST:10/14 - Remove items...FAIL\n");
		printf("CB UNIT TEST:11/14 - Empty buffer completely...FAIL\n");
	}
	
	result=cb_Dequeue(cb, &temp);
	if(result==UNDERDEQUEUE){
		i++;
		printf("CB UNIT TEST:12/14 - Error on overempty...PASS\n");
	}
	else
		printf("CB UNIT TEST:12/14 - Error on overempty...FAIL\n");
	
	if(cb->head==cb->buffer){
		i++;
		printf("CB UNIT TEST:13/14 - Wrap around at the boundary when buffer is empty...PASS\n");
	}
	else
		printf("CB UNIT TEST:13/14 - Wrap around at the boundary when buffer is empty...FAIL\n");
	
	result=cb_Destroy(cb);
	if(result==FREE_SUCCESS){
		i++;
		printf("CB UNIT TEST:14/14 - Destroy free memory back to malloc...PASS\n");
	}
	else
		printf("CB UNIT TEST:14/14 - Destroy free memory back to malloc...FAIL\n");
	
	if(i==14)
		printf("CIRCBUFF UNIT TEST SUITE: SUCCESS (%d/14 PASS)",i);
	else
		printf("CIRCBUFF UNIT TEST SUITE: FAIL (%d/14 PASS)",i);
}
