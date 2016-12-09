#include "includes.h"

void SYS_EnterCritical(void)
{
	SYS_DisableIRQGlobal();
}

void SYS_ExitCritical(void)
{
	SYS_EnableIRQGlobal();
}

system_status_t SYS_MutexCreate(mutex_t *pMutex)
{
    assert(pMutex);

    pMutex->isLocked  = false;
    pMutex->isWaiting = false;
    pMutex->time_start = 0u;
    pMutex->timeout = 0u;

    return status_SYS_Success;
}

system_status_t SYS_MutexLock(mutex_t *pMutex, uint32_t timeout)
{
		uint32_t currentTime;
		assert(pMutex);

    /* Always check first. Deal with timeout only if not available. */
    if (pMutex->isLocked == false)
    {
        /* Get the lock and return success */
        SYS_DisableIRQGlobal();
		pMutex->isLocked = true;
		pMutex->isWaiting = false;
        SYS_EnableIRQGlobal();
        return status_SYS_Success;
    }
    else
    {
			if(0 == timeout)
			{
				return status_SYS_Timeout;
			}
			else if(pMutex->isWaiting)
			{
				currentTime = SYS_TimeGetMsec();
				if(pMutex->timeout < SYS_TimeDiff(pMutex->time_start, currentTime))
				{
					SYS_DisableIRQGlobal();
					pMutex->isWaiting = false;
					SYS_EnableIRQGlobal();
					return status_SYS_Timeout;
				}
			}
			else if(timeout != SYS_WAIT_FOREVER)
			{
				SYS_DisableIRQGlobal();
				pMutex->isWaiting = true;
				SYS_EnableIRQGlobal();
				pMutex->time_start = SYS_TimeGetMsec();
				pMutex->timeout =timeout;
			}
		}
		return status_SYS_Idle;
}

system_status_t SYS_MutexUnlock(mutex_t *pMutex)
{
    assert(pMutex);

    SYS_DisableIRQGlobal();
    pMutex->isLocked = false;
    SYS_EnableIRQGlobal();

    return status_SYS_Success;
}

system_status_t SYS_SemaphoreCreate(semaphore_t *pSem, uint8_t initValue)
{
    assert(pSem);

    pSem->semCount  = initValue;
    pSem->isWaiting = false;
    pSem->time_start = 0u;
    pSem->timeout = 0u;

    return status_SYS_Success;
}

system_status_t SYS_SemaWait(semaphore_t *pSem, uint32_t timeout)
{
		uint32_t currentTime;
    assert(pSem);

    /* Check the sem count first. Deal with timeout only if not already set */
    if (pSem->semCount)
    {
        SYS_DisableIRQGlobal();
        pSem->semCount --;
        pSem->isWaiting = false;
        SYS_EnableIRQGlobal();
        return status_SYS_Success;
    }
    else
    {
        if (0 == timeout)
        {
            /* If timeout is 0 and semaphore is not available, return kStatus_OSA_Timeout. */
            return status_SYS_Timeout;
        }
				else if(pSem->isWaiting)
				{
					currentTime = SYS_TimeGetMsec();
					if(pSem->timeout <SYS_TimeDiff(pSem->time_start, currentTime))
					{
						SYS_DisableIRQGlobal();
						pSem->isWaiting = false;
						SYS_EnableIRQGlobal();
						return status_SYS_Timeout;
					}
				}
				else if (timeout != SYS_WAIT_FOREVER)
				{
					SYS_DisableIRQGlobal();
					pSem->isWaiting = true;
					SYS_EnableIRQGlobal();
					pSem->time_start = SYS_TimeGetMsec();
					pSem->timeout = timeout;
				}
    }
    return status_SYS_Idle;
}

system_status_t SYS_SemaPost(semaphore_t *pSem)
{
    assert(pSem);
    /* The max value is 0xFF */
    if (0xFF == pSem->semCount)
    {
        return status_SYS_Error;
    }
    SYS_DisableIRQGlobal();
    ++pSem->semCount;
    SYS_EnableIRQGlobal();

    return status_SYS_Success;
}

system_status_t SYS_SemaDestroy(semaphore_t *pSem)
{
    assert(pSem);

    return status_SYS_Success;
}

msg_queue_handler_t SYS_MsgQueueCreate(msg_queue_t *queue, uint8_t queue_size)
{
	assert(queue);
	
	queue->size = queue_size;
	queue->queueMem = (uint8_t *)malloc(sizeof(uint8_t)*queue_size);
	queue->head = 0;
	queue->tail = 0;
	queue->isEmpty = true;
	
	return queue;
}

system_status_t SYS_MsgEnqueue(msg_queue_handler_t handler, void* pMsg)
{
	assert(handler);
	
	uint8_t *pFrom;
	uint8_t *pTo;
	
	/* Prevent the enqueue process from being interrupted */
	SYS_DisableIRQGlobal();
	
	/* Check if there is room in the queue for new message */
	if((handler->tail != handler->head) || (handler->isEmpty))
	{
		pFrom = (uint8_t *) pMsg;
		pTo	= &handler->queueMem[handler->tail];
		//pTo	= handler->queueMem;
		
		*pTo = *pFrom;
		
		++handler->tail;
		
	/* Wrap the tail pointer in case the end of the buffer is reached */
		if(handler->tail == handler->size)
		{
			handler->tail = 0;
		}
		
	/* If the queue was empty, signal the empty flag */
		if(handler->isEmpty)
		{
			handler->isEmpty = false;
		}
		
		SYS_EnableIRQGlobal();
		return status_SYS_Success;
	}
	else
	{
		SYS_EnableIRQGlobal();
		return status_SYS_Error;
	}
	
}

system_status_t SYS_MsgDequeue(msg_queue_handler_t handler, void* pMsg)
{
	assert(handler);
	
	uint8_t *pFrom, *pTo;
	
	/* Prevent the enqueue process from being interrupted */
	SYS_DisableIRQGlobal();
	
	/* Check if the queue is not empty */
	if(!handler->isEmpty)
	{
		pFrom = &handler->queueMem[handler->head];
		pTo = (uint8_t*)pMsg;
		
		*pTo = *pFrom;
		
		++handler->head;
		
	/* Wrap the head pointer in case the end of the buffer is reach */
		if(handler->head == handler->size)
		{
			handler->head = 0;
		}
	
	/*If queue is empty, signal the empty flag */
		if(handler->head == handler->tail)
		{
			handler->isEmpty = true;
		}
		
		SYS_EnableIRQGlobal();
		return status_SYS_Success; 
	}
	else
	{
		SYS_EnableIRQGlobal();
		return status_SYS_Error;
	}
}

uint32_t SYS_TimeDiff(uint32_t time_start, uint32_t time_end)
{
	if(time_end >= time_start)
	{
		return (time_end-time_start);
	}
	else
	{
		return SYS_TIME_RANGE - time_start + time_end + 1; 
	}
}

uint32_t SYS_TimeGetMsec(void)
{
	return LPTMR_Hal_GetCounterValue();
}

void SYS_TimeDelay(uint32_t delay)
{
	uint32_t currTime, timeStart;
	timeStart = SYS_TimeGetMsec();
	do
	{
		currTime = SYS_TimeGetMsec();
	}while(delay >= SYS_TimeDiff(timeStart, currTime));
}
