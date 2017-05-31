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
	TOKEN_TYPE_NEST = 1,
	TOKEN_TYPE_BINARY_OPERATOR = 2,
	TOKEN_TYPE_UNARY_OPERATOR = 3,
	TOKEN_TYPE_EQUALITY = 4,
} tokenType;

typedef enum {
	NODE_TYPE_LEAF = 0,
	NODE_TYPE_INTERNAL = 1,
	NODE_TYPE_ROOT = 2
} nodeType;

typedef struct {
	tokenType type;
	char data[32]; // ideally void* to handle all data types
	unsigned char precedence;
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
token* mathLexicon(char* expression);
node* genTree(token* tokens);
node* genNode(token token);
void deleteNode(node* node);