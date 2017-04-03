#pragma once

// standard
#include <stdio.h>
// local
#include "vksdl.h"

// global allocation
typedef struct {
	unsigned char* code;
	size_t size;
} shaderHandle;

// prototype allocation
unsigned char createPipeline(vksdl_shit* vksdlshit, char* vertShader, char* fragShader);
shaderHandle loadShader(char* shaderLoc);