/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/physics/shape/shape2D.h"
#include "core/feature/osal.h"
#include "core/feature/log.h"

shape2d* shape2dCreate(shape2dType type, body2d* body){
    shape2d* shape = NULL;
    if(osalMalloc((void**)&shape, sizeof(shape2d))){ logError("osalMalloc fail");
        return NULL;
    }
    shape->type = type;
    shape->body = body;
    return shape;
}
void shape2dDestroy(shape2d* shape){
    if(shape){
        if(osalFree(shape)){ logError("osalFree fail"); return; }
    }
}
