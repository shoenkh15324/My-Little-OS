#pragma once
/******************************************************************************
 *  Author : Mingyu Kim
 *  Created: 2026-02-12
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#include "core/sysDefs.h"

typedef struct{
    uint8_t* pBuf;
    size_t size, head, tail, usage;
    bool lock;
#if APP_BUFFER_STATISTICS
    size_t totalPushCount, totalPopCount, maxUsage, discardCount, pushFailCount, errorCount;
#endif
} ringBuffer;

int bufferOpen(ringBuffer*, size_t);
int bufferClose(ringBuffer*);
int bufferReset(ringBuffer*);
int bufferCanPush(ringBuffer*, size_t);
int bufferPush(ringBuffer*, uint8_t*, size_t);
size_t bufferPop(ringBuffer*, uint8_t*, size_t);
#if APP_BUFFER_PEAK
size_t bufferPeak(ringBuffer*, uint8_t*, size_t);
#endif
#if APP_BUFFER_STATISTICS
size_t bufferGetTotalPushCount(ringBuffer*);
size_t bufferGetTotalPopped(ringBuffer*);
size_t bufferGetCurrentUsage(ringBuffer*);
size_t bufferGetMaxUsage(ringBuffer*);
size_t bufferGetDiscardCount(ringBuffer*);
size_t bufferGetPushFailCount(ringBuffer*);
#endif

#ifdef __cplusplus
}
#endif
