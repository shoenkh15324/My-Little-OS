#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-14
 ******************************************************************************/
#include "core/system.h"
#include "core/feature/active.h"

typedef enum{
    // appMain
    appMainEventStart = objSyncBegin,
        appMainEventTimer,
    appMainEventEnd = 99,
    // appTest
    appTestEventStart = 100,
        appTestEventTimer,
    appTestEventEnd = 199,
} appEventList;
typedef enum{
    appMainStateXXX = objStateBegin,
} appMainState;
typedef enum{
    appTestStateXXX = objStateBegin,
} appTestState;

typedef struct{
    activeObject actor;
} appMain;
typedef struct{
    activeObject actor;
} appTest;

int appClose(void);
int appOpen(void);
int appSync(uint16_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t);
