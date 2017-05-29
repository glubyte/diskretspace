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

char* mathLex(char* expression)
{
	unsigned int i = 0, j = 0;
	char* buffer = NULL;

	// delete white-spaces
	while (expression[i])
	{
		buffer = (char*)realloc(buffer, sizeof(char)*(j + 1));

		if (isspace(expression[i]))
		{
			i++;
			continue;
		}
		buffer[j] = expression[i];
		i++;
		j++;
	}
	buffer[j] = '\0';

	printf("Interpreted as: %s\n", buffer);
	return buffer;
}
token* genTokens(char* expression)
{
	token* tokens = NULL;
	uint32_t numTokens = 0;
	time_t start;
	time_t finish;
	unsigned int i = 0, j = 0;

	time(&start);
	while (expression[i])
	{
		tokens = (token*)realloc(tokens, sizeof(token)*(numTokens + 1));

		if (isdigit(expression[i]))
		{
			// prepare for large numbers
			tokens[numTokens].type = TOKEN_TYPE_OPERAND;
			tokens[numTokens].data[j] = expression[i];
			j++;
			i++;
			while (isdigit(expression[i]))
			{
				tokens[numTokens].data[j] = expression[i];
				j++;
				i++;
			}
			numTokens++;
			j = 0;
			continue;
		}
		tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
		tokens[numTokens].data[0] = expression[i];
		i++;
		numTokens++;
	}
	time(&finish);
	printf("Tokens resolved: %i\nDuration: %i s\n", numTokens, difftime(finish, start));
	for (i = 0; i < numTokens; i++)
	{
		printf("%s\n", tokens[i].data);
	}
	free(expression);
	expression = NULL;
	return tokens;
}
node* genTree(token* tokens)
{
	node* tree = NULL;
	unsigned int nodes = 0;
	unsigned int leaves = 0;
	unsigned char i;
	time_t start;
	time_t finish;

	// the following is more cellular automata-esque than what i had imagined
	time(&start);
	while (tokens[i].data)
	{
		node* newNode = (node*)malloc(sizeof(node));
		strcpy(newNode->data, tokens[i].data);

		switch (tokens[i].type)
		{
			case TOKEN_TYPE_OPERAND:
			{
				newNode->type = NODE_TYPE_LEAF;
				newNode->left = NULL;
				newNode->right = NULL;
				leaves++;
				break;
			}
			case TOKEN_TYPE_BINARY_OPERATOR:
			{
				newNode->type = NODE_TYPE_INTERNAL;
				nodes++;
				break;
			}
			case TOKEN_TYPE_PARENTHESIS:
			{
				break;
			}
		}
	}
	time(&finish);

	printf("Binary expression tree generated in %i seconds with %i leaves and %i nodes.\n", difftime(finish, start), leaves, nodes);
	free(tokens);
	tokens = NULL;
	return tree;
}
void insertNode(node* tree)
{

}
