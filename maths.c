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
			tokens[numTokens].data[j] = '\0';
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
				int p = 1;

				// deal with nested parenthesis
				while (p)
				{
					i++;
					j++;
					switch (expression[i])
					{
						case '(':
						{
							p++;
							break;
						}
						case ')':
						{
							p--;
							break;
						}
					}
					tokens[numTokens].data[j] = expression[i];
				}
				tokens[numTokens].data[j] = '\0';
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
			default:
			{
				i++;
				continue;
			}
		}
		i++;
		numTokens++;
	}
	time = clock() - time;
	printf("%i tokens resolved in %f seconds over %i iterations.\n", numTokens, (float)time / CLOCKS_PER_SEC, i - 1);
	
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
	node* buffer = NULL;
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
				node* buffer = genNode(tokens[i]);
				buffer->type = NODE_TYPE_LEAF;
				buffer->left = NULL;
				buffer->right = NULL;

				if (root == NULL)
				{
					root = buffer;
					break;
				}

			}
			case TOKEN_TYPE_NEST:
			{
				// insert this in same manner as operand
				genTree(mathLexicon(tokens[i].data));
			}
			case TOKEN_TYPE_BINARY_OPERATOR:
			{

			}
			case TOKEN_TYPE_UNARY_OPERATOR:
			{

			}
			case TOKEN_TYPE_EQUALITY:
			{
				// assign as root and build the right side of the tree in the same way. attach to the right of equality when finished
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
