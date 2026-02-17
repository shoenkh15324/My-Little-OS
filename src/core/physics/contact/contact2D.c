/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/physics/contact/contact2D.h"
#include "core/feature/osal.h"
#include "core/feature/log.h"

contact2d* contact2dCreate(body2d* a, body2d* b){
    if(!a || !b){ logError("Invalid Params"); return NULL; }
    contact2d* contact = NULL;
    if(osalMalloc((void**)&contact, sizeof(contact2d))){ logError("osalMalloc fail");
        return NULL;
    }
    contact->bodyA = a;
    contact->bodyB = b;
    contact->normal = (vector2d){0.0f, 0.0f};
    contact->penetration = 0.0f;
    return contact;
}
void contact2dDestroy(contact2d* contact){
    if(contact){
        if(osalFree(contact)){ logError("osalFree fail"); return; }
    }
}
