#include "stars.h"

void createStar(star star)
{
	char loc[50] = "stars/";
	strcat(loc, star.name);

	FILE* file = fopen(loc, "w");

	fclose(file);
}