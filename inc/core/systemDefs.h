#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-14
 ******************************************************************************/
// C Standard Library
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* [SYSTEM INFO] */
#define SYSTEM_NAME "My Little OS"
#define SYSTEM_AUTHOR "Minkyu Kim"
#define SYSTEM_VERSION_MAJOR 0
#define SYSTEM_VERSION_MINOR 1
#define SYSTEM_VERSION_PATCH 0

/* [MCU] */
#define MCU_NONE 0
#define MCU_STM32F103C8T6 1
#define MCU_STM32F411CEU6 2

/* [BOARD] */
#define BOARD_NONE 0

/* [OS] */
#define OS_NONE 0
#define OS_BAREMETAL 1
#define OS_FREERTOS 2
#define OS_ZEPHYR 3
#define OS_LINUX 4

/* [SDK] */
#define SDK_NONE 0
#define SDK_STM32_HAL 1
#define SDK_POSIX 2

/* [LOG] */
#define SYSTEM_LOG_ENABLE 1
#define SYSTEM_LOG_PREFIX "MARIO"
#define SYSTEM_LOG_BACKEND_PRINTF 1
#define SYSTEM_LOG_BACKEND_PRINTK 2
#define SYSTEM_LOG_BACKEND_UART 3
#define SYSTEM_LOG_MODE_SYNC 0
#define SYSTEM_LOG_MODE_ASYNC 1
#define SYSTEM_LOG_LEVEL_ERROR 1
#define SYSTEM_LOG_LEVEL_WARN 2
#define SYSTEM_LOG_LEVEL_INFO 3
#define SYSTEM_LOG_LEVEL_DEBUG 4
#define SYSTEM_LOG_LEVEL_NONE 5
#define SYSTEM_LOG_COLOR_ENABLE 1
#define SYSTEM_LOG_TIMESTAMP_ENABLE 1

/* [BUFFER] */
#define SYSTEM_BUFFER_PEAK_ENABLE 1
#define SYSTEM_BUFFER_PUSH_OVERWRITE_ENABLE 1
#define SYSTEM_BUFFER_STATISTICS_ENABLE 1
#define SYSTEM_BUFFER_LOCK_ENABLE 1

/* [OSAL] */
// THREAD
#define SYSTEM_OSAL_THREAD_ENABLE 1
// TIMER
#define SYSTEM_OSAL_TIMER_ENABLE 1
// MEMORY
#define SYSTEM_OSAL_STATIC_MEM 1
#define SYSTEM_OSAL_DYNAMIC_MEM 2
// MUTEX
#define SYSTEM_OSAL_MUTEX_ENABLE 1
#define SYSTEM_OSAL_MUTEX_TYPE_NORMAL 1
#define SYSTEM_OSAL_MUTEX_TYPE_RECURSIVE 2
// SEMAPHORE
#define SYSTEM_OSAL_SEMAPHORE_ENABLE 1
#define SYSTEM_OSAL_SEMAPHORE_TYPE_BINARY 1
#define SYSTEM_OSAL_SEMAPHORE_TYPE_COUNTING 2
// EPOLL
#define SYSTEM_OSAL_EPOLL_ENABLE 1

typedef enum{
    retOk = 0,
    retFail = -1,
    retTimeout = -2,
    retInvalidParam = -3,
    retEmptyBuffer = -4,
} returnCode;

typedef enum{
    objStateClosed = 0,
    objStateClosing,
    objStateOpening,
    objStateOpened,
    objStateBegin,
} objectState;
typedef enum{
    objSyncBegin = 0,
} objectSync;

