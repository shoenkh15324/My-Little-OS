/******************************************************************************
 *  Author : Mingyu Kim
 *  Created: 2026-02-13
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>
#include "core/osal.h"
#include "core/system.h"
#if APP_OS == OS_LINUX
    #include <unistd.h>
    #include <sys/time.h>
    #include <errno.h>
#endif

// Time / Tick
void osalGetDate(char* buf, size_t bufSize){
    checkParamsVoid(buf, bufSize);
#if APP_OS == OS_LINUX
    struct timeval  time;
    gettimeofday(&time, NULL);
    struct tm* tmInfo = localtime(&time.tv_sec);
    snprintf(buf, bufSize, "%02d:%02d:%02d.%03ld", tmInfo->tm_hour, tmInfo->tm_min, tmInfo->tm_sec, time.tv_usec  / 1000);
#endif
}
int osalGetTimeMs(void){
#if APP_OS == OS_LINUX
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (int)((time.tv_sec * 1000) + (time.tv_nsec / 1000000));
#else 
    return 0;
#endif
}
int64_t osalGetTimeUs(void){
#if APP_OS == OS_LINUX
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (int64_t)time.tv_sec * 1000000LL + (int64_t)(time.tv_nsec / 1000);
#else
    return 0;
#endif
}
int64_t osalGetTimeNs(void){
#if APP_OS == OS_LINUX
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time.tv_sec * 1000000000L + time.tv_nsec;
#else
    return 0;
#endif
}
int osalGetTick(void){
    //
    return 0;
}
void osalDelayMs(int ms){
#if APP_OS == OS_LINUX
    usleep(ms * 1000);
#endif
}
void osalDelayUs(int us){
#if APP_OS == OS_LINUX // Note: In Linux user-space, microsecond delays are not precise due to scheduler and timer resolution.
    usleep(us);
#endif
}

// Timer
#if (APP_TIMER == SYSTEM_OSAL_TIMER_ENABLE) && (APP_OS == OS_LINUX)
static void _timerCallbackWrapper(union sigval sigval){
    osalTimer* pTimer = (osalTimer*)sigval.sival_ptr;
    if(pTimer && pTimer->userCb) pTimer->userCb(pTimer->userArg);
}
#endif
int osalTimerOpen(osalTimer* handle, osalTimerCb expiredCallback, int periodMs){
#if APP_TIMER == SYSTEM_OSAL_TIMER_ENABLE
    checkParams(handle, expiredCallback, periodMs);
    handle->userCb = expiredCallback;
    handle->userArg = handle;
    #if APP_OS == OS_LINUX
        memset(&handle->signal, 0, sizeof(struct sigevent));
        handle->signal.sigev_notify = SIGEV_THREAD;
        handle->signal.sigev_notify_function = _timerCallbackWrapper;
        handle->signal.sigev_value.sival_ptr = (void*)handle;
        if(timer_create(CLOCK_REALTIME, &handle->signal, &handle->timerId)){ logError("timer_create fail");
            return retFail;
        }
        struct itimerspec timerSpec;
        timerSpec.it_value.tv_sec = periodMs / 1000;
        timerSpec.it_value.tv_nsec = (periodMs % 1000) * 1000000;
        timerSpec.it_interval.tv_sec = timerSpec.it_value.tv_sec;
        timerSpec.it_interval.tv_nsec = timerSpec.it_value.tv_nsec;
        if(timer_settime(handle->timerId, 0, &timerSpec, NULL) == -1){ logError("timer_settime fail");
            timer_delete(handle->timerId);
            return retFail;
        }
    #endif
#endif
    return retOk;
}
int osalTimerClose(osalTimer* handle){
#if APP_TIMER == SYSTEM_OSAL_TIMER_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        if(timer_delete(handle->timerId)){ logError("timer_delete fail");
            return retFail;
        }
    #endif
#endif
    return retOk;
}

// Memory
#if APP_MEM == SYSTEM_OSAL_STATIC_MEM
static alignas(APP_MEM_ALIGNMENT) uint8_t _memPool[APP_MEM_POOL_SIZE] = {0};
static bool _memBlockUsed[APP_MEM_BLOCK_COUNT] = {0}; // 0 = free, 1 = used
#endif
int osalMalloc(void** handle, size_t size){
    checkParams(handle, size);
#if APP_OS == OS_LINUX
    #if APP_MEM == SYSTEM_OSAL_DYNAMIC_MEM
        *handle = malloc(size);
        if(*handle == NULL){ logError("malloc fail");
            return retFail;
        }
    #elif APP_MEM == SYSTEM_OSAL_STATIC_MEM
        if(size > APP_MEM_BLOCK_SIZE){ logError("Requested size too large (%zu > %d)", size, APP_MEM_BLOCK_SIZE);
            return retFail;
        }
        for(int i = 0; i < APP_MEM_BLOCK_COUNT; i++){
            if(!_memBlockUsed[i]){
                _memBlockUsed[i] = true;
                *handle = &_memPool[i * APP_MEM_BLOCK_SIZE];
                return retOk;
            }
        }
        logError("Insufficient memory pool size");
        return retFail;
    #endif
#else
    //
#endif
    return retOk;
}
int osalFree(void* handle){
    checkParams(handle);
#if APP_OS == OS_LINUX
    #if APP_MEM == SYSTEM_OSAL_DYNAMIC_MEM
        free(handle);
    #elif APP_MEM == SYSTEM_OSAL_STATIC_MEM
        uint8_t* ptr = (uint8_t*)handle;
        ptrdiff_t offset = ptr - _memPool;
        if(ptr < _memPool || offset >= APP_MEM_POOL_SIZE) { logError("Out of memory pool range");
            return retFail;
        }
        if(!(offset % APP_MEM_BLOCK_SIZE)){
            int index = (int)(offset / APP_MEM_BLOCK_SIZE);
            _memBlockUsed[index] = false;
        }else{ logError("Invalid memory offset (misaligned or corrupted)");
            return retFail;
        }
    #endif
#else
    //
#endif
    return retOk;
}

// Thread
int osalThreadOpen(osalThread* handle, const osalThreadAttribute* attr, oslThreadEntry threadEntryCb, void* userArg){
#if APP_THREAD == SYSTEM_OSAL_THREAD_ENABLE
    checkParams(handle, attr, threadEntryCb);
    #if APP_OS == OS_LINUX
        pthread_attr_t threadAttrLinux;
        if(pthread_attr_init(&threadAttrLinux)){ logError("pthread_attr_init fail");
            return retFail;
        }
        if(attr->statckSize > 0){
            if(pthread_attr_setstacksize(&threadAttrLinux, attr->statckSize)){ logError("pthread_attr_setstacksize fail");
                pthread_attr_destroy(&threadAttrLinux);
                return retFail; 
            }
        }
        if(pthread_create(&(handle->thread), &threadAttrLinux, (void *(*)(void *))threadEntryCb, userArg)){ logError("pthread_create fail");
            pthread_attr_destroy(&threadAttrLinux);
            return retFail;
        }
        handle->isCreated = 1;
        pthread_attr_destroy(&threadAttrLinux);
    #endif
#endif
    return retOk;
}
int osalThreadSetPriority(osalThread* handle, osalThreadPriority priority){
#if APP_THREAD == SYSTEM_OSAL_THREAD_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        int policy = SCHED_OTHER;
        struct sched_param param;
        if(priority >= osalThreadPriorityHigh){
            policy = SCHED_FIFO;
            param.sched_priority = 10;
        }else{
            policy = SCHED_OTHER;
            param.sched_priority = 0;
        }
        if(pthread_setschedparam(handle->thread, policy, &param)){ logError("pthread_setschedparam fail");
            return retFail;
        }
    #endif
#endif
    return retOk;
}
int osalThreadJoin(osalThread* handle){
#if APP_THREAD == SYSTEM_OSAL_THREAD_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        if(handle->isCreated){
            if(pthread_join(handle->thread, NULL)){ logError("pthread_join fail");
                return retFail;
            }
        }
        handle->isCreated = 0;
    #endif
#endif
    return retOk;
}
int osalThreadClose(osalThread* handle){
#if APP_THREAD == SYSTEM_OSAL_THREAD_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        if(handle->isCreated) pthread_detach(handle->thread);
        handle->thread = 0;
        handle->isCreated = 0;
    #endif
#endif
    return retOk;
}

// Mutex
int osalMutexOpen(osalMutex* handle){
#if APP_MUTEX == SYSTEM_OSAL_MUTEX_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        pthread_mutexattr_t attr;
        if(pthread_mutexattr_init(&attr)){ logError("pthread_mutexattr_init fail");
            return retFail;
        }
        int mutexType = PTHREAD_MUTEX_NORMAL;
        #if APP_MUTEX_TYPE_RECURSIVE == SYSTEM_OSAL_MUTEX_TYPE_RECURSIVE
            mutexType = PTHREAD_MUTEX_RECURSIVE;
        #endif
        if(pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE)){ logError("pthread_mutexattr_settype fail");
            pthread_mutexattr_destroy(&attr);
            return retFail;
        }
        if(pthread_mutex_init(&(handle->mutex), &attr)){ logError("pthread_mutex_init fail");
            return retFail;
        }
        if(pthread_mutexattr_destroy(&attr)){ logError("pthread_mutexattr_destroy fail");
            return retFail;
        }
    #endif
#endif
    return retOk;
}
int osalMutexClose(osalMutex* handle){
#if APP_MUTEX == SYSTEM_OSAL_MUTEX_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        if(pthread_mutex_destroy(&(handle->mutex))){ logError("pthread_mutex_destroy fail");
            return retFail;
        }
    #endif
#endif
    return retOk;
}
int osalMutexLock(osalMutex* handle){
#if APP_MUTEX == SYSTEM_OSAL_MUTEX_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        #if defined(APP_MUTEX_LOCK_TIMEOUT_MS) && (APP_MUTEX_LOCK_TIMEOUT_MS > 0)
            struct timespec timeSpec;
            if(clock_gettime(CLOCK_REALTIME, &timeSpec)){ logError("clock_gettime fail");
                return retFail;
            }
            timeSpec.tv_sec += (APP_MUTEX_LOCK_TIMEOUT_MS / 1000);
            timeSpec.tv_nsec += (APP_MUTEX_LOCK_TIMEOUT_MS % 1000) * 1000000;
            if(timeSpec.tv_nsec >= 1000000000){
                timeSpec.tv_sec += 1;
                timeSpec.tv_nsec -= 1000000000;
            }
            int res = pthread_mutex_timedlock(&(handle->mutex), &timeSpec);
            if(res != 0){
                if(res == ETIMEDOUT){
                    logError("osalMutexLock: timeout occurred");
                }else{
                    logError("osalMutexLock: fail");
                }
                return retFail;
            }
        #else
            if(pthread_mutex_lock(&(handle->mutex))){ logError("pthread_mutex_lock fail");
                return retFail;
            }
        #endif
    #endif
#endif
    return retOk;
}
int osalMutexUnlock(osalMutex* handle){
#if APP_MUTEX == SYSTEM_OSAL_MUTEX_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        if(pthread_mutex_unlock(&(handle->mutex))){ logError("pthread_mutex_unlock fail");
            return retFail;
        }
    #endif
#endif
    return retOk;
}

// Semaphore
int osalSemaphoreOpen(osalSemaphore* handle, int count){
#if APP_SEMAPHORE == SYSTEM_OSAL_SEMAPHORE_ENABLE
    checkParams(handle, count);
    unsigned int initCount = 0;
    #if APP_SEMAPHORE_TYPE == SYSTEM_OSAL_SEMAPHORE_TYPE_BINARY
        initCount = (count > 0) ? 1 : 0;
    #else
        initCount = (count == -1) ? APP_SEMAPHORE_MAX_COUNT : (unsigned int)count;
    #endif
    #if APP_OS == OS_LINUX
        if(sem_init(&(handle->sema), 0, initCount) != 0) {
            logError("sem_init fail");
            return retFail;
        }
    #endif
#endif
    return retOk;
}
int osalSemaphoreClose(osalSemaphore* handle){
#if APP_SEMAPHORE == SYSTEM_OSAL_SEMAPHORE_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        if(sem_destroy(&(handle->sema)) != 0){ logError("sem_destroy fail");
            return retFail;
        }
    #endif
#endif
    return retOk;
}
int osalSemaphoreTake(osalSemaphore* handle){
#if APP_SEMAPHORE == SYSTEM_OSAL_SEMAPHORE_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        #if defined(APP_SEMAPHORE_TIMEOUT_MS) && (APP_SEMAPHORE_TIMEOUT_MS > 0)
            struct timespec ts;
            if(clock_gettime(CLOCK_REALTIME, &ts) == -1){ logError("clock_gettime fail");
                return retFail;
            }
            ts.tv_sec += (APP_SEMAPHORE_TIMEOUT_MS / 1000);
            ts.tv_nsec += (APP_SEMAPHORE_TIMEOUT_MS % 1000) * 1000000;
            if(ts.tv_nsec >= 1000000000){
                ts.tv_sec += 1;
                ts.tv_nsec -= 1000000000;
            }
            if(!sem_timedwait(&(handle->sema), &ts)){
                if(errno == ETIMEDOUT){
                    logError("osalSemaphoreTake: timeout");
                }else{
                    logError("osalSemaphoreTake: fail");
                }
                return retFail;
            }
        #else
            if(sem_wait(&(handle->sema)) != 0){ logError("sem_wait fail");
                return retFail;
            }
        #endif
    #endif
#endif
    return retOk;
}
int osalSemaphoreGive(osalSemaphore* handle){
#if APP_SEMAPHORE == SYSTEM_OSAL_SEMAPHORE_ENABLE
    checkParams(handle);
    #if APP_OS == OS_LINUX
        if(sem_post(&(handle->sema)) != 0){ logError("sem_post fail");
            return retFail;
        }
    #endif
#endif
    return retOk;
}

// Etc
int osalIsInIsr(void){
#if APP_OS == OS_LINUX
    return 0; // User-space Linux has no direct ISR access.
#endif
}

#ifdef __cplusplus
}
#endif

