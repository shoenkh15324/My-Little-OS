#pragma once
/******************************************************************************
 *  Author : Mingyu Kim
 *  Created: 2026-02-14
 ******************************************************************************/
#include "core/system.h"

typedef struct{
    activeObject actObj;
    osalThread appThread;
    osalThreadAttribute appThreadAttr;
    osalTimer appTimer;
    uint8_t payloadBuf[APP_THREAD_EVENT_PAYLOAD_BUFFER_SIZE];
} appMain;

int appClose(void);
int appOpen(void);
int appSync(uint16_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);
