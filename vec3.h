#pragma once
#include <limits>

struct vec3
{
    float x, y, z;
    static vec3 Subtract(vec3 src, vec3 dst);
    static float Magnitude(vec3 vec);
    static float Distance(vec3 src, vec3 dst);
    static vec3 CalcAngle(vec3 src, vec3 dst);
};
