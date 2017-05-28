#include "fps.h"

void initFPS(fps_shit* fpsshit)
{
	fpsshit->frames = 0;
	fpsshit->timer = 0;
}
void calculateFPS(fps_shit* fpsshit)
{
	fpsshit->dt = SDL_GetTicks() - fpsshit->timer;

	if (fpsshit->dt > 1000)
	{
		fpsshit->fps = 1000 * fpsshit->frames / fpsshit->dt;
		fpsshit->frames = 0;
		fpsshit->timer = SDL_GetTicks();

		printf("FPS: %i\n", fpsshit->fps);
	}
}