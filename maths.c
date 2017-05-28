#include "maths.h"

float dot(vec3 a, vec3 b) 
{
	return a.i * b.i + a.j * b.j + a.k * b.k;
}
vec3 cross(vec3 a, vec3 b)
{
	vec3 result;

	result.i = a.j * b.k - a.k * b.j;
	result.j = a.k * b.i - a.i * b.k;
	result.k = a.i * b.j - a.j * b.i;

	return result;
}
float length(vec3 a) 
{
	return sqrt(dot(a, a));
}
void normalize(vec3* a) 
{
	float l = length(*a);
	
	a->i = a->i / l;
	a->j = a->j / l;
	a->k = a->k / l;
}
bool formatChecker(char* equation)
{
	// this function is utilized for human input only

	int i = 0;
	char variable;

	// check for equals sign
	if (strchr(equation, '=') != NULL)
	{
		// check for variables
		while (equation[i])
		{
			if (isalpha(equation[i]))
			{
				variable = equation[i];
				// ensure first order
				while (equation[i])
				{
					if (isalpha(equation[i]))
					{
						if (variable != equation[i])
						{
							printf("That is not a first order equation.\n");
							return 0;
						}
					}
					i++;
				}
				printf("Format verified.\n");
				return 1;
			}
			i++;
		}
		printf("That is not an equation.\n");
		return 0;
	}
	else
	{
		printf("That is not an equation.\n");
		return 0;
	}
}
float solveFirstOrderEquation()
{
	uint32_t operations;
	time_t start;
	time_t finish;
	char equation[256];

	printf("Enter a first order equation: \n");
	// acquire equation
	fgets(equation, sizeof(equation), stdin);
	// check format
	if (!formatChecker(equation))
	{
		return 0;
	}
	// begin solution
	time(&start);

	time(&finish);
	printf("Solved in %i seconds over %i operations.\n", difftime(finish, start), operations);
	return 1;
}
