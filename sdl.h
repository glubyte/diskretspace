#pragma once

// standard
#include <stdio.h>
#include <stdbool.h>
// external
#include <SDL.h>
#include <SDL_syswm.h>

// stack handles
typedef struct {
	SDL_Window* window;
	SDL_SysWMinfo windowInfo;
	SDL_Event event;
	int width;
	int height;
} sdl_shit;

// prototypes
bool initSDL(sdl_shit* sdlshit);
bool createWindow(sdl_shit* sdlshit);
void cleanSDL(sdl_shit* sdlshit);