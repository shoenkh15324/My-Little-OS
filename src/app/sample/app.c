/******************************************************************************
 *  Author : Mingyu Kim
 *  Created: 2026-02-14
 ******************************************************************************/
#include "app/appCommon.h"

static appMain _appMain = {
    .actObj.objState = objStateClosed, 
    .appThreadAttr.name = "main",
    .appThreadAttr.priority = 0,
    .appThreadAttr.statckSize = APP_THREAD_STACK_SIZE,
};

static void _appEventHandler(asyncPacket* pAsync, uint8_t* pPayload){
    osalMutexLock(&_appMain.actObj.objMutex, -1);
    switch(pAsync->eventId){
        case appMainEventTimer: logDebug("appMainEventTimer");
            break;
    }
appEventHandlerExit:
    osalMutexUnlock(&_appMain.actObj.objMutex);
}
static void _appTimerHandler(void* arg){
    if(asyncPush(asyncTypeAsync, appMainEventTimer, NULL, NULL, NULL, NULL)){
        logError("asyncPush fail");
    }
}
static void _appThreadHandler(void){
    if(_appMain.actObj.objState < objStateOpened){ logError("objState(%d) < objStateOpened", _appMain.actObj.objState); return; }
    asyncPacket async;
    for(;;){
        if(osalSemaphoreTake(&_appMain.actObj.objSema, -1)){
            if(!asyncPop(&_appMain.actObj, &async, &_appMain.payloadBuf)){
                _appEventHandler(&async, &_appMain.payloadBuf);
            }
        }
    }
}
int appClose(void){
    int result = retOk;
    if(_appMain.actObj.objState >= objStateOpening){
        _appMain.actObj.objState = objStateClosing;
        osalMutexLock(&_appMain.actObj.objMutex, -1);
        //
        _appMain.actObj.objState = objStateClosed;
        osalMutexUnlock(&_appMain.actObj.objMutex);
    }
    return result;
}
int appOpen(void){
    int result = retOk;
    osalSemaphoreOpen(&_appMain.actObj.objSema, -1);
    osalMutexOpen(&_appMain.actObj.objMutex);
    osalMutexLock(&_appMain.actObj.objMutex, -1);
    _appMain.actObj.objState = objStateOpening;
    //
    if(bufferOpen(&_appMain.actObj.eventQueue, APP_THREAD_EVENT_QUEUE_SIZE)){ logError("bufferOpen fail");
        result = retFail; goto appOpenExit;
    }
    if(asyncSubscribe(&_appMain.actObj, appMainEventStart, appMainEventEnd)){ logError("asyncSubscribe fail");
        result = retFail; goto appOpenExit;
    }
    if(osalThreadOpen(&_appMain.appThread, &_appMain.appThreadAttr, _appThreadHandler, NULL)){ logError("osalThreadOpen fail");
        result = retFail; goto appOpenExit;
    }
    //
    _appMain.actObj.objState = objStateOpened;
appOpenExit:
    osalMutexUnlock(&_appMain.actObj.objMutex);
    //
    if(osalTimerOpen(&_appMain.appTimer, _appTimerHandler, APP_TIMER_INTERVAL)){ logError("osalTimerOpen fail");
        return retFail;
    }
    return result;
}
int appSync(uint16_t, uintptr_t, uintptr_t, uintptr_t, uintptr_t){
    return retOk;
}
