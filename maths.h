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
	TOKEN_TYPE_OPERAND = 0,
	TOKEN_TYPE_BINARY_OPERATOR = 1,
	TOKEN_TYPE_UNARY_OPERATOR = 2,
	TOKEN_TYPE_EQUALITY = 3,
} tokenType;

typedef enum {
	OPERATOR_TYPE_ADD = 0,
	OPERATOR_TYPE_MINUS = 1,
	OPERATOR_TYPE_MULTIPLY = 2,
	OPERATOR_TYPE_DIVIDE = 3,
	OPERATOR_TYPE_EXP = 4,
	OPERATOR_TYPE_SQRT = 5,
	OPERATOR_TYPE_SIN = 6,
	OPERATOR_TYPE_COS = 7,
	OPERATOR_TYPE_TAN = 8,
	OPERATOR_TYPE_DOT = 9,
	OPERATOR_TYPE_CROSS = 10,
	OPERATOR_TYPE_INTEGRAL = 11,
	OPERATOR_TYPE_DERIVATIVE = 12,
	OPERATOR_TYPE_FACTORIAL = 13,
	OPERATOR_TYPE_SUM = 14
} operatorType;

typedef enum {
	OPERAND_TYPE_INTEGER = 0,
	OPERAND_TYPE_VARIABLE = 1,
	OPERAND_TYPE_VECTOR = 2,
	OPERAND_TYPE_MATRIX = 3,
	OPERAND_TYPE_COMPLEX = 4,
	OPERAND_TYPE_NEST = 5
} operandType;

typedef enum {
	NODE_TYPE_LEAF = 0,
	NODE_TYPE_INTERNAL = 1,
	NODE_TYPE_ROOT = 2
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
	struct tree* left;
	struct tree* right;
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
node* genTree(token* tokens);
void freeToken(token token);
void freeNode(node* node);
void freeTree(node* root);