#pragma once

// standard
#include <stdio.h>

// stack handles
typedef struct {
	struct {
		float G; // gravitational constant
		double c; // speed of light
	} universalConstants;
} physik_shit;

// prototype handles
void initPhysik(physik_shit* physik);
void fillUniversalConstants(physik_shit* physik);