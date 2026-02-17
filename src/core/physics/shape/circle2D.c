/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/physics/shape/circle2D.h"
#include "core/feature/osal.h"
#include "core/feature/log.h"

circle2d* circle2dCreate(float radius, body2d* body){
    circle2d* circle = NULL;
    if(osalMalloc((void**)&circle, sizeof(circle2d))){ logError("osalMalloc fail");
        return NULL;
    }
    circle->base.type = shape2dTypeCircle;
    circle->base.body = body;
    circle->radius = radius;
    return circle;
}
void circle2dDestroy(circle2d* circle){
    if(circle){
        if(osalFree(circle)){ logError("osalFree fail"); return; }
    }
}
