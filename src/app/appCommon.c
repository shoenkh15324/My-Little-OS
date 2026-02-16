/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-10
 ******************************************************************************/
#include "service/serviceCommon.h"

int appCommonClose(void){
    int result = retOk;
    if(appClose()){ logError("appClose fail"); result = retFail; }
    return result;
}
int appCommonOpen(void){
    int result = retOk;
    if(appOpen()){ logError("appOpen fail"); result = retFail; }
    return result;
}
