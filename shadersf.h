#ifndef SHADERSF_H
#define SHADERSF_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <string.h>

// Declaration for shaders functions utilities
char* readTextFile(char* aTextFile);
int setShader(char* shaderType, char* shaderFile);

#endif