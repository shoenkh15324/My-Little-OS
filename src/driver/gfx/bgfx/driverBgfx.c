/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-21
 ******************************************************************************/
#include "driver/driverCommon.h"
#if APP_DRIVER_GFX == DRIVER_GFX_BGFX
#include <bgfx/c99/bgfx.h>

static driverBgfx _driverBgfx = {
    .objState = objStateClosed,
};

int driverBgfxClose(void){
    int result = retOk;
    if(_driverBgfx.objState >= objStateOpening){
        osalMutexLock(&_driverBgfx.objMutex, -1);
        _driverBgfx.objState = objStateClosing;
        //
        _driverBgfx.objState = objStateClosed;
closeExit:
        osalMutexUnlock(&_driverBgfx.objMutex);
    }
    return result;
}
int driverBgfxOpen(void){
    int result = retOk;
    osalMutexOpen(&_driverBgfx.objMutex);
    osalMutexLock(&_driverBgfx.objMutex, -1);
    _driverBgfx.objState = objStateOpening;
    //
    _driverBgfx.objState = objStateOpened;
openExit:
    osalMutexUnlock(&_driverBgfx.objMutex);
    return result;
}
int driverBgfxSync(uint16_t sync, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3, uintptr_t arg4){
    if(_driverBgfx.objState < objStateOpened){ logError("objState(%d) < objStateOpened", _driverBgfx.objState); return retFail; }
    int result = retOk;
    osalMutexLock(&_driverBgfx.objMutex, -1);
    switch(sync){
        //
    }
syncExit:
    osalMutexUnlock(&_driverBgfx.objMutex);
    return result;
}
#endif
