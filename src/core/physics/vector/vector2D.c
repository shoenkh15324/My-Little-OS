/******************************************************************************
 *  Author : Minkyu Kim
 *  Created: 2026-02-17
 ******************************************************************************/
#include "core/physics/vector/vector2D.h"
#include "math.h"

vector2d vector2dCreate(float x, float y){
    vector2d v = {x, y};
    return v;
}
vector2d vector2dAdd(vector2d a, vector2d b){
    return (vector2d){a.x + b.x, a.y + b.y};
}
vector2d vector2dSub(vector2d a, vector2d b){
    return (vector2d){a.x - b.x, a.y - b.y};
}
vector2d vector2dScale(vector2d v, float s){
    return (vector2d){v.x * s, v.y * s};
}
float vector2dLength(vector2d v){
    return sqrtf(v.x * v.x + v.y * v.y);
}
float vector2dLengthSq(vector2d v){
    return v.x * v.x + v.y * v.y;
}
vector2d vector2dNormalize(vector2d v){
    float len = vector2dLength(v);
    if(len > 0.0f){
        return vector2dScale(v, 1.0f / len);
    }
    return vector2dZero();
}
float vector2dDot(vector2d a, vector2d b){
    return a.x * b.x + a.y * b.y;
}
float vector2dCross(vector2d a, vector2d b){
    return a.x * b.y - a.y * b.x;
}
vector2d vector2dZero(void){
    return (vector2d){0.0f, 0.0f};
}
