#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "shape2D.h"

typedef struct circle2d{
    shape2d base;
    float radius;
} circle2d;

circle2d* circle2dCreate(float, body2d*);
void circle2dDestroy(circle2d*);
