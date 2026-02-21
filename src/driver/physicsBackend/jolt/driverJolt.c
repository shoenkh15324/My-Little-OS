/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-21
 ******************************************************************************/
#include "driver/driverCommon.h"

#if APP_DRIVER_PHYSICS_BACKEND == DRIVER_PHYSICS_BACKEND_JOLT

static driverJolt _driverJolt = {
    .objState = objStateClosed,
};

int driverJoltClose(void){
    int result = retOk;
    if(_driverJolt.objState >= objStateOpening){
        osalMutexLock(&_driverJolt.objMutex, -1);
        _driverJolt.objState = objStateClosing;
        //
        _driverJolt.objState = objStateClosed;
closeExit:
        osalMutexUnlock(&_driverJolt.objMutex);
    }
    return result;
}
int driverJoltOpen(void){
    int result = retOk;
    osalMutexOpen(&_driverJolt.objMutex);
    osalMutexLock(&_driverJolt.objMutex, -1);
    _driverJolt.objState = objStateOpening;
    //
    _driverJolt.objState = objStateOpened;
openExit:
    osalMutexUnlock(&_driverJolt.objMutex);
    return result;
}
int driverJoltSync(uint16_t sync, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3, uintptr_t arg4){
    if(_driverJolt.objState < objStateOpened){ logError("objState(%d) < objStateOpened", _driverJolt.objState); return retFail; }
    int result = retOk;
    osalMutexLock(&_driverJolt.objMutex, -1);
    switch(sync){
        //
    }
syncExit:
    osalMutexUnlock(&_driverJolt.objMutex);
    return result;
}
#endif
