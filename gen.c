#include "gen.h"

void genSphere(int radius) {
	FILE* sphere = fopen("sphere", "w");
	vec3 v;
	int rs = radius * radius;

	for (v.i = -radius; v.i < radius; v.i++)
	{
		for (v.j = -radius; v.j < radius; v.j++)
		{
			for (v.k = -radius; v.k < radius; v.k++)
			{
				if (dot(v, v) < rs)
				{
					fprintf(sphere, "%i ", 1);
					continue;
				}
				fprintf(sphere, "%i ", 0);
			}
			fprintf(sphere, "\n");
		}
		fprintf(sphere, "\n");
	}
	fclose(sphere);
}