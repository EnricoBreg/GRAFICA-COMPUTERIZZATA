/* Main with shaders */
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <cglm/cglm.h>
#include <math.h>

#include "shadersf.h"
#include "modelViewStack.h"

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

/**
 * Maximum number of rotations
*/
#define MAX_ROT 200

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

float XtranslationValue = 2.0;
float YtranslationValue = -1.5;
float ZtranslationValue = -4.0;

float angleMultiplier = 90.0;
float translationMultiplier = 0.05; // BEEP BEEP!
float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;

float cubesDistance = 1.05;
const float MIN_CUBE_DISTANCE = 1.05;
const float MAX_CUBE_DISTANCE = 2.0;
const float CUBE_DIST_ADD_STEP = 0.05;

const GLfloat CwRotValue = M_PI / 2.0f;   // 90.0 degrees clockwise rotation;
const GLfloat CcwRotValue = -M_PI / 2.0f; // -90.0 degrees counter-clockwise rotation;

float angleX, angleY;

// Valori per la funzione glOrtho o glFrustum
const GLdouble left = -0.2;
const GLdouble right = 0.2;
const GLdouble bottom = -0.3;
const GLdouble top = 0.1;
const GLdouble nearVal = 0.08;
const GLdouble farVal = 100.0;

// ----------------------------- SHADERS DATA----------------------------- //

mat4 modelViewMat = GLM_MAT4_IDENTITY_INIT, projMat = GLM_MAT4_IDENTITY_INIT;

unsigned int
    vertexShadersId,
    fragmentShadersId,
    programId,
    projMatLoc,
    modelViewMatLoc;

// ----------------------------- END SHADERS DATA----------------------------- //

// ----------------------------- MODELVIEW STACK ----------------------------- //
modelViewStack_t modelViewStack;
// ----------------------------- END MODELVIEW STACK ----------------------------- //

typedef struct mousePositionStruct
{
    int mousePositionX;     // Current mouse position on axis X
    int mousePositionY;     // Current mouse position on axis Y
    int prevMousePositionX; // Previous mouse position on axis X
    int prevMousePositionY; // Previous mouse position on axis Y
} mousePositionStruct_t;

mousePositionStruct_t mousePos;
const float mouseSensX = 0.03; // Mouse Sensibility on axis X and Y

/**
 * La vera e propria rotazione viene aggiunta nella coda di rotazioni definita in
 * VaoDrawingInfoStructure.RotationStructure.rotHistory
 */
char inputKey;

typedef struct RotationStructure
{
    char rotHistory[MAX_ROT]; // array per le rotazioni
    int lastRotation;         // Posizione dell'ultimo elemento nella coda
} RotationStructure_t;

typedef struct VaoDrawingInfoStructure
{
    RotationStructure_t rotation;
} VaoDrawingInfoStructure_t;

typedef struct Vertex
{
    float coords[N_VERTICES]; // Coordinate omogenee
    float colors[N_VERTICES]; // RGBA
} Vertex_t;

GLint indicesArray[N_FACES * N_VERTICES];

/** Vertici per una faccia del cubo */
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

GLuint vao[N_CUBES]; // VAO - Vertex Array Object
GLuint buffers[2];   // VBO - Vertex Buffer Ojbect
VaoDrawingInfoStructure_t vaoInfoStruct[N_CUBES];

// Funzioni
void init(void);                                  // OpenGL initialization routine (required by OpenGL)
void display(void);                               // OpenGL routine to draw the scene (required by OpenGL)
void printCommandsOnBash(void);                              // OpenGL routine to make a simple menu
void translationKeyInput(int key, int x, int y);  // Define keys to translate the Rubik Cube on screen (deprecated)
void keyInput(unsigned char key, int x, int y);   // Define keys to interact with Rubik Cube
void initIndices(void);                           // Init indices to correctly draw the cube
void drawCube(void);                              // Draw the rubik cube
void addRotation(int cubeIndex, char key);        // Add a rotation (charID) in rotation history of Rubik Cube
void initMousePosition(void);                     // Init mouse coordinates (X and Y) for struct RotationStructure
void mouseRotation(int x, int y);                 // Mouse routine to make the Rubik Cube rotate in the scene
void writeBitmapString(void *font, char *string); // Routine to show some strings on screen (BitMap -> Indipendent from the scene)
void rotationSwapIndices(int a, int b, int c, int d, int e, int f, int g, int h);

