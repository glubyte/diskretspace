#include "r3.h"

// prototype definitions
void initR3(r3_shit *r3)
{
	r3->nearplane = 0.1f; 
	r3->farplane = 500.0f; // ultimately from config file
	r3->fov = 45.0f; // ultimately from config file

	genProj(r3);
}
void genProj(r3_shit *r3)
{
	// get new aspect ratio (since this function is called upon window resize and view distance change)
	r3->aspectratio = (float)WIDTH / (float)HEIGHT;

	// here be magic
	float c0 = tan(r3->fov * (M_PI / 180) / 2);
	r3->proj[0] = 1.0f / c0;
	r3->proj[1] = r3->aspectratio / c0;
	r3->proj[2] = -r3->farplane / (r3->farplane - r3->nearplane);
	r3->proj[3] = -r3->farplane * r3->nearplane / (r3->farplane - r3->nearplane);
} 