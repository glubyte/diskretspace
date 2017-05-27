// stop SDL from fucking with shit
#define SDL_MAIN_HANDLED

// standard
#include <stdio.h>
// local
#include "vk.h"
#include "sdl.h"
#include "planets.h"
#include "gen.h"

int main(void)
{
	// stacking
	unsigned char flag = 1;
	uint32_t timer = 0;
	uint32_t dt;
	uint32_t frames = 0;
	sdl_shit sdlshit;
	vk_shit vkshit;
	// init
	printf("Initializing...\n");
	if (initSDL(&sdlshit)) 
	{
		printf("SDL failed to initialize.\n");
		printf("Press ENTER to quit.\n");
		getchar();
		return 0;
	}
	printf("Initialization successful.\n");

	// main loop
	while(flag)
	{
		// FPS handler
		dt = SDL_GetTicks() - timer;
		if (dt > 1000)
		{
			printf("FPS: %i\n", 1000 * frames / dt);
			frames = 0;
			timer = SDL_GetTicks();
		}
		// event handler (put in own function)
		while (SDL_PollEvent(&sdlshit.event))
		{
			switch (sdlshit.event.type)
			{
			case SDL_WINDOWEVENT:
				switch (sdlshit.event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					resizeWindow(&sdlshit);
					printf("%i %i\n", sdlshit.width, sdlshit.height);
					break;
				case SDL_WINDOWEVENT_CLOSE:
					flag = 0;
					break;
				case SDL_WINDOWEVENT_MAXIMIZED:
					SDL_SetWindowFullscreen(sdlshit.window, SDL_WINDOW_FULLSCREEN);
					break;
				}
				break;
			case SDL_KEYUP:
				switch (sdlshit.event.key.keysym.sym)
				{
				case SDLK_c:
				{
					planet planet;
					printf("Enter planet name:\n");
					scanf("%s", planet.name);
					createPlanet(planet);
					break;
				}
				}
				break;
			}
		}
		// more shit
		// SDL_Delay(16);
		++frames;
	}

	// clean up
	// cleanVk(&vkshit);
	cleanSDL(&sdlshit);

	printf("Press ENTER to quit.\n");
	getchar();
	
	return 0;
}