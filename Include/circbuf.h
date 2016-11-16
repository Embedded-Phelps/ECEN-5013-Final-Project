/***************************************************************************
 *
 *	Filename: 		circbuf.h
 *  Description:  	circular buffer function prototypes header file 
 *  Author: 		ShuTing Guo  
 *  Date: 			Oct. 2016
 *
 *****************************************************************************/
#ifndef CIRCBUF_H
#define CIRCBUF_H

#include "includes.h"

/********************************************************
 * Circular buffer structure declaration
 */
typedef struct CircBuf{
	uint8_t * buffer;	//pointer to the buffer in memory
	uint8_t * head;		//pointer to the oldest data
	uint8_t * tail;		//pointer to the newest data
	size_t	size;		//size of the buffer
	size_t  length;		//current number of items in the buffer
}CircBuf_t;

/********************************************************
 * Different circular buffer states 
 */
typedef enum CB_e{
	EMPTY, 
	FULL, 
	NORMAL, 
	OVERFILL, 
	UNDERDEQUEUE,
	FREE_ERROR,
	FREE_SUCCESS
}CB_e;

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
CB_e cb_IsFull (CircBuf_t * cb);

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
CB_e cb_IsEmpty (CircBuf_t * cb);

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
CB_e cb_Enqueue(CircBuf_t * cb, uint8_t data);

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
CB_e cb_Dequeue(CircBuf_t * cb, uint8_t *output);

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
CircBuf_t * cb_Init(CircBuf_t * cb, uint32_t num_items);

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
CB_e cb_Destroy(CircBuf_t * cb);

/********************************************************
 *@name        cb_Empty_Buff
 *
 *@description Completely empty a circular buffer 
 *
 *@param       cb - pointer to the circular buffer
 *             
 */
void cb_Empty_Buff(CircBuf_t * cb);

/********************************************************
 *@name        cb_Fill_Buff
 *
 *@description Completely fill a circular buffer 
 *
 *@param       cb - pointer to the circular buffer
 *             
 */
void cb_Fill_Buff(CircBuf_t * cb);

/********************************************************
 *@name        circBuf_UnitTest
 *
 *@description Perform the unit test of circular buffer functions 
 *             
 */
void circBuf_UnitTest(void);
#endif /* CIRCBUF_H*/
