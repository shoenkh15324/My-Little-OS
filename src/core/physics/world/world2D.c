/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/physics/world/world2D.h"
#include "core/physics/collision/collision2D.h"
#include "core/physics/solver/solver2D.h"
#include "core/feature/osal.h"
#include "core/feature/log.h"

static void _world2dClearContacts(world2d* world){
    for(size_t i = 0; i < world->contactCount; i++){
        if(osalFree(world->contacts[i])){ logError("osalFree fail"); return; }
    }
    world->contactCount = 0;
}
world2d* world2dCreate(size_t maxBodies, size_t maxContacts){
    world2d* world = NULL;
    if(osalMalloc((void**)&world, sizeof(world2d))){ logError("osalMalloc fail");
        return NULL;
    }
    if(osalMalloc((void**)&world->bodies, (sizeof(body2d*) * maxBodies))){logError("osalMalloc fail");
        osalFree(world);
        return NULL;
    }
    world->bodyCount = 0;
    world->bodyCapacity = maxBodies;
    if(osalMalloc((void**)&world->contacts, (sizeof(contact2d*) * maxContacts))){logError("osalMalloc fail");
        osalFree(world->bodies);
        osalFree(world);
        return NULL;
    }
    world->contactCount = 0;
    world->contactCapacity = maxContacts;
    return world;
}
void world2dDestroy(world2d* world){
    if(!world) return;
    if(world->contacts){
        _world2dClearContacts(world);
        if(osalFree(world->contacts)){ logError("osalFree fail"); }
    }
    if(world->bodies){
        if(osalFree(world->bodies)){ logError("osalFree fail"); }
    }
    if(osalFree(world)){ logError("osalFree fail"); }
}
int world2dAddBody(world2d* world, body2d* body){
    if(!world || !body){ logError("Invalid Params"); return retFail; }
    if(world->bodyCount >= world->bodyCapacity){ logError("world->bodyCount >= world->bodyCapacity"); return retFail; }
    world->bodies[world->bodyCount++] = body;
    return retOk;
}
int world2dRemoveBody(world2d* world, body2d* body){
    if(!world || !body){ logError("Invalid Params"); return retFail; }
    size_t i;
    for(i = 0; i < world->bodyCount; i++){
        if(world->bodies[i] == body) break;
    }
    if(i == world->bodyCount) return retFail;
    memmove(&world->bodies[i], &world->bodies[i+1], (world->bodyCount - i - 1) * sizeof(body2d*));
    world->bodyCount--;
    return 0;
}
void world2dStep(world2d* world, float dt){
    if(!world){ logError("Invalid Params"); return; }
    // 1. 각 body 위치 업데이트 (단순 위치 = velocity * dt)
    for(size_t i = 0; i < world->bodyCount; i++){
        body2d* body = world->bodies[i];
        body->position.x += body->velocity.x * dt;
        body->position.y += body->velocity.y * dt;
    }
    // 2. 충돌 검사: contact 생성
    _world2dClearContacts(world);
    for(size_t i = 0; i < world->bodyCount; i++){
        for(size_t j = i + 1; j < world->bodyCount; j++){
            body2d* a = world->bodies[i];
            body2d* b = world->bodies[j];
            contact2d* contact = NULL;
            if(collision2dCheck(a, b, &contact)){
                if(world->contactCount < world->contactCapacity){
                    world->contacts[world->contactCount++] = contact;
                } else {
                    if(contact) osalFree(contact);
                }
            }
        }
    }
    // 3. 충돌 해결 (solver 호출)
    if(world->contactCount > 0){
        solver2dCollisionSolve(world->contacts, (int)world->contactCount);
    }
}
