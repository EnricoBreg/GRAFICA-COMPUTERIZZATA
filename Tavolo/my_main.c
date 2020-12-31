#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <cglm/cglm.h>
#include <math.h>

#include "tavolo.h"
#include "shader.h"

unsigned int vao[1];

unsigned int
    vertexShaderId,
    fragmentShaderId,
    programId,
    projMatLoc,
    colorLoc,
    mvMatLoc;

mat4 modelViewMat = GLM_MAT4_IDENTITY_INIT;
static mat4 projMat = GLM_MAT4_IDENTITY_INIT;

void display(void)
{
    GLint indFace, indVertex;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm_mat4_identity(modelViewMat);

    glm_translate(modelViewMat, (vec3){10.0, 0.0, 0.0});
    glUniformMatrix4fv(mvMatLoc, 1, GL_FALSE, (GLfloat *)modelViewMat);

    //draw pietà
    glBindVertexArray(vao[0]);

    for (indFace = 0; indFace < NFACES; indFace++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(indFace * NVERTICES * sizeof(GLuint)));
    }

    glutPostRedisplay();
}

void init(void)
{

    GLenum glErr;

    // ... it does not hurt to check that everything went OK
    if ((glErr = glGetError()) != 0)
    {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);

    vertexShaderId = setShader("vertex", "vertexShader.glsl");
    fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    glUseProgram(programId);

    /* genero vao */
    glGenVertexArrays(1, vao);

    /* inizializzo il primo vao */
    glBindVertexArray(vao[0]);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    /* Si definisce la grandezza del buffer */
    glBufferData(GL_ARRAY_BUFFER, sizeof(legVertices), NULL, GL_STATIC_DRAW);

    // copia i vertici nel buffer
    glBufferData(GL_ARRAY_BUFFER, 0, sizeof(legVertices), legVertices);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(legIndices), legIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(legVertices[0]), 0);
    glEnableVertexAttribArray(0);

    /* fine inizializzazione VAOs */

    projMatLoc = glGetUniformLocation(programId, "projMat");
    glm_frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0, projMat);
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, (GLfloat *)projMat);

    colorLoc = glGetUniformLocation(programId, "color");
    glUniform4fv(colorLoc, 1, &colorArray[0]);

    mvMatLoc = glGetUniformLocation(programId, "modelViewMat");

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glClear(GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);

    glDisable(GL_CULL_FACE);
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
