#pragma once

// standard
#include <stdio.h>
#include <string.h>

// stack handles
typedef struct {
	unsigned char name[32];
	unsigned int age;
} star;

// prototypes
void createStar(star star);