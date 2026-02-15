#pragma once
/******************************************************************************
 *  Author : Mingyu Kim
 *  Created: 2026-02-10
 ******************************************************************************/
#if defined (APP_SAMPLE)
    #include "app/sample/app.h"
#endif

typedef enum{
    appMainEventStart = objSyncBegin,
        appMainEventTimer,
    appMainEventEnd,
    appTestEventStart,
        appTestEventTimer,
    appTestEventEnd,
    
} appEventList;

int appCommonClose(void);
int appCommonOpen(void);
