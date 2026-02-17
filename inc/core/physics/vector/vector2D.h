#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/systemDefs.h"

typedef struct vector2d{
    float x, y;
} vector2d;

vector2d vector2dCreate(float, float);
vector2d vector2dAdd(vector2d, vector2d);
vector2d vector2dSub(vector2d, vector2d);
vector2d vector2dScale(vector2d, float);
float vector2dLength(vector2d);
float vector2dLengthSq(vector2d);
vector2d vector2dNormalize(vector2d);
float vector2dDot(vector2d, vector2d);
float vector2dCross(vector2d, vector2d);
vector2d vector2dZero(void);
