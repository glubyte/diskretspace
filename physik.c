#include "physik.h"

// prototype definitions
void initPhysik(physik_shit* physik)
{
	fillUniversalConstants(physik);
}
void fillUniversalConstants(physik_shit* physik)
{
	physik->universalConstants.G = 6.67408E-11;
	physik->universalConstants.c = 3E+8;
}