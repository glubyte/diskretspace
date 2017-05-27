#include "sdl.h"

bool initSDL(sdl_shit* sdlshit)
{
	sdlshit->width = 800; // ultimately from ini file
	sdlshit->height = 600; // ultimately from ini file
	SDL_VERSION(&sdlshit->windowInfo.version);

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		printf("SDL video module failed to initialize: %s\n", SDL_GetError());
		return 1;
	}

	printf("Creating window...\n");
	if (createWindow(sdlshit))
		return 1;
	printf("Window created.\n");

	return 0;
}
bool createWindow(sdl_shit* sdlshit)
{
	SDL_DisplayMode mode;

	// create window
	sdlshit->window = SDL_CreateWindow("Diskretspace", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sdlshit->width, sdlshit->height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (sdlshit->window == NULL)
	{
		printf("Window could not be created: %s\n", SDL_GetError());
		return 1;
	}

	// acquire desktop display information and apply for fullscreen
	SDL_GetDesktopDisplayMode(0, &mode);
	SDL_SetWindowDisplayMode(sdlshit->window, &mode);

	// acquire window manager information
	if (SDL_GetWindowWMInfo(sdlshit->window, &sdlshit->windowInfo))
	{
		switch (sdlshit->windowInfo.subsystem)
		{
		case SDL_SYSWM_WINDOWS:
			printf("Win32 window manager detected...\n");
			return 0;
		case SDL_SYSWM_X11:
			printf("X11 window manager detected...\n");
			return 0;
		case SDL_SYSWM_WAYLAND:
			printf("Wayland window manager detected...\n");
			return 0;
		case SDL_SYSWM_MIR:
			printf("Mir window manager detected...\n");
			return 0;
		default:
			printf("Unsupported window manager.\n");
			return 1;
		}
	}
	printf("Could not retrieve window manager information: %s", SDL_GetError());
	return 1;
}
void resizeWindow(sdl_shit* sdlshit)
{
	SDL_GetWindowSize(sdlshit->window, &sdlshit->width, &sdlshit->height);
}
void cleanSDL(sdl_shit* sdlshit)
{
	SDL_DestroyWindow(sdlshit->window);
	SDL_Quit();

	printf("SDL clean up successful.\n");
}