#ifndef MODELVIEWSTACK_H
#define MODELVIEWSTACK_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <cglm/cglm.h>
#include <math.h>

#define MODELVIEW_STACK_SIZE 32

typedef struct modelViewStack {
    mat4 stack[MODELVIEW_STACK_SIZE];
    int tail;
} modelViewStack_t;

// MODELVIEW MATRIX STACK FUNCS
void initStack(modelViewStack_t* stack);
void popMatrix(modelViewStack_t *stack, mat4 modelViewMatrix);
void pushMatrix(modelViewStack_t *stack, mat4 modelViewMatrix);
void printMatrix(mat4 matrix);

#endif