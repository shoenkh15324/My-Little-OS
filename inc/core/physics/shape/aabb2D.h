#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "shape2D.h"

typedef struct aabb2d{
    shape2d base;
    float width, height;
} aabb2d;

aabb2d* aabb2dCreate(float, float, body2d*);
void aabb2dDestroy(aabb2d*);