int main(int argc, char *argv[])
{
    // Command prompt on bash
    printCommandsOnBash();

    glutInit(&argc, argv);

    // OpenGL Context Version Initialization
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    // Create Window
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Cubik");

    // Checking if errors occured
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("GLEW: Errore %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    else
    {
        //printf("GLEW: init ok!\n");
    }

    // Setting function for interactions
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);
    glutMotionFunc(mouseRotation);
    glutSpecialFunc(translationKeyInput);

    // Calling init function
    init();

    // Enter in OpengGL main loop
    glutMainLoop();

    // ANSI C requires main function returning int value
    return 0;
}

void init(void)
{
    unsigned int vaoIndex;
    GLenum err;

    glClearColor(0.5, 0.5, 0.5, 0.0);

    // Checking for errors
    if ((err = glGetError()) != 0)
    {
        fprintf(stderr, "ERRORE: %d\n", err);
        exit(EXIT_FAILURE);
    }

    // Setting shaders ProjectionMatrix and ModelViewMatrix
    vertexShadersId = setShader("vertex", "VertexShader.glsl");
    fragmentShadersId = setShader("fragment", "FragmentShader.glsl");

    programId = glCreateProgram();
    glAttachShader(programId, vertexShadersId);
    glAttachShader(programId, fragmentShadersId);

    glLinkProgram(programId);
    glUseProgram(programId);

    glm_frustum(left, right, bottom, top, nearVal, farVal, projMat);
    projMatLoc = glGetUniformLocation(programId, "projMat");
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, (GLfloat *)projMat);

    modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)modelViewMat);

    // Attivazione del depth test
    glEnable(GL_DEPTH_TEST);

    for (size_t i = 0; i < (N_FACES * N_VERTICES); i++)
    {
        indicesArray[i] = i;
    }

    glGenVertexArrays(N_CUBES, vao);

    // Genero N_CUBES vertex array objects
    for (vaoIndex = 0; vaoIndex < N_CUBES; vaoIndex++)
    {
        glBindVertexArray(vao[vaoIndex]);
        glGenBuffers(2, buffers);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesArray), verticesArray, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesArray), indicesArray, GL_STATIC_DRAW);
        // Vertices
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(verticesArray[0]), 0);
        glEnableVertexAttribArray(0);
        // Colors
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(verticesArray[0]), (GLvoid *)(sizeof(verticesArray[0].colors)));
        glEnableVertexAttribArray(1);
    }

    initIndices();
    initMousePosition(); // Initialization of MousePositionStructure
}

void display(void)
{
    char mousePosString[20];
    unsigned int faceIndex;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm_mat4_identity(modelViewMat);
    pushMatrix(&modelViewStack, modelViewMat);

    // show mouse position in bitmap characters
    glColor3f(0.0, 0.0, 0.0);
    /* snprintf(mousePosString, 20, "x=%d - y=%d", mousePos.mousePositionX, mousePos.mousePositionY);
    glRasterPos3f(1.0, 1.0, -1.0);
    writeBitmapString(GLUT_BITMAP_8_BY_13, mousePosString) */
    ;

    glm_translate(modelViewMat, (vec4){-2.0, -2.0, -0.5, 0.0});
    /* set scena globale */
    glm_translate(modelViewMat, (vec4){XtranslationValue, 0.0, 0.0, 0.0});
    glm_translate(modelViewMat, (vec4){0.0, YtranslationValue, 0.0, 0.0});
    glm_translate(modelViewMat, (vec4){0.0, 0.0, ZtranslationValue, 0.0});

    /* glRotatef(Xangle, 1.0, 0.0, 0.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Zangle, 0.0, 0.0, 1.0); */

    // Mouse Rotation
    glm_rotate(modelViewMat, angleY, (vec3){1.0, 0.0, 0.0});
    glm_rotate(modelViewMat, angleX, (vec3){0.0, 1.0, 0.0});

    pushMatrix(&modelViewStack, modelViewMat);

    /* Origine disegno */
    glm_translate(modelViewMat, (vec4){-1.1, 1.1, -1.1, 0.0});

    // Disegno il cubo
    drawCube();

    popMatrix(&modelViewStack, modelViewMat);

    popMatrix(&modelViewStack, modelViewMat);

    glFinish();
}
void printCommandsOnBash(void) 
{   
    // Print command prompt on terminal
    printf("\nCubik - A Rubik Cube Simple Game :-) \t(v. 2.0)\n");
    printf("(sviluppato da Gaia Rossi e Enrico Bregoli)\n\n");
    printf("Comandi:\n");
    printf("  - LEFT CLICK+DRAG per ruotare il cubo\n");
    printf("  - Premi 'q' ('Q'), 'w' ('W'), 'e' ('E') per muovere le facce del cubo VERTICALMENTE in senso oraio (anti-orario).\n");
    printf("  - Premi 'a' ('A'), 's' ('S'), 'd' ('D') per muovere le facce del cubo ORIZZONTALMENTE in senso oraio (anti-orario).\n");
    printf("  - Premi 'z' ('Z'), 'x' ('X'), 'c' ('C') per muovere le facce del cubo LUNGO L'ASSE Z in senso oraio (anti-orario).\n");
    printf("  - Premi 'n' per AUMENTARE la distanza tra i cubi\n");
    printf("  - Premi 'm' per DIMINUIRE la distanza tra i cubi\n");
    printf("\n\n\n");
    
    printf("© Copyright 2021, all rights reserved to creators.\n\n");
}

