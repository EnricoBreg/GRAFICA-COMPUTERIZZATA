#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>   

#define NFACES 6
#define NVERTICES 4

/* GAMBE TAVOLO */
unsigned int gamba1buffers[2];
unsigned int gamba2buffers[2];
unsigned int gamba3buffers[2];
unsigned int gamba4buffers[2];

/* PIANO DEL TAVOLO */
unsigned int surfaceBuffers[2];

/* ==================================================================== */
//
/*                            GAMBE TAVOLO                              */


/* Array per gli indici dei dati nei vbo */
GLint legsVertexIndices[NFACES * NVERTICES];

/* Vertici  */
GLfloat legsVertexArray[NFACES*NVERTICES*3] = {

    //laterale sx
    0.0, 0.0, 0.0,
    0.0, 17.5, 0.0,
    0.0, 0.0, 1.0,
    0.0, 17.5, 1.0,

    //davanti
    0.0, 0.0, 1.0, 
    1.0, 0.0, 1.0,
    0.0, 17.5, 1.0,
    1.0, 17.5, 1.0,

    //laterale dx
    1.0, 0.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 17.5, 1.0,
    1.0, 17.5, 0.0,

    //dietro
    1.0, 0.0, 0.0,
    1.0, 17.5, 0.0,
    0.0, 0.0, 0.0,
    0.0, 17.5, 0.0,
    
    //faccia sopra
    0.0, 17.5, 0.0,
    1.0, 17.5, 0.0,
    0.0, 17.5, 1.0,
    1.0, 17.5, 1.0,
    
    //faccia sotto
    0.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 0.0, 1.0  
};

/* ==================================================================== */
//
/*                           SUPERFICIE TAVOLO                          */

/* Indici elementi array del tavolo */
GLint surfaceVertexIndices[NFACES * NVERTICES];

/* Vertici superficie del tavolo */
GLfloat surfaceVertexArray[NFACES*NVERTICES*3] = {

    //laterale sx
    0.0, 0.0, 0.0,
    0.0, 0.75, 0.0,
    0.0, 0.0, 18.75,
    0.0, 0.75, 18.75,

    //davanti
    0.0, 0.0, 18.75, 
    37.5, 0.0, 18.75,
    0.0, 0.75, 18.75,
    37.5, 0.75, 18.75,

    //laterale dx
    37.5, 0.0, 18.75,
    37.5, 0.0, 0.0,
    37.5, 0.75, 18.75,
    37.5, 0.75, 0.0,

    //dietro
    37.5, 0.0, 0.0,
    37.5, 0.75, 0.0,
    0.0, 0.0, 0.0,
    0.0, 0.75, 0.0,
    
    //faccia sopra
    0.0, 0.75, 0.0,
    37.5, 0.75, 0.0,
    0.0, 0.75, 18.75,
    37.5, 0.75, 18.75,
    
    //faccia sotto
    0.0, 0.0, 0.0,
    37.5, 0.0, 0.0,
    0.0, 0.0, 18.75,
    37.5, 0.0, 18.75  
    
};


/* ======================================================== */
/*                          COLORI                          */
GLfloat colorArray[NFACES*NVERTICES*3] = {
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,

    0.64, 1.0, 0.16,
    0.64, 1.0, 0.16,
    0.64, 1.0, 0.16,
    0.64, 1.0, 0.16,

    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,

    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,

    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,

    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16,
    0.64, 0.16, 0.16
};