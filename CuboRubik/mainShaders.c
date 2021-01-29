/*
 * OpenGL: shaders. First, simple example
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>


// Here wde define our data structures
typedef struct Vertex
{
    float coords[4];
    float colors[4];
} Vertex;

// vertex coordinates and colors
static Vertex squareVertices[4] =
{
    { { 20.0, 20.0, 0.0, 1.0 }, { 1.0, 0.0, 0.0, 1.0 } },
    { { 80.0, 20.0, 0.0, 1.0 }, { 1.0, 1.0, 0.0, 1.0 } },
    { { 20.0, 80.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 } },
    { { 80.0, 80.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 } }
};

typedef struct Matrix4x4
{
    float entries[16];
} Matrix4x4;

// GLOBALS
static Matrix4x4 modelViewMat;
static Matrix4x4 projMat;

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
buffer[1],
vao[1];

// Drawing routine.
void display(void)
{
    // set clear color
    glClear(GL_COLOR_BUFFER_BIT);

    // draw square
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFlush();
}


// Function to read text file.
char* readTextFile(char* aTextFile)
{
    FILE* filePointer = fopen(aTextFile, "rb");
    char* content = NULL;
    long numVal = 0;

    // estimate buffer length
    fseek(filePointer, 0L, SEEK_END);
    numVal = ftell(filePointer);

    fseek(filePointer, 0L, SEEK_SET);
    content = (char*) malloc((numVal+1) * sizeof(char));
    fread(content, 1, numVal, filePointer);
    content[numVal] = '\0';
    fclose(filePointer);
    return content;
}

// Initialization routine.
void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);

    // Create shader program executable.
    char* vertexShader = readTextFile("vertexShader.glsl");
    vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, (const char**) &vertexShader, NULL);
    glCompileShader(vertexShaderId);

    char* fragmentShader = readTextFile("fragmentShader.glsl");
    fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderId, 1, (const char**) &fragmentShader, NULL);
    glCompileShader(fragmentShaderId);

    programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);
    glLinkProgram(programId);
    glUseProgram(programId);
    // -------------------------------------------

    // Create VAO and VBO and associate data with vertex shader.
    glGenVertexArrays(1, vao);
    glGenBuffers(1, buffer);

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices),
        squareVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
        sizeof(squareVertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                          sizeof(squareVertices[0]),
                          (GLvoid*)sizeof(squareVertices[0].coords));
    glEnableVertexAttribArray(1);
    // -------------------------------------------

    // Obtain projection matrix uniform location and set value.
    Matrix4x4 projMat = {
        {
            0.02, 0.0,  0.0, -1.0,
            0.0,  0.02, 0.0, -1.0,
            0.0,  0.0, -1.0,  0.0,
            0.0,  0.0,  0.0,  1.0
        }
    };
    projMatLoc = glGetUniformLocation(programId, "projMat");
    glUniformMatrix4fv(projMatLoc, 1, GL_TRUE, projMat.entries);
    // -------------------------------------------

    // Obtain modelview matrix uniform location and set value.
    Matrix4x4 modelViewMat = {
        {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        }
    };
    modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_TRUE, modelViewMat.entries);
    // -------------------------------------------
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
    glViewport(0, 0, w, h);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch(key) {
    // ESC button
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

// Main routine.
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    // set OpenGL version
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    // set glut parameters and functions
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("My first shaders");
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyInput);

    glewInit();

    init();

    glutMainLoop();
}
