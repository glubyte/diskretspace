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

void mathLexicon(char* expression)
{
	token* tokens = NULL;
	uint32_t numTokens = 0;
	clock_t time;
	unsigned int i = 0, j = 0;

	time = clock();
	while (expression[i])
	{
		if (isgraph(expression[i]))
		{
			tokens = (token*)realloc(tokens, sizeof(token) * (numTokens + 1));

			if (isdigit(expression[i]))
			{
				// prepare for number of indefinite size
				tokens[numTokens].type = TOKEN_TYPE_OPERAND;
				tokens[numTokens].operandType = OPERAND_TYPE_INTEGER;
				tokens[numTokens].precedence = 1;
				tokens[numTokens].data = NULL;

				while (isdigit(expression[i]))
				{
					tokens[numTokens].data = (char*)realloc(tokens[numTokens].data, sizeof(char) * (j + 2));
					tokens[numTokens].data[j] = expression[i];
					i++;
					j++;
				}
				tokens[numTokens].data[j] = '\0';

				j = 0;
				numTokens++;
				continue;
			}
			if (isalpha(expression[i]))
			{
				// check for specific strings which can be operators and long variable names
				// variables with names longer than one character should be enclosed within parenthesis

				// for now, default to single variables
				tokens[numTokens].type = TOKEN_TYPE_OPERAND;
				tokens[numTokens].operandType = OPERAND_TYPE_VARIABLE;
				tokens[numTokens].precedence = 1;
				tokens[numTokens].data = (char*)malloc(sizeof(char) * 2);
				tokens[numTokens].data[0] = expression[i];
				tokens[numTokens].data[1] = '\0';

				i++;
				numTokens++;
				continue;
			}
			switch (expression[i])
			{
				case '=':
				{
					tokens[numTokens].type = TOKEN_TYPE_EQUALITY;
					tokens[numTokens].precedence = 0;
					tokens[numTokens].data = NULL;

					numTokens++;
					break;
				}
				case '(':
				{
					tokens[numTokens].type = TOKEN_TYPE_OPERAND;
					tokens[numTokens].operandType = OPERAND_TYPE_NEST;
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
					}
					tokens[numTokens].data = (char*)malloc(sizeof(char) * j);
					strncpy(tokens[numTokens].data, &expression[i - j + 1], sizeof(char) * (j - 1));
					tokens[numTokens].data[j] = '\0';

					j = 0;
					numTokens++;
					break;
				}
				case '+':
				{
					tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
					tokens[numTokens].operatorType = OPERATOR_TYPE_ADD;
					tokens[numTokens].precedence = 2;
					tokens[numTokens].data = NULL;

					numTokens++;
					break;
				}
				case '-':
				{
					tokens[numTokens].type = TOKEN_TYPE_UNARY_OPERATOR;
					tokens[numTokens].operatorType = OPERATOR_TYPE_MINUS;
					tokens[numTokens].precedence = 1;
					tokens[numTokens].data = NULL;

					numTokens++;
					break;
				}
				case '*':
				{
					tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
					tokens[numTokens].operatorType = OPERATOR_TYPE_MULTIPLY;
					tokens[numTokens].precedence = 3;
					tokens[numTokens].data = NULL;

					numTokens++;
					break;
				}
				case '/':
				{
					tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
					tokens[numTokens].operatorType = OPERATOR_TYPE_DIVIDE;
					tokens[numTokens].precedence = 3;
					tokens[numTokens].data = NULL;

					numTokens++;
					break;
				}
				case '^':
				{
					tokens[numTokens].type = TOKEN_TYPE_BINARY_OPERATOR;
					tokens[numTokens].operatorType = OPERATOR_TYPE_EXP;
					tokens[numTokens].precedence = 4;
					tokens[numTokens].data = NULL;

					numTokens++;
					break;
				}
				case '!':
				{
					tokens[numTokens].type = TOKEN_TYPE_UNARY_OPERATOR;
					tokens[numTokens].operatorType = OPERATOR_TYPE_FACTORIAL;
					tokens[numTokens].precedence = 1;
					tokens[numTokens].data = NULL;

					numTokens++;
					break;
				}
			}
		}
		i++;
	}
	time = clock() - time;
	printf("%i tokens resolved in %f seconds over %i iterations:\n", numTokens, (float)time / CLOCKS_PER_SEC, i - 1);
	
	free(expression);
	expression = NULL;

	for (i = 0; i < numTokens; i++)
	{
		if (tokens[i].data == NULL)
		{
			printf("Token of type %i.\n", tokens[i].type);
		}
		printf("%s\n", tokens[i].data);
	}

	printf("Freeing tokens...\n");
	for (i = 0; i < numTokens; i++)
	{
		freeToken(tokens[i]);
	}
	free(tokens);
	tokens = NULL;

	// return tokens;
}
node* genNode(token token)
{
	node* newNode = (node*)malloc(sizeof(node));
	// newNode.token = token;

	return newNode;
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
			/*
			case TOKEN_TYPE_NEST:
			{
				// insert this in same manner as operand
				genTree(mathLexicon(tokens[i].data));
			}
			*/
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

	return root;
}
void freeToken(token token)
{
	free(token.data);
	token.data = NULL;
}
void freeNode(node* node)
{
	freeToken(node->token);

	free(node);
}
void freeTree(node* root)
{

}