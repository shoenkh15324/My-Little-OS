#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-13
 ******************************************************************************/

#include "core/systemDefs.h"

typedef enum{
    asyncTypeAsync = 0,
    asyncTypeAsyncPayload,
    asyncTypeAwait,
    asyncTypeExpress,
} asyncType;

typedef struct asyncPacket{
    asyncType type;
    uint16_t eventId;
    uintptr_t arg1, arg2, arg3, arg4;
    size_t payloadSize;
} __attribute__((packed)) asyncPacket;

typedef struct activeObject activeObject;

typedef struct asyncSubscriber{
    struct activeObject* pActObj;
    uint16_t startId, endId;
} asyncSubscriber;

int asyncSubscribe(struct activeObject*, uint16_t, uint16_t);
int asyncPush(asyncType, uint16_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);
size_t asyncPop(struct activeObject*, asyncPacket*, uint8_t*);
