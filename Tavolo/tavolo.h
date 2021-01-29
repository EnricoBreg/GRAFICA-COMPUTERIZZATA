#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h> 

#define TABLE_HEIGHT 17.5
#define TABLE_LENGHT 37.5
#define TABLE_WIDTH 18.5

#define TABLE_COLOR_RED 0.64
#define TABLE_COLOR_GREEN 0.16
#define TABLE_COLOR_BLUE 0.16

#define NFACES 6
#define NVERTICES 4

unsigned int buffers[2];

GLfloat legVertices[8 * 3] = {
    //faccia sotto
    0.0, 0.0, 0.0, //0
    1.0, 0.0, 0.0,
    1.0, 0.0, 1.0,
    0.0, 0.0, 1.0,

    //faccia sopra
    0.0, TABLE_HEIGHT, 0.0, //4
    1.0, TABLE_HEIGHT, 0.0,
    1.0, TABLE_HEIGHT, 1.0,
    0.0, TABLE_HEIGHT, 1.0   
};

GLint legIndices[] = {
    //laterale sx
    0, 4, 3, 7,

    //davanti
    3, 2, 7, 6,

    //laterale dx
    2, 1, 6, 5,

    //dietro
    1, 5, 0, 4,

    //faccia sopra
    4, 5, 7, 6,

    //faccia sotto
    0, 1, 3, 2
};

GLfloat colorArray[8 * 3] = {
    TABLE_COLOR_RED,
    TABLE_COLOR_GREEN,
    TABLE_COLOR_BLUE,

    TABLE_COLOR_RED,
    TABLE_COLOR_GREEN,
    TABLE_COLOR_BLUE,

    TABLE_COLOR_RED,
    TABLE_COLOR_GREEN,
    TABLE_COLOR_BLUE,

    TABLE_COLOR_RED,
    TABLE_COLOR_GREEN,
    TABLE_COLOR_BLUE,

    TABLE_COLOR_RED,
    TABLE_COLOR_GREEN,
    TABLE_COLOR_BLUE,
    
    TABLE_COLOR_RED,
    TABLE_COLOR_GREEN,
    TABLE_COLOR_BLUE,

    TABLE_COLOR_RED,
    TABLE_COLOR_GREEN,
    TABLE_COLOR_BLUE,

    TABLE_COLOR_RED,
    TABLE_COLOR_GREEN,
    TABLE_COLOR_BLUE
};

void initTable(GLfloat, GLfloat, GLfloat);

void drawTable();