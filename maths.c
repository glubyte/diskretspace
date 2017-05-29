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

		if (isalnum(expression[i]))
		{
			// prepare for large numbers
			tokens[numTokens].type = TOKEN_TYPE_OPERAND;
			tokens[numTokens].precedence = 1;
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
		switch (expression[i])
		{
			case '+':
			case '-':
			{
				tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
				tokens[numTokens].precedence = 2;
				break;
			}
			case '*':
			case '/':
			{
				tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
				tokens[numTokens].precedence = 3;
				break;
			}
			case '^':
			{
				tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
				tokens[numTokens].precedence = 4;
				break;
			}
			case '(':
			case ')':
			{
				tokens[numTokens].type = TOKEN_TYPE_PARENTHESIS;
				tokens[numTokens].precedence = 5;
				break;
			}
		}
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
	node* t1 = NULL;
	node* t2 = NULL;
	unsigned int nodes = 0;
	unsigned int leaves = 0;
	unsigned char i = 0;
	time_t start;
	time_t finish;

	// pretty much a cellular automata
	time(&start);
	while (tokens[i].data)
	{

	}
	time(&finish);

	printf("Binary expression tree generated in %i seconds with %i leaves and %i nodes.\n", difftime(finish, start), leaves, nodes);
	free(tokens);
	tokens = NULL;
	// return tree;
}
node* genNode(token token)
{
	node* newNode = (node*)malloc(sizeof(node));
	strcpy(newNode->data, token.data);

	switch (token.type)
	{
		case TOKEN_TYPE_OPERAND:
		{
			newNode->type = NODE_TYPE_LEAF;
			newNode->left = NULL;
			newNode->right = NULL;
			break;
		}
		case TOKEN_TYPE_BINARY_OPERATOR:
		{
			newNode->type = NODE_TYPE_INTERNAL;
			break;
		}
		case TOKEN_TYPE_UNARY_OPERATOR:
		{
			newNode->type = NODE_TYPE_INTERNAL;
			break;
		}
	}

	return newNode;
}
void deleteNode(node* node)
{

}