void translationKeyInput(int key, int x, int y)
{
    if (key == GLUT_KEY_DOWN)
        ZtranslationValue -= translationMultiplier;
    if (key == GLUT_KEY_UP)
        ZtranslationValue += translationMultiplier;
    if (key == GLUT_KEY_RIGHT)
        XtranslationValue += translationMultiplier;
    if (key == GLUT_KEY_LEFT)
        XtranslationValue -= translationMultiplier;
    glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y)
{
    int tmp;
    switch (key)
    {
    case 27: // ESC
        exit(0);
        break;

    case 'q':

        /* rotazione oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(0, 18, 24, 6, 3, 9, 21, 15);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_0[i]], 'q');
        }

        glutPostRedisplay();

        break;

    case 'Q':
        /* rotazione anti-oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(0, 6, 24, 18, 3, 15, 21, 9);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_0[i]], 'Q');
        }

        glutPostRedisplay();

        break;

    case 'w':
        /* rotazione oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(7, 1, 19, 25, 4, 16, 22, 10);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_1[i]], 'w');
        }

        glutPostRedisplay();

        break;

    case 'W':
        /* rotazione anti-oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(7, 25, 19, 1, 4, 10, 22, 16);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_1[i]], 'W');
        }

        glutPostRedisplay();

        break;

    case 'e':
        /* rotazione oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(8, 2, 20, 26, 5, 11, 23, 17);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_2[i]], 'e');
        }

        glutPostRedisplay();

        break;

    case 'E':
        /* rotazione oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(8, 26, 20, 2, 5, 17, 23, 11);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_2[i]], 'E');
        }

        glutPostRedisplay();

        break;

    case 'a':
        /* rotazione oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(0, 6, 8, 2, 1, 3, 7, 5);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_0[i]], 'a');
        }

        glutPostRedisplay();

        break;

    case 'A':
        /* rotazione anti-oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(0, 2, 8, 6, 1, 5, 7, 3);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_0[i]], 'A');
        }

        glutPostRedisplay();

        break;

    case 's':
        /* rotazione oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(9, 15, 17, 11, 12, 16, 14, 10);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_1[i]], 's');
        }

        glutPostRedisplay();

        break;

    case 'S':

        /* rotazione anti-oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(9, 11, 17, 15, 12, 10, 14, 16);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_1[i]], 'S');
        }

        glutPostRedisplay();

        break;

    case 'd':
        /* rotazione oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(18, 24, 26, 20, 21, 25, 23, 19);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_2[i]], 'd');
        }

        glutPostRedisplay();

        break;

    case 'D':
        /* rotazione anti-oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(18, 20, 26, 24, 19, 23, 25, 21);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_2[i]], 'D');
        }

        glutPostRedisplay();

        break;

    case 'z':
        /* rotazione oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(6, 24, 26, 8, 7, 15, 25, 17);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_0[i]], 'z');
        }

        glutPostRedisplay();

        break;

    case 'Z':
        /* Rotazione antioraria */

        /* Aggiornamento indici */
        rotationSwapIndices(6, 8, 26, 24, 7, 17, 25, 15);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_0[i]], 'Z');
        }

        break;

    case 'x':
        /* rotazione oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(3, 21, 23, 5, 4, 12, 22, 14);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_1[i]], 'x');
        }

        glutPostRedisplay();

        break;

    case 'X':
        /* rotazione anti-oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(3, 5, 23, 21, 4, 14, 22, 12);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_1[i]], 'X');
        }

        glutPostRedisplay();

        break;

    case 'c':
        /* rotazione oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(0, 18, 20, 2, 1, 9, 19, 11);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_2[i]], 'c');
        }

        glutPostRedisplay();

        break;

    case 'C':
        /* rotazione anti-oraria */

        /* Aggiornamento indici */
        rotationSwapIndices(0, 2, 20, 18, 1, 11, 19, 9);

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_2[i]], 'C');
        }

        glutPostRedisplay();

        break;

    case 'y':
        Yangle += (90.0 / 360.0) * (2 * M_PI);
        glutPostRedisplay();
        break;
        glutPostRedisplay();

    case 'Y':
        Yangle -= (90.0 / 360.0) * (2 * M_PI);
        glutPostRedisplay();
        break;

    case 'n':
        if (cubesDistance <= MAX_CUBE_DISTANCE)
        {
            cubesDistance += CUBE_DIST_ADD_STEP;
            glutPostRedisplay();
        }

        break;

    case 'm':
        if (cubesDistance > MIN_CUBE_DISTANCE)
        {
            cubesDistance -= CUBE_DIST_ADD_STEP;
            glutPostRedisplay();
        }

        break;

    default:
        // Non fare nulla
        break;
    }
}

void initIndices()
{
    int i;
    for (i = 0; i < N_CUBES; i++)
    {
        cubeIndices[i] = i;
    }
    return;
}

void drawCube()
{
    int cubeNo = 0;
    int i, j, k;

    for (i = 0; i < 3; i++)
    {
        pushMatrix(&modelViewStack, modelViewMat);

        for (j = 0; j < 3; j++)
        {
            pushMatrix(&modelViewStack, modelViewMat);

            for (k = 0; k < 3; k++)
            {
                // Un cubo per fila
                pushMatrix(&modelViewStack, modelViewMat);
                // disegno un cubo
                for (int rot = vaoInfoStruct[cubeIndices[cubeNo]].rotation.lastRotation; rot >= 0; rot--)
                {
                    inputKey = vaoInfoStruct[cubeIndices[cubeNo]].rotation.rotHistory[rot];

                    // Rotazione asse X
                    if (inputKey == 'q' || inputKey == 'w' || inputKey == 'e')
                    {
                        glm_rotate(modelViewMat, CwRotValue, (vec3){1.0, 0.0, 0.0});
                    }

                    if (inputKey == 'Q' || inputKey == 'W' || inputKey == 'E')
                    {
                        glm_rotate(modelViewMat, CcwRotValue, (vec3){1.0, 0.0, 0.0});
                    }

                    // Rotazione asse Y
                    if (inputKey == 'a' || inputKey == 's' || inputKey == 'd')
                    {
                        glm_rotate(modelViewMat, CcwRotValue, (vec3){0.0, 1.0, 0.0});
                    }

                    if (inputKey == 'A' || inputKey == 'S' || inputKey == 'D')
                    {
                        glm_rotate(modelViewMat, CwRotValue, (vec3){0.0, 1.0, 0.0});
                    }

                    // Rotazioni asse Z
                    if (inputKey == 'z' || inputKey == 'x' || inputKey == 'c')
                    {
                        glm_rotate(modelViewMat, CcwRotValue, (vec3){0.0, 0.0, 1.0});
                    }

                    if (inputKey == 'Z' || inputKey == 'X' || inputKey == 'C')
                    {
                        glm_rotate(modelViewMat, CwRotValue, (vec3){0.0, 0.0, 1.0});
                    }
                }

                /* printf("Matrice per cubo %d:\n", cubeNo);
                printMatrix(modelViewMat); */

                glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)modelViewMat);

                glBindVertexArray(vao[cubeIndices[cubeNo]]);

                for (int faceIndex = 0; faceIndex < N_FACES; faceIndex++)
                {
                    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
                }
                // mi sposto a destra
                popMatrix(&modelViewStack, modelViewMat);

                glm_translate(modelViewMat, (vec4){cubesDistance, 0.0, 0.0, 0.0});

                // Incremento cubeNo
                cubeNo++;
            }
            popMatrix(&modelViewStack, modelViewMat);

            glm_translate(modelViewMat, (vec4){0.0, 0.0, cubesDistance, 0.0});
        }

        popMatrix(&modelViewStack, modelViewMat);

        glm_translate(modelViewMat, (vec4){0.0, -cubesDistance, 0.0, 0.0});
    }

    glutPostRedisplay();
}

/**
 * @brief Routine per aggiornamento indici rotazione oraria/anti-oraria 
 * 
 * ================== Indici cubi angoli (a, b, c, d) ==================
 * ordine di aggiornamento tmp <- a <- b <- c <- d <-tmp 
 * (Viceversa per rotazione anti-oraria)
 * @param a 1st cube index
 * @param b 2st cube index
 * @param c 3rd cube index
 * @param d 4th cube index
 * ================== Indici cubi centrali (e, f, g, h) ==================
 * ordine di aggiornamento tmp <- e <- f <- g <- h <-tmp 
 * (Viceversa per rotazione anti-oraria)
 * @param e 5th cube index
 * @param f 6th cube index
 * @param g 7th cube index
 * @param h 8th cube index
 */
void rotationSwapIndices(int a, int b, int c, int d, int e, int f, int g, int h)
{
    int tmp;

    /* Aggiornamento "angoli" */
    tmp = cubeIndices[a];
    cubeIndices[a] = cubeIndices[b];
    cubeIndices[b] = cubeIndices[c];
    cubeIndices[c] = cubeIndices[d];
    cubeIndices[d] = tmp;

    /* Aggiornamento "quelli di mezzo" (Mordor) */
    tmp = cubeIndices[e];
    cubeIndices[e] = cubeIndices[f];
    cubeIndices[f] = cubeIndices[g];
    cubeIndices[g] = cubeIndices[h];
    cubeIndices[h] = tmp;

    return;
}

void addRotation(int cubeIndex, char key)
{
    int index = vaoInfoStruct[cubeIndex].rotation.lastRotation;
    if (index != MAX_ROT - 1)
    {
        vaoInfoStruct[cubeIndex].rotation.rotHistory[index + 1] = key;
        vaoInfoStruct[cubeIndex].rotation.lastRotation++;
    }
    else
    {
        printf("Hai perso!");
        exit(EXIT_FAILURE);
    }
}

void initMousePosition(void)
{
    mousePos.mousePositionX = mousePos.mousePositionY = mousePos.prevMousePositionX = mousePos.prevMousePositionY = 0;
}

void mouseRotation(int x, int y)
{

    mousePos.mousePositionX = x;
    mousePos.mousePositionY = y;

    // Case vettore spostamento con verso positivo per le X
    if (mousePos.prevMousePositionX < mousePos.mousePositionX)
    {
        // Rotazione asse Y oraria...
        angleX += mouseSensX;
        // ... aggiornamento prevMousePosition
        mousePos.prevMousePositionX = mousePos.mousePositionX;
    }
    // Case vettore spostamento con verso positivo per le X
    if (mousePos.prevMousePositionX > mousePos.mousePositionX)
    {
        // Rotazione asse Y anti-oraria...
        angleX -= mouseSensX;
        // ... aggiornamento prevMousePosition
        mousePos.prevMousePositionX = mousePos.mousePositionX;
    }

    // Case vettore spostamento con verso positivo per le Y
    if (mousePos.prevMousePositionY > mousePos.mousePositionY)
    {
        // Rotazione asse X oraria...
        angleY += mouseSensX;
        // ... aggiornamento prevMousePosition
        mousePos.prevMousePositionY = mousePos.mousePositionY;
    }
    // Case vettore spostamento con verso positivo per le Y
    if (mousePos.prevMousePositionY < mousePos.mousePositionY)
    {
        // Rotazione asse X oraria...
        angleY -= mouseSensX;
        // ... aggiornamento prevMousePosition
        mousePos.prevMousePositionY = mousePos.mousePositionY;
    }

    glutPostRedisplay();
}

void writeBitmapString(void *font, char *string)
{
    char *c;
    for (c = string; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

/**
 * array indici-piano
 * 
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
*/
