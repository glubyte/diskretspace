#include "planets.h"

void createPlanet(planet planet)
{
	char loc[50] = "planets/";
	strcat(loc, planet.name);

	FILE* file = fopen(loc, "w");

	fclose(file);
}