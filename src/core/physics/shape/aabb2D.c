/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/physics/shape/aabb2D.h"
#include "core/feature/osal.h"
#include "core/feature/log.h"

aabb2d* aabb2dCreate(float width, float height, body2d* body){
    aabb2d* aabb = NULL;
    if(osalMalloc((void**)&aabb, sizeof(aabb2d))){ logError("osalMalloc fail");
        return NULL;
    }
    aabb->base.type = shape2dTypeAABB;
    aabb->base.body = body;
    aabb->width = width;
    aabb->height = height;
    return aabb;
}
void aabb2dDestroy(aabb2d* aabb){
    if(aabb){
        if(osalFree(aabb)){ logError("osalFree fail"); return; }
    }
}
