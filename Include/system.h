#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "includes.h"

typedef enum system_status_t
{
    status_SYS_Success = 0U, /*!< Success */
    status_SYS_Error   = 1U, /*!< Failed */
    status_SYS_Timeout = 2U, /*!< Timeout occurs while waiting */
    status_SYS_Idle    = 3U  /*!< Used for bare metal only, the wait object is not ready
                                and timeout still not occur */
} system_status_t;

typedef struct semaphore
{
    volatile bool    isWaiting;  /*!< Is any task waiting for a timeout on this object */
    volatile uint8_t semCount;   /*!< The count value of the object                    */
    uint32_t         time_start; /*!< The time to start timeout                        */
    uint32_t         timeout;    /*!< Timeout to wait in milliseconds                  */
} semaphore_t;

typedef struct mutex
{
    volatile bool    isWaiting;  /*!< Is any task waiting for a timeout on this mutex */
    volatile bool    isLocked;   /*!< Is the object locked or not                     */
    uint32_t         time_start; /*!< The time to start timeout                       */
    uint32_t         timeout;    /*!< Timeout to wait in milliseconds                 */
} mutex_t;

typedef struct MsgQueue
{
    uint32_t              *queueMem;      /*!< Points to the queue memory               */
    uint16_t               size;          /*!< The size of the queue        */
    uint16_t               head;          /*!< Index of the next message to be read     */
    uint16_t               tail;          /*!< Index of the next place to write to      */
    volatile bool          isEmpty;       /*!< Whether queue is empty                   */
}msg_queue_t;

typedef msg_queue_t*	msg_queue_handler_t;

#define SYS_WAIT_FOREVER  0xFFFFFFFFU
#define SYS_TIME_RANGE 0xFFFFU

#if defined(__cplusplus)
extern "C" {
#endif

void SYS_EnterCritical(void);

void SYS_ExitCritical(void);



system_status_t SYS_MutexCreate(mutex_t *pMutex);

system_status_t SYS_MutexLock(mutex_t *pMutex, uint32_t timeout);

system_status_t SYS_MutexUnlock(mutex_t *pMutex);



system_status_t SYS_SemaphoreCreate(semaphore_t *pSem, uint8_t initValue);

system_status_t SYS_SemaWait(semaphore_t *pSem, uint32_t timeout);

system_status_t SYS_SemaPost(semaphore_t *pSem);

system_status_t SYS_SemaDestroy(semaphore_t *pSem);



msg_queue_handler_t SYS_MsgQueueCreate(msg_queue_t *queue, uint16_t queue_size);

system_status_t SYS_MsgEnqueue(msg_queue_handler_t handler, void* pMsg);

system_status_t SYS_MsgDequeue(msg_queue_handler_t handler, void* pMsg);

uint32_t SYS_TimeDiff(uint32_t time_start, uint32_t time_end);

uint32_t SYS_TimeGetMsec(void);

void SYS_TimeDelay(uint32_t delay);

__STATIC_INLINE void SYS_DisableIRQGlobal(void)
{
	__disable_irq();
}

__STATIC_INLINE void SYS_EnableIRQGlobal(void)
{
	__enable_irq();
}

#if defined(__cplusplus)
}
#endif

#endif
