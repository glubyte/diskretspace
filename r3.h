#pragma once

// standard
#include <stdio.h>
#include <math.h>
// local
#include "vksdl.h"

// stack handles
typedef struct {
	float farplane;
	float nearplane;
	float fov;
	float aspectratio;
	float proj[4];
} r3_shit;

// prototypes
void initR3(r3_shit *r3);
void genProj(r3_shit *r3);