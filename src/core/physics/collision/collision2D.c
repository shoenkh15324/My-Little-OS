/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/physics/collision/collision2D.h"
#include "core/physics/vector/vector2D.h"
#include <math.h>

int collision2dCheckCricleToCircle(circle2d* circleA, circle2d* circleB, contact2d** outContact){
    vector2d diff = vector2dSub(circleB->base.body->position, circleA->base.body->position);
    float dist = vector2dLength(diff);
    float radiusSum = circleA->radius + circleB->radius;
    if(dist >= radiusSum) return 0; // 충돌 없음
    if(outContact){
        *outContact = contact2dCreate(circleA->base.body, circleB->base.body);
        if(*outContact){
            (*outContact)->penetration = radiusSum - dist;
            (*outContact)->normal = (dist == 0.0f) ? (vector2d){1.0f,0.0f} : vector2dScale(diff, 1.0f/dist);
        }
    }
    return 1;
}
int collision2dCheckCircleToAabb(circle2d* circle, aabb2d* aabb, contact2d** outContact){
    vector2d circlePos = circle->base.body->position;
    vector2d boxPos = aabb->base.body->position;
    float halfWidth = aabb->width * 0.5f;
    float halfHeight = aabb->height * 0.5f;
    // AABB에서 circle 중심과 가장 가까운 점 계산
    float closestX = fmaxf(boxPos.x - halfWidth, fminf(circlePos.x, boxPos.x + halfWidth));
    float closestY = fmaxf(boxPos.y - halfHeight, fminf(circlePos.y, boxPos.y + halfHeight));
    vector2d closestPoint = {closestX, closestY};
    vector2d diff = vector2dSub(circlePos, closestPoint);
    float dist = vector2dLength(diff);
    if(dist >= circle->radius) return 0; // 충돌 없음
    if(outContact){
        *outContact = contact2dCreate(circle->base.body, aabb->base.body);
        if(*outContact){
            (*outContact)->penetration = circle->radius - dist;
            (*outContact)->normal = (dist == 0.0f) ? (vector2d){1.0f,0.0f} : vector2dScale(diff, 1.0f/dist);
        }
    }
    return 1;
}
int collision2dCheckAabbToAabb(aabb2d* aabbA, aabb2d* aabbB, contact2d** outContact){
    float aLeft = aabbA->base.body->position.x - aabbA->width * 0.5f;
    float aRight = aabbA->base.body->position.x + aabbA->width * 0.5f;
    float aBottom = aabbA->base.body->position.y - aabbA->height * 0.5f;
    float aTop = aabbA->base.body->position.y + aabbA->height * 0.5f;
    float bLeft = aabbB->base.body->position.x - aabbB->width * 0.5f;
    float bRight = aabbB->base.body->position.x + aabbB->width * 0.5f;
    float bBottom = aabbB->base.body->position.y - aabbB->height * 0.5f;
    float bTop = aabbB->base.body->position.y + aabbB->height * 0.5f;
    if(aRight < bLeft || aLeft > bRight || aTop < bBottom || aBottom > bTop) return 0; // 충돌 없음
    if(outContact){
        *outContact = contact2dCreate(aabbA->base.body, aabbB->base.body);
        if(*outContact){
            // penetration 계산
            float penX = fminf(aRight - bLeft, bRight - aLeft);
            float penY = fminf(aTop - bBottom, bTop - aBottom);
            (*outContact)->penetration = fminf(penX, penY);
            // normal 결정
            if(penX < penY){
                (*outContact)->normal = (aabbA->base.body->position.x < aabbB->base.body->position.x) ? (vector2d){-1,0} : (vector2d){1,0};
            } else {
                (*outContact)->normal = (aabbA->base.body->position.y < aabbB->base.body->position.y) ? (vector2d){0,-1} : (vector2d){0,1};
            }
        }
    }
    return 1;
}
int collision2dCheck(body2d* bodyA, body2d* bodyB, contact2d** outContact){
    if(!bodyA || !bodyB) return 0;
    if(bodyA->shape->type == shape2dTypeCircle && bodyB->shape->type == shape2dTypeCircle){
        return collision2dCheckCricleToCircle((circle2d*)bodyA->shape, (circle2d*)bodyB->shape, outContact);
    }else if(bodyA->shape->type == shape2dTypeAABB && bodyB->shape->type == shape2dTypeAABB){
        return collision2dCheckAabbToAabb((aabb2d*)bodyA->shape, (aabb2d*)bodyB->shape, outContact);
    }else if(bodyA->shape->type == shape2dTypeCircle && bodyB->shape->type == shape2dTypeAABB){
        return collision2dCheckCircleToAabb((circle2d*)bodyA->shape, (aabb2d*)bodyB->shape, outContact);
    }else if(bodyA->shape->type == shape2dTypeAABB && bodyB->shape->type == shape2dTypeCircle){
        return collision2dCheckCircleToAabb((circle2d*)bodyB->shape, (aabb2d*)bodyA->shape, outContact);
    }
    return 0;
}
