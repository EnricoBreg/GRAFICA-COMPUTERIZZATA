#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>

#define NFACES 6
#define NVERTICES 4

void drawTable()
{
    GLint indFace;

    glPushMatrix();

    for (indFace = 0; indFace < NFACES; indFace++)
    {
        glDrawElements(GL_TRIANGLE_STRIP,                                 // mode
                       4,                                                 // count
                       GL_UNSIGNED_INT,                                   // type
                       (GLvoid *)(indFace * NVERTICES * sizeof(GLuint))); // indices
    }

    glPopMatrix();

    glPushMatrix();

    for (indFace = 0; indFace < NFACES; indFace++)
    {
        glDrawElements(GL_TRIANGLE_STRIP,                                 // mode
                       4,                                                 // count
                       GL_UNSIGNED_INT,                                   // type
                       (GLvoid *)(indFace * NVERTICES * sizeof(GLuint))); // indices
    }

    glPopMatrix();

    glPushMatrix();

    for (indFace = 0; indFace < NFACES; indFace++)
    {
        glDrawElements(GL_TRIANGLE_STRIP,                                 // mode
                       4,                                                 // count
                       GL_UNSIGNED_INT,                                   // type
                       (GLvoid *)(indFace * NVERTICES * sizeof(GLuint))); // indices
    }

    glPopMatrix();

    glPushMatrix();

    for (indFace = 0; indFace < NFACES; indFace++)
    {
        glDrawElements(GL_TRIANGLE_STRIP,                                 // mode
                       4,                                                 // count
                       GL_UNSIGNED_INT,                                   // type
                       (GLvoid *)(indFace * NVERTICES * sizeof(GLuint))); // indices
    }

    glPopMatrix();
}