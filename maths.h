#pragma once

// standard
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

// stack handles
typedef enum {
	TOKEN_TYPE_OPERAND,
	TOKEN_TYPE_BINARY_OPERATOR,
	TOKEN_TYPE_UNARY_OPERATOR,
	TOKEN_TYPE_EQUALITY
} tokenType;

typedef enum {
	OPERATOR_TYPE_ADD,
	OPERATOR_TYPE_MINUS,
	OPERATOR_TYPE_MULTIPLY,
	OPERATOR_TYPE_DIVIDE,
	OPERATOR_TYPE_EXP,
	OPERATOR_TYPE_SQRT,
	OPERATOR_TYPE_SIN,
	OPERATOR_TYPE_COS,
	OPERATOR_TYPE_TAN,
	OPERATOR_TYPE_DOT,
	OPERATOR_TYPE_CROSS,
	OPERATOR_TYPE_INTEGRAL,
	OPERATOR_TYPE_DERIVATIVE,
	OPERATOR_TYPE_FACTORIAL,
	OPERATOR_TYPE_SUM
} operatorType;

typedef enum {
	OPERAND_TYPE_INTEGER,
	OPERAND_TYPE_VARIABLE,
	OPERAND_TYPE_VECTOR,
	OPERAND_TYPE_MATRIX,
	OPERAND_TYPE_COMPLEX,
	OPERAND_TYPE_NEST
} operandType;

typedef enum {
	NODE_TYPE_LEAF,
	NODE_TYPE_INTERNAL,
	NODE_TYPE_ROOT
} nodeType;

typedef struct {
	tokenType type;
	operatorType operatorType;
	operandType operandType;
	unsigned char precedence;
	char* data;
} token;

typedef struct tree {
	nodeType type;
	token token;
	unsigned int branches;
	struct tree** branch;
} node;

typedef struct {
	float i, j, k;
} vec3;

// prototypes

// linear algebra
float dot(vec3 a, vec3 b);
vec3 cross(vec3 a, vec3 b);
float length(vec3 a);
void normalize(vec3* a);

// symbolic computation
void mathLexicon(char* expression); // will return the tokens, but currently debugging
node* genNode(token token);
node* genTree(token* tokens, unsigned int numTokens);
void freeToken(token token);
void freeNode(node* node);
void freeTree(node* root);