// stop SDL from fucking with shit
#define SDL_MAIN_HANDLED

// standard
#include <stdio.h>
// local
#include "vk.h"
#include "sdl.h"
#include "planets.h"
#include "gen.h"
#include "fps.h"

int main(void)
{
	// stacking
	unsigned char flag = 1;
	char* expression = NULL;
	fps_shit fpsshit;
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
	printf("SDL successfully initialized.\n");
	initFPS(&fpsshit);
	printf("FPS module successfully initialized.\n");
	printf("Initialization complete.\n");

	// main loop
	while(flag)
	{
		// FPS handler
		// calculateFPS(&fpsshit);

		expression = (char*)malloc(sizeof(char) * 100);
		printf("Enter an expression, organism:\n");
		fgets(expression, 100, stdin);
		fseek(stdin, 0, SEEK_END);
		mathLexicon(expression);

		// event handler (put in own function)
		while (SDL_PollEvent(&sdlshit.event))
		{
			switch (sdlshit.event.type)
			{
			case SDL_WINDOWEVENT:
				switch (sdlshit.event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					resizeWindow(&vkshit, &sdlshit);
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
			}
		}
		// more shit
		// SDL_Delay(16);
		// ++fpsshit.frames;
	}

	// clean up
	// cleanVk(&vkshit);
	cleanSDL(&sdlshit);

	printf("Press ENTER to quit.\n");
	getchar();
	
	return 0;
}