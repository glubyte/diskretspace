#pragma once

// standard
#include <stdio.h>
#include <stdint.h>
// local
#include "sdl.h"

// stack handles
typedef struct {
	uint32_t fps;
	uint32_t timer;
	uint32_t frames;
	uint32_t dt;
} fps_shit;

// prototypes
void initFPS(fps_shit* fpsshit);
void calculateFPS(fps_shit* fpsshit);