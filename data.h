#ifndef DATA_H
#define DATA_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <GL/freeglut.h>

#include "structures.h"

/**
 * Maximum number of rotations
*/
#define MAX_ROT 200

/**
 * 4 vertices per face
*/
#define N_VERTICES 4

/**
 * Number of faces to draw
*/
#define N_FACES 6

/**
 * Number of cubes
*/
#define N_CUBES 27

/* 
 * Numero di cubi per faccia
 * Usato per automatizzare la costruzione del cubo
 */
const unsigned int CubesPerPlan = 9; 

/**
 * array indici
*/
GLuint cubeIndices[N_CUBES];
GLuint x_0[] = {0, 3, 6, 9, 12, 15, 18, 21, 24};
GLuint x_1[] = {1, 4, 7, 10, 13, 16, 19, 22, 25};
GLuint x_2[] = {2, 5, 8, 11, 14, 17, 20, 23, 26};
//--------------------------------------------------
GLuint y_0[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
GLuint y_1[] = {9, 10, 11, 12, 13, 14, 15, 16, 17};
GLuint y_2[] = {18, 19, 20, 21, 22, 23, 24, 25, 26};
//--------------------------------------------------
GLuint z_0[] = {6, 7, 8, 15, 16, 17, 24, 25, 26};
GLuint z_1[] = {3, 4, 5, 12, 13, 14, 21, 22, 23};
GLuint z_2[] = {0, 1, 2, 9, 10, 11, 18, 19, 20}; 

// Indices array used bby glDrawElements
GLint indicesArray[N_FACES * N_VERTICES];

/** Vertici per faccia del cubo */
Vertex_t verticesArray[N_VERTICES * N_FACES] = {

    //first face f1: v0, v1, v3, v2
    {{0.5, -0.5, -0.5, 1.0}, {224.0 / 255.0, 34.0 / 255.0, 34.0 / 255.0, 1.0}},
    {{0.5, 0.5, -0.5, 1.0}, {224.0 / 255.0, 34.0 / 255.0, 34.0 / 255.0, 1.0}},
    {{-0.5, -0.5, -0.5, 1.0}, {224.0 / 255.0, 34.0 / 255.0, 34.0 / 255.0, 1.0}},
    {{-0.5, 0.5, -0.5, 1.0}, {224.0 / 255.0, 34.0 / 255.0, 34.0 / 255.0, 1.0}},

    // second face f2: v7, v6, v0, v1
    {{0.5, -0.5, 0.5, 1.0}, {235.0 / 255.0, 171.0 / 255.0, 52.0 / 255.0, 1.0}},
    {{0.5, 0.5, 0.5, 1.0}, {235.0 / 255.0, 171.0 / 255.0, 52.0 / 255.0, 1.0}},
    {{0.5, -0.5, -0.5, 1.0}, {235.0 / 255.0, 171.0 / 255.0, 52.0 / 255.0, 1.0}},
    {{0.5, 0.5, -0.5, 1.0}, {235.0 / 255.0, 171.0 / 255.0, 52.0 / 255.0, 1.0}},

    // third face f3: v4, v5, v7, v6
    {{-0.5, -0.5, 0.5, 1.0}, {246.0 / 255.0, 255.0 / 255.0, 0.0, 1.0}},
    {{-0.5, 0.5, 0.5, 1.0}, {246.0 / 255.0, 255.0 / 255.0, 0.0, 1.0}},
    {{0.5, -0.5, 0.5, 1.0}, {246.0 / 255.0, 255.0 / 255.0, 0.0, 1.0}},
    {{0.5, 0.5, 0.5, 1.0}, {246.0 / 255.0, 255.0 / 255.0, 0.0, 1.0}},

    // fourth face f4: v3, v2, v4, v5
    {{-0.5, -0.5, -0.5, 1.0}, {0.0, 128.0 / 255.0, 255.0 / 255.0, 1.0}},
    {{-0.5, 0.5, -0.5, 1.0}, {0.0, 128.0 / 255.0, 255.0 / 255.0, 1.0}},
    {{-0.5, -0.5, 0.5, 1.0}, {0.0, 128.0 / 255.0, 255.0 / 255.0, 1.0}},
    {{-0.5, 0.5, 0.5, 1.0}, {0.0, 128.0 / 255.0, 255.0 / 255.0, 1.0}},

    // fifth face f5: v1, v6, v2, v5
    {{0.5, 0.5, -0.5, 1.0}, {52.0 / 255.0, 235.0 / 255.0, 125.0 / 255.0, 1.0}},
    {{0.5, 0.5, 0.5, 1.0}, {52.0 / 255.0, 235.0 / 255.0, 125.0 / 255.0, 1.0}},
    {{-0.5, 0.5, -0.5, 1.0}, {52.0 / 255.0, 235.0 / 255.0, 125.0 / 255.0, 1.0}},
    {{-0.5, 0.5, 0.5, 1.0}, {52.0 / 255.0, 235.0 / 255.0, 125.0 / 255.0, 1.0}},

    // sixth face f6: v3, v4, v0, v7
    {{-0.5, -0.5, -0.5, 1.0}, {233.0 / 255.0, 239.0 / 255.0, 245.0 / 255.0, 1.0}},
    {{-0.5, -0.5, 0.5, 1.0}, {233.0 / 255.0, 239.0 / 255.0, 245.0 / 255.0, 1.0}},
    {{0.5, -0.5, -0.5, 1.0}, {233.0 / 255.0, 239.0 / 255.0, 245.0 / 255.0, 1.0}},
    {{0.5, -0.5, 0.5, 1.0}, {233.0 / 255.0, 239.0 / 255.0, 245.0 / 255.0, 1.0}},
};

#endif