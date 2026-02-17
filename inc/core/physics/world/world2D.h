#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include <stddef.h>
#include "core/physics/body/body2D.h"
#include "core/physics/contact/contact2D.h"
#include "core/physics/collision/collision2D.h"

typedef struct world2d{
    body2d** bodies;
    size_t bodyCount, bodyCapacity;
    contact2d** contacts;
    size_t contactCount, contactCapacity;
} world2d;

world2d* world2dCreate(size_t, size_t);
void world2dDestroy(world2d*);
int world2dAddBody(world2d*, body2d*);
int world2dRemoveBody(world2d*, body2d*);
void world2dStep(world2d*, float);
