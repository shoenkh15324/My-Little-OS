#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-21
 ******************************************************************************/
#include "core/feature/osal.h"

enum{
    driverBgfxSyncXXX = objSyncBegin,
};
enum{
    driverBgfxStateXXX = objStateBegin,
};

typedef struct driverBgfx{
    objectState objState;
    osalMutex objMutex;
} driverBgfx;

int driverBgfxOpen(void);
int driverBgfxClose(void);
int driverBgfxSync(uint16_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);
