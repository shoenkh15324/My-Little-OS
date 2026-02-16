#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-14
 ******************************************************************************/
// C Standard Library
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Option Propagation: defaults only if not defined by appConfig.h
#ifndef APP_OS
    #define APP_OS 0
#endif
#ifndef APP_LOG_ENABLE
    #define APP_LOG_ENABLE 0
#endif
#ifndef APP_LOG_LEVEL
    #define APP_LOG_LEVEL 0
#endif
#ifndef SYSTEM_LOG_LEVEL_ERROR
    #define SYSTEM_LOG_LEVEL_ERROR 1
#endif
#ifndef SYSTEM_LOG_LEVEL_WARN
    #define SYSTEM_LOG_LEVEL_WARN 2
#endif
#ifndef SYSTEM_LOG_LEVEL_INFO
    #define SYSTEM_LOG_LEVEL_INFO 3
#endif
#ifndef SYSTEM_LOG_LEVEL_DEBUG
    #define SYSTEM_LOG_LEVEL_DEBUG 4
#endif
#ifndef SYSTEM_LOG_LEVEL_NONE
    #define SYSTEM_LOG_LEVEL_NONE 5
#endif

#if APP_OS == OS_LINUX
    #define _GNU_SOURCE
#endif

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

