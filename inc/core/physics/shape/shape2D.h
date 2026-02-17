#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/physics/vector/vector2D.h"

typedef enum{
    shape2dTypeCircle = 0,
    shape2dTypeAABB,
} shape2dType;

typedef struct body2d body2d;

typedef struct shape2d {
    shape2dType type;
    struct body2d* body;
} shape2d;

shape2d* shape2dCreate(shape2dType, struct body2d*);
void shape2dDestroy(shape2d*);
