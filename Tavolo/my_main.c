#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>    
#include "dati.h"

unsigned int vao[5];
GLfloat angle = 0;

void display(void) {
    GLint indFace, indVertex;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();

    glRotatef(30.0, 0.0, 1.0, 0.0);
    glRotatef(40.0, 1.0, 0.0, 0.0);
    //glTranslatef(17.25, 16.7, -9.375);
    glTranslatef(20.0, 0.0, -9.375);

    glPushMatrix();

    glTranslatef(0.0, -0.8, -30.0);

    glBindVertexArray(vao[0]);

    for(indFace = 0; indFace < NFACES; indFace++) {
        glDrawElements(GL_TRIANGLE_STRIP,                               // mode
                    4,                                                  // count
                    GL_UNSIGNED_INT,                                    // type
                    (GLvoid *)(indFace * NVERTICES * sizeof(GLuint)));  // indices
    }

    glPopMatrix();

    glPushMatrix();

    glTranslatef(30.5, -0.8, -30.0);

    glBindVertexArray(vao[1]);
    
    for(indFace = 0; indFace < NFACES; indFace++) {
        glDrawElements(GL_TRIANGLE_STRIP,                               // mode
                    4,                                                  // count
                    GL_UNSIGNED_INT,                                    // type
                    (GLvoid *)(indFace * NVERTICES * sizeof(GLuint)));  // indices
    }                                              

    glPopMatrix();
    
    glPushMatrix();

    glTranslatef(0.0, -0.8, -15.0);

    glBindVertexArray(vao[2]);
    
    for(indFace = 0; indFace < NFACES; indFace++) {
        glDrawElements(GL_TRIANGLE_STRIP,                               // mode
                    4,                                                  // count
                    GL_UNSIGNED_INT,                                    // type
                    (GLvoid *)(indFace * NVERTICES * sizeof(GLuint)));  // indices
    }                                              

    glPopMatrix();

    glPushMatrix();

    glTranslatef(30.5, -0.8, -15.0);

    glBindVertexArray(vao[3]);
    
    for(indFace = 0; indFace < NFACES; indFace++) {
        glDrawElements(GL_TRIANGLE_STRIP,                               // mode
                    4,                                                  // count
                    GL_UNSIGNED_INT,                                    // type
                    (GLvoid *)(indFace * NVERTICES * sizeof(GLuint)));  // indices
    }                                              

    glPopMatrix();

    glPushMatrix();

    glTranslatef(-3.0, 16.7, -31.0);

    
    /* Superficie del tavolo */

    glBindVertexArray(vao[4]);
    
    for(indFace = 0; indFace < NFACES; indFace++) {
        glDrawElements(GL_TRIANGLE_STRIP,                               // mode
                    4,                                                  // count
                    GL_UNSIGNED_INT,                                    // type
                    (GLvoid *)(indFace * NVERTICES * sizeof(GLuint)));  // indices
    }                                        
    
    glPopMatrix();

    glPopMatrix();

    glFlush();

    angle += 1;

    glutPostRedisplay();
}

void init(void) {

    GLenum glErr;

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);

    //glFrustum(-10.0, 50.0, -1.0, 30.0, 2.0, 100.0);
    glOrtho(-10.0, 50.0, -10.0, 50.0, 0.1, 100.0);

    // ... it does not hurt to check that everything went OK
    if ((glErr=glGetError()) != 0) {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }

    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClear(GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);
    
    glDisable(GL_CULL_FACE);
    
    /* genero 4 vao */
    glGenVertexArrays(4, vao);

    /* inizializzo il primo vao */
    glBindVertexArray(vao[0]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glGenBuffers(2, gamba1buffers);

    glBindBuffer(GL_ARRAY_BUFFER, gamba1buffers[0]);
    /* Si definisce la grandezza del buffer */
    glBufferData(GL_ARRAY_BUFFER, sizeof(legsVertexArray) + sizeof(colorArray), NULL, GL_STATIC_DRAW); 
    
    // copia i vertici nella prima metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(legsVertexArray), legsVertexArray);
    // copia i colori nella seconda metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(legsVertexArray), sizeof(colorArray), colorArray);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gamba1buffers[1]);

    /* for per inizializzare indici */
    unsigned int i;
    for(i = 0; i < NVERTICES * NFACES; i++){
        legsVertexIndices[i] = i;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(legsVertexIndices), legsVertexIndices, GL_STATIC_DRAW);
    
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(legsVertexArray)));

    /* inizializzo secondo vao */
    glBindVertexArray(vao[1]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glGenBuffers(2, gamba2buffers);

    glBindBuffer(GL_ARRAY_BUFFER, gamba2buffers[0]);
    /* Si definisce la grandezza del buffer */
    glBufferData(GL_ARRAY_BUFFER, sizeof(legsVertexArray) + sizeof(colorArray), NULL, GL_STATIC_DRAW); 
    
    // copia i vertici nella prima metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(legsVertexArray), legsVertexArray);
    // copia i colori nella seconda metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(legsVertexArray), sizeof(colorArray), colorArray);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gamba2buffers[1]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(legsVertexIndices), legsVertexIndices, GL_STATIC_DRAW);
    
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(legsVertexArray)));

    /* inizializzo terzo vao */
    glBindVertexArray(vao[2]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glGenBuffers(2, gamba3buffers);

    glBindBuffer(GL_ARRAY_BUFFER, gamba3buffers[0]);
    /* Si definisce la grandezza del buffer */
    glBufferData(GL_ARRAY_BUFFER, sizeof(legsVertexArray) + sizeof(colorArray), NULL, GL_STATIC_DRAW); 
    
    // copia i vertici nella prima metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(legsVertexArray), legsVertexArray);
    // copia i colori nella seconda metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(legsVertexArray), sizeof(colorArray), colorArray);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gamba3buffers[1]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(legsVertexIndices), legsVertexIndices, GL_STATIC_DRAW);
    
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(legsVertexArray)));

    /* inizializzo quarto vao */
    glBindVertexArray(vao[3]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glGenBuffers(2, gamba4buffers);

    glBindBuffer(GL_ARRAY_BUFFER, gamba4buffers[0]);
    /* Si definisce la grandezza del buffer */
    glBufferData(GL_ARRAY_BUFFER, sizeof(legsVertexArray) + sizeof(colorArray), NULL, GL_STATIC_DRAW); 
    
    // copia i vertici nella prima metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(legsVertexArray), legsVertexArray);
    // copia i colori nella seconda metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(legsVertexArray), sizeof(colorArray), colorArray);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gamba4buffers[1]);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(legsVertexIndices), legsVertexIndices, GL_STATIC_DRAW);
    
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(legsVertexArray)));

    /* inizializzo quinto vao */
    glBindVertexArray(vao[4]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glGenBuffers(2, surfaceBuffers);

    glBindBuffer(GL_ARRAY_BUFFER, surfaceBuffers[0]);
    /* Si definisce la grandezza del buffer */
    glBufferData(GL_ARRAY_BUFFER, sizeof(surfaceVertexArray) + sizeof(colorArray), NULL, GL_STATIC_DRAW); 
    
    // copia i vertici nella prima metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(surfaceVertexArray), surfaceVertexArray);
    // copia i colori nella seconda metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(surfaceVertexArray), sizeof(colorArray), colorArray);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surfaceBuffers[1]);

    for(i = 0; i < NVERTICES * NFACES; i++) {
        surfaceVertexIndices[i] = i;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(legsVertexIndices), legsVertexIndices, GL_STATIC_DRAW);
    
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(surfaceVertexArray)));

    /* fine inizializzazione VAOs */
}


int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(2000, 2000);
    glutCreateWindow("Prima prova");

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "GLEW init failed: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("GLEW init success\n");
    }

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
