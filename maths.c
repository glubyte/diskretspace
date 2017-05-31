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

char* mathClean(char* expression)
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
token* mathLexicon(char* expression)
{
	token* tokens = NULL;
	uint32_t numTokens = 0;
	clock_t time;
	unsigned int i = 0, j = 0;

	time = clock();
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
			case '(':
			{
				tokens[numTokens].type = TOKEN_TYPE_NEST;
				tokens[numTokens].precedence = 1;
				i++;

				while (expression[i] != ')')
				{
					tokens[numTokens].data[j] = expression[i];
					j++;
					i++;
				}
				j = 0;
				break;
			}
			case '+':
			case '-':
			{
				if (!isalnum(expression[i - 1]))
				{
					tokens[numTokens].type = TOKEN_TYPE_UNARY_OPERATOR;
					tokens[numTokens].precedence = 6;
					tokens[numTokens].data[0] = expression[i];
					break;
				}
				tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
				tokens[numTokens].precedence = 2;
				tokens[numTokens].data[0] = expression[i];
				break;
			}
			case '*':
			case '/':
			{
				tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
				tokens[numTokens].precedence = 3;
				tokens[numTokens].data[0] = expression[i];
				break;
			}
			case '^':
			{
				tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
				tokens[numTokens].precedence = 4;
				tokens[numTokens].data[0] = expression[i];
				break;
			}
			case '=':
			{
				tokens[numTokens].type = TOKEN_TYPE_EQUALITY;
				tokens[numTokens].precedence = 5;
				tokens[numTokens].data[0] = expression[i];
				break;
			}
			case '!':
			{
				tokens[numTokens].type = TOKEN_TYPE_UNARY_OPERATOR;
				tokens[numTokens].precedence = 6;
				tokens[numTokens].data[0] = expression[i];
				break;
			}
		}
		i++;
		numTokens++;
	}
	time = clock() - time;
	printf("%i tokens resolved in %f seconds.\n", numTokens, (float)time / CLOCKS_PER_SEC);
	
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
	node* root = NULL;
	node* nodeBuffer = NULL;
	unsigned int nodes = 0, leaves = 0;
	unsigned char i = 0;
	clock_t time;

	// pretty much a cellular automata
	time = clock();
	while (tokens[i].data)
	{
		switch (tokens[i].type)
		{
			case TOKEN_TYPE_OPERAND:
			{
				nodeBuffer = genNode(tokens[i]);
				nodeBuffer->type = NODE_TYPE_LEAF;
				nodeBuffer->left = NULL;
				nodeBuffer->right = NULL;
				break;
			}
			case TOKEN_TYPE_BINARY_OPERATOR:
			{

			}
		}
	}
	time = clock() - time;

	printf("Binary expression tree generated in %f seconds with %i leaves and %i nodes.\n", (float)time / CLOCKS_PER_SEC, leaves, nodes);
	free(tokens);
	tokens = NULL;
	return root;
}
node* genNode(token token)
{
	node* newNode = (node*)malloc(sizeof(node));
	newNode->token = token;

	return newNode;
}
void deleteNode(node* node)
{

}
