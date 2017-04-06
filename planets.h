#pragma once

// standard
#include <stdio.h>
// local

// stack handles
typedef struct {
	unsigned char* name;
	unsigned char* dateCreated;
	unsigned int mass;
	unsigned int volume;
	int surfaceTemperature;
	float surfaceGravity;
	int meanRadius;
	int polarRadius;
	int equatorialRadius;
	float surfacePressure;
} planet;

// prototypes