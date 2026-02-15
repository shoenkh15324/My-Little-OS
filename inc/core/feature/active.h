#pragma once
/******************************************************************************
 *  Author : Mingyu Kim
 *  Created: 2026-02-15
 ******************************************************************************/

#include "core/feature/async.h"
#include "core/sysDefs.h"
#include "core/feature/buffer.h"
#include "core/feature/osal.h"

typedef struct activeObject {
    objectState objState;
    // Synchronization
    osalMutex objMutex;
    osalSemaphore objSema;
#if APP_OS == OS_LINUX
    osalEpoll objEpoll;
#endif
    // Event Queue
    ringBuffer eventQueue;
    size_t eventQueueSize;
    // Thread
    osalThread appThread;
    osalThreadAttribute appThreadAttr;
    void (*appThreadHandler)(struct activeObject* actor, asyncPacket* async, uint8_t* payload);
    // Timer
    osalTimer appTimer;
    void (*appTimerHandler)(struct activeObject* actor);
    uint64_t appTimerCount;
    // Etc
    bool isMainThread;
    uint8_t *pPayloadBuffer;
    uint16_t appEventIdxStart, appEventIdxEnd;
    size_t payloadBufferSize;
    void (*appOnOpenHandler)(struct activeObject* actor);
} activeObject ;

int activeOpen(activeObject*);
int activeClose(activeObject*);
