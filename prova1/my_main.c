#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>    

#define NFACES 4
#define NVERTICES 4

unsigned int buffers[2];
unsigned int lidSottoBuffer[2];
unsigned int lidSopraBuffer[2];
unsigned int vao[3];

/* GLubyte elementIndices[NFACES*NVERTICES] =
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}; */

/* Array per gli indici dei dati nei vbo */
GLint vertexIndices[NFACES * NVERTICES];
GLint lidSottoVertexIndices[4];
GLint lidSopraVertexIndices[4];

/* Vertici facce laterali del cubo */
GLfloat vertexArray[NFACES*NVERTICES*3] = {
    /*// first face - v0,v1,v3,v2
    0.5, -0.5, 0.5, // v0
    0.5, 0.5, 0.5, // v1
    -0.5, -0.5, 0.5, // v3
    -0.5, 0.5, 0.5, // v2

    // second face - v3,v2,v7,v6
    -0.5, -0.5, 0.5, // v3
    -0.5, 0.5, 0.5, // v2
    -0.5, -0.5, -0.5, // v7
    -0.5, 0.5, -0.5, // v6

    // third face - v7,v6,v4,v5
    -0.5, -0.5, -0.5, // v7
    -0.5, 0.5, -0.5, // v6
    0.5, -0.5, -0.5, // v4
    0.5, 0.5, -0.5, // v5

    // forth face - v4, v5, v0, v1
    0.5, -0.5, -0.5, // v4
    0.5, 0.5, -0.5, // v5
    0.5, -0.5, 0.5, // v0
    0.5, 0.5, 0.5 // v1 */

    //laterale sx
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 1.0, 1.0,

    //davanti
    0.0, 0.0, 1.0, 
    1.0, 0.0, 1.0,
    0.0, 1.0, 1.0,
    1.0, 1.0, 1.0,

    //laterale dx
    1.0, 0.0, 1.0,
    1.0, 0.0, 0.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 0.0,

    //dietro
    1.0, 0.0, 0.0,
    1.0, 1.0, 0.0,
    0.0, 0.0, 0.0,
    0.0, 1.0, 0.0
};

GLfloat lidSopraVertexArray[NVERTICES * 3] = {
    //faccia sopra
    0.0, 1.0, 0.0,
    1.0, 1.0, 0.0,
    0.0, 1.0, 1.0,
    1.0, 1.0, 1.0 
};

GLfloat lidSottoVertexArray[NVERTICES * 3] = {
    //faccia sotto
    0.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    0.0, 0.0, 1.0,
    1.0, 0.0, 1.0  
};

GLfloat lidSottoColorArray[NVERTICES * 3] = {
    1.0, 1.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 0.0
};

GLfloat lidSopraColorArray[NVERTICES * 3] = {
    1.0, 1.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 0.0,
    1.0, 1.0, 0.0
};

GLfloat colorArray[NFACES*NVERTICES*3] = {
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0,
    0.0, 1.0, 1.0
};

void display(void) {
    GLint indFace, indVertex;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    // Piazza il cubo nella scena
    glTranslatef(-0.8, -0.8, -1.5);
    glRotatef(30.0, 0.0, 1.0, 0.0);
    glRotatef(25.0, 1.0, 0.0, 0.0);

    glBindVertexArray(vao[0]);

    for(indFace = 0; indFace < NFACES; indFace++) {
        glDrawElements(GL_TRIANGLE_STRIP,                               // mode
                    4,                                                  // count
                    GL_UNSIGNED_INT,                                    // type
                    (GLvoid *)(indFace * NVERTICES * sizeof(GLuint)));  // indices
    }

    
    glBindVertexArray(vao[1]);
    
    glDrawElements(GL_TRIANGLE_STRIP,                                       // mode
                4,                                                          // count
                GL_UNSIGNED_INT,                                            // type
                (GLvoid *)0);                                               // indices
    
    
    glBindVertexArray(vao[2]);
    
    glDrawElements(GL_TRIANGLE_STRIP,                               // mode
                4,                                                  // count
                GL_UNSIGNED_INT,                                    // type
                (GLvoid *)(0));                                     // indices
    

    glPopMatrix();

    glFlush();
}

void init(void) {

    GLenum glErr;

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);

    glFrustum(-0.2, 0.2, -0.3, 0.1, 0.1, 5.0);

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
    
    /* genero 3 vao */
    glGenVertexArrays(3, vao);

    /* inizializzo il primo vao */
    glBindVertexArray(vao[0]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    /* Si definisce la grandezza del buffer */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray) + sizeof(colorArray), NULL, GL_STATIC_DRAW); 
    
    // copia i vertici nella prima metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexArray), vertexArray);
    // copia i colori nella seconda metà del buffer
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexArray), sizeof(colorArray), colorArray);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

    /* for per inizializzare indici */
    unsigned int i;
    for(i = 0; i < NVERTICES * NVERTICES; i++) {
        vertexIndices[i] = i;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndices), vertexIndices, GL_STATIC_DRAW);
    
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertexArray)));

    /* inizializzo secondo vao */
    glBindVertexArray(vao[1]);

    glGenBuffers(2, lidSottoBuffer);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, lidSottoBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lidSottoVertexArray) + sizeof(lidSottoColorArray), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lidSottoVertexArray), lidSottoVertexArray);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(lidSottoVertexArray), sizeof(lidSottoColorArray), lidSottoColorArray);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lidSottoBuffer[1]);
    
    for(i = 0; i < NVERTICES; i++){
        lidSottoVertexIndices[i] = i;
    }
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lidSottoVertexIndices), lidSottoVertexIndices, GL_STATIC_DRAW);

    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)sizeof(lidSottoVertexArray));

    /* inizializzo terzo vao */
    glBindVertexArray(vao[2]);

    glGenBuffers(2, lidSopraBuffer);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, lidSopraBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lidSopraVertexArray) + sizeof(lidSopraColorArray), NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lidSopraVertexArray), lidSopraVertexArray);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(lidSottoVertexArray), sizeof(lidSopraColorArray), lidSopraColorArray);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lidSopraBuffer[1]);

    for(i = 0; i < NVERTICES; i++){
        lidSopraVertexIndices[i] = i;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lidSopraVertexIndices), lidSopraVertexIndices, GL_STATIC_DRAW);

    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)sizeof(lidSopraVertexArray));

    /* fine inizializzazione VAOs */
}


int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(0, 0);
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
