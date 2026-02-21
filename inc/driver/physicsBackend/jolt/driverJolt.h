#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-21
 ******************************************************************************/
#include "core/feature/osal.h"

enum{
    driverJoltSyncXXX = objSyncBegin,
};
enum{
    driverJoltStateXXX = objStateBegin,
};

typedef struct driverJolt{
    objectState objState;
    osalMutex objMutex;
} driverJolt;

int driverJoltOpen(void);
int driverJoltClose(void);
int driverJoltSync(uint16_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);
