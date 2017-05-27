#pragma once

// standard
#include <math.h>

// stack handles
typedef struct {
	float i, j, k;
} vec3;

// prototypes
// linear algebra
float dot(vec3 a, vec3 b);
vec3 cross(vec3 a, vec3 b);
float length(vec3 a);
void normalize(vec3* a);