#pragma once
/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/physics/body/body2D.h"
#include "core/physics/contact/contact2D.h"
#include "core/physics/shape/circle2D.h"
#include "core/physics/shape/aabb2D.h"

int collision2dCheck(body2d*, body2d*, contact2d**);
int collision2dCheckCricleToCircle(circle2d*, circle2d*, contact2d**);
int collision2dCheckCircleToAabb(circle2d*, aabb2d*, contact2d**);
int collision2dCheckAabbToAabb(aabb2d*, aabb2d*, contact2d**);
