#pragma once

// standard
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

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

// ai math
bool formatChecker(char* equation);
float solveFirstOrderEquation();