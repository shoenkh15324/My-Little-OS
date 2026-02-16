#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-10
 ******************************************************************************/
#include "core/systemDefs.h"

#if defined (APP_SAMPLE)
    #include "app/sample/app.h"
#endif

int appCommonClose(void);
int appCommonOpen(void);
