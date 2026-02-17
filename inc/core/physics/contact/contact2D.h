#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/physics/vector/vector2D.h"

typedef struct body2d body2d;

typedef struct{
    body2d* bodyA;
    body2d* bodyB;
    vector2d normal; // 충돌 법선 (bodyA -> bodyB 방향)
    float penetration; // 침투 깊이
} contact2d;

contact2d* contact2dCreate(body2d*, body2d*);
void contact2dDestroy(contact2d*);
