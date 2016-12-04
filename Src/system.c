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

system_status_t SYS_MutexLock(mutex_t *pMutex)
{
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
		return status_SYS_Idle;
	}
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

msg_queue_handler_t SYS_MsgQueueCreate(msg_queue_t *queue, uint16_t queue_size)
{
	assert(queue);
	
	queue->size = queue_size;
	queue->head = 0;
	queue->tail = 0;
	queue->isEmpty = true;
	
	return queue;
}

system_status_t SYS_MsgEnqueue(msg_queue_handler_t handler, void* pMsg)
{
	assert(handler);
	
	uint32_t *pFrom, *pTo;
	
	/* Prevent the enqueue process from being interrupted */
	SYS_DisableIRQGlobal();
	
	/* Check if there is room in the queue for new message */
	if((handler->tail != handler->head) || (handler->isEmpty))
	{
		pFrom = (uint32_t *) pMsg;
		pTo	= &handler->queueMem[handler->tail];
		
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
	
	uint32_t *pFrom, *pTo;
	
	/* Prevent the enqueue process from being interrupted */
	SYS_DisableIRQGlobal();
	
	/* Check if the queue is not empty */
	if(!handler->isEmpty)
	{
		pFrom = &handler->queueMem[handler->head];
		pTo = (uint32_t*)pMsg;
		
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
		
		SYS_EableIRQGlobal();
		return status_SYS_Success; 
	}
	else
	{
		SYS_EableIRQGlobal();
		return status_SYS_Error;
	}
}
