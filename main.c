// stop SDL from fucking with shit
#define SDL_MAIN_HANDLED

// standard
#include <stdio.h>
// local
#include "vksdl.h"
#include "physik.h"
#include "shaderpipes.h"
#include "r3.h"

int main()
{
	// initialize all header structs
	vksdl_shit vksdlshit; // vulkan + sdl
	physik_shit physik; // physics
	r3_shit r3; // perspective and projection
	transform t;
	// fill structs
	initPhysik(&physik);
	initR3(&r3);
	t.proj[0] = r3.proj[0];
	t.proj[1] = r3.proj[1];
	t.proj[2] = r3.proj[2];
	t.proj[3] = r3.proj[3];
	t.model[0] = 0.0f;
	t.model[1] = 0.0f;
	t.model[2] = -5.0f;
	t.view[0] = 0.0f;
	t.view[1] = 0.0f;
	t.view[2] = 0.0f;
	printf("ar: %f, px: %f py: %f pz: %f\n", r3.aspectratio, t.proj[0], t.proj[1], t.proj[2]);

	if (!initVkSDL(&vksdlshit))
	{
		printf("fucking init CIA niggers\nPress ENTER to quit.\n");
		getchar();
	}
	
	if (!createPipeline(&vksdlshit, "shaders/vert.spv", "shaders/frag.spv"))
	{
		printf("fucking pipe niggers\n");
		getchar();
	}
	else if (!recordCommands(&vksdlshit))
	{
		printf("fucking recording CIA niggers\nPress ENTER to quit.\n");
		getchar();
	}
	
	// freetype

	// game loop
	// these variables below to go into gameloop header struct
	unsigned char flag = 1;
	uint32_t timer = 0;
	uint32_t dt;
	uint32_t frames = 0;
	updateUniformBuffer(&vksdlshit, &t);
	while (flag != 0)
	{
		// FPS handler; loop should be capped at (50 / 3) = 16.66666 ms per frame; 60 FPS
		dt = SDL_GetTicks() - timer;
		if (dt > 500)
		{
			printf("FPS: %i\n", 1000 * frames / dt);
			frames = 0;
			timer = SDL_GetTicks();
		}

		while (SDL_PollEvent(&vksdlshit.event) != 0)
		{
			if (vksdlshit.event.type == SDL_KEYDOWN)
			{
				if (vksdlshit.event.key.keysym.sym == SDLK_ESCAPE)
				{
					// quit game
					flag = 0;
					break;
				}
				if (vksdlshit.event.key.keysym.sym == SDLK_SPACE)
				{
					++t.model[1];
					updateUniformBuffer(&vksdlshit, &t);
				}
				if (vksdlshit.event.key.keysym.sym == SDLK_LSHIFT)
				{
					--t.model[1];
					updateUniformBuffer(&vksdlshit, &t);
				}
				if (vksdlshit.event.key.keysym.sym == SDLK_d)
				{
					++t.model[0];
					updateUniformBuffer(&vksdlshit, &t);
				}
				if (vksdlshit.event.key.keysym.sym == SDLK_a)
				{
					--t.model[0];
					updateUniformBuffer(&vksdlshit, &t);
				}
				if (vksdlshit.event.key.keysym.sym == SDLK_w)
				{
					++t.model[2];
					updateUniformBuffer(&vksdlshit, &t);
				}
				if (vksdlshit.event.key.keysym.sym == SDLK_s)
				{
					--t.model[2];
					updateUniformBuffer(&vksdlshit, &t);
				}

			}
			/* to be diagnosed
			if (vksdlshit.event.type == SDL_WINDOWEVENT)
				switch (vksdlshit.event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					changeWindowSize(&vksdlshit);
					break;
				}
			*/
		}

		drawFrame(&vksdlshit);
		
		++frames;
	}
	
	// clean up
	cleanVkSDL(&vksdlshit);
	getchar();
	// exit program
	return 0;
}