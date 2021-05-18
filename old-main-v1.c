
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <cglm/cglm.h>
#include <math.h>

/**
 * 4 vertices per face
*/
#define N_VERTICES 4

/**
 * Number of faces to draw
*/
#define N_FACES 6

#define USE_GL_FRUSTUM

/**
 * array indici-piano
*/
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

/**
 * Number of cubes
*/
const GLsizei N_CUBES = 27;

const unsigned int CubesPerPlan = 9;

float XtranslationValue = 2.0;
float YtranslationValue = -1.5;
float ZtranslationValue = -4.0;

float angleMultiplier = 90.0;
float translationMultiplier = 0.05; // BEEP BEEP!
float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;

const float cubesDistance = 1.05;

// Valori per la funzione glOrtho o glFrustum
const GLdouble left = -0.2;
const GLdouble right = 0.2;
const GLdouble bottom = -0.3;
const GLdouble top = 0.1;
const GLdouble nearVal = 0.08;
const GLdouble farVal = 100.0;

typedef struct VaoDrawingInfoStructure {
    GLfloat x_RotateAngle;
    GLfloat y_RotateAngle;
    GLfloat z_RotateAngle;
} VaoDrawingInfoStructure_t;

typedef struct Vertex
{
    float coords[N_VERTICES]; // Coordinate omogenee
    float colors[N_VERTICES]; // RGBA
} Vertex;

GLint indicesArray[N_FACES * N_VERTICES];

/** Vertici per una faccia del cubo */
Vertex verticesArray[N_VERTICES * N_FACES] = {

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

GLuint vao[27]; // VAO - Vertex Array Object
GLuint buffers[2];   // VBO - Vertex Buffer Ojbect
VaoDrawingInfoStructure_t vaoInfoStruct[27];

// Funzioni
void init(void);
void display(void);
void translationKeyInput(int key, int x, int y);
void keyInput(unsigned char key, int x, int y);

int main(int argc, char *argv[])
{
    /**
     * Prompt
    */
    printf("Interazioni: \n");
    printf("Premi i tasti x, X, y, Y, z, Z per girare la scena.\n");
    printf("Premi UP_ARROW, DOWN_ARROW, LEFT_ARROW, RIGHT_ARROW  per traslare la scena.\n");
    printf("Premi 'R' per resettare la scena.\n");

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Prova Depth Buffer - Rubik Cube");

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        printf("GLEW: Errore %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("GLEW: init ok!\n");
    }

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(translationKeyInput);
    glutMainLoop();

    return 0;
}

void init(void)
{
    unsigned int vaoIndex;
    GLenum err;

    glClearColor(0.5, 0.5, 0.5, 0.0);

    glMatrixMode(GL_PROJECTION);
#ifdef USE_GL_FRUSTUM
    glFrustum(left, right, bottom, top, nearVal, farVal);
#else
    glOrtho(left, right, bottom, top, nearVal, farVal);
#endif

    // Controllo eventuali errori
    if ((err = glGetError()) != 0)
    {
        fprintf(stderr, "ERRORE: %d\n", err);
        exit(EXIT_FAILURE);
    }

#ifdef DEBUG_MODE
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Attivazione del depth test
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Disabilito il culling delle facce (Commenta questa riga per attivarlo)
#ifdef USE_FACE_CULLING
    glEnable(GL_CULL_FACE);
#else
    glDisable(GL_CULL_FACE);
#endif

    for (size_t i = 0; i < (N_FACES * N_VERTICES); i++)
    {
        indicesArray[i] = i;
    }

    glGenVertexArrays(27, vao);

    // Genero N_CUBES vertex array objects
    for (vaoIndex = 0; vaoIndex < 27; vaoIndex++)
    {
        glBindVertexArray(vao[vaoIndex]);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glGenBuffers(2, buffers);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticesArray), verticesArray, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesArray), indicesArray, GL_STATIC_DRAW);

        glVertexPointer(4, GL_FLOAT, sizeof(verticesArray[0]), 0);
        glColorPointer(4, GL_FLOAT, sizeof(verticesArray[0]), (GLvoid *)(sizeof(verticesArray[0].colors)));
    }
}

void display(void)
{
    unsigned int faceIndex;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();

    glTranslatef(-2.0, -2.0, 0.0);
    /* set scena globale */
    glTranslatef(XtranslationValue, 0.0, 0.0);
    glTranslatef(0.0, YtranslationValue, 0.0);
    glTranslatef(0.0, 0.0, ZtranslationValue);

    glRotatef(Xangle, 1.0, 0.0, 0.0);
    glRotatef(Yangle, 0.0, 1.0, 0.0);
    glRotatef(Zangle, 0.0, 0.0, 1.0);

    glPushMatrix();

    /* Piano y_0 */
    glTranslatef(-1.1, 1.1, -1.1);

    /* Cubo #0 */
    glPushMatrix();
    glRotatef(vaoInfoStruct[0].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[0].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[0].z_RotateAngle, 0.0, 0.0, 1.0);
    

    glBindVertexArray(vao[0]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #1 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();
    glRotatef(vaoInfoStruct[1].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[1].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[1].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[1]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #2 */
    glTranslatef(1.1, 0.0, 0.0);
    
    glPushMatrix();

    glRotatef(vaoInfoStruct[2].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[2].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[2].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[2]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #3 */
    glTranslatef(-2.2, 0.0, 1.1);

    glPushMatrix();

    glRotatef(vaoInfoStruct[3].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[3].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[3].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[3]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #4 */
    glTranslatef(1.1, 0.0, 0.0);
    
    glPushMatrix();

    glRotatef(vaoInfoStruct[4].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[4].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[4].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[4]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #5 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();

    glRotatef(vaoInfoStruct[5].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[5].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[5].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[5]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #6 */
    glTranslatef(-2.2, 0.0, 1.1);

    glPushMatrix();

    glRotatef(vaoInfoStruct[6].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[6].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[6].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[6]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #7 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();

    glRotatef(vaoInfoStruct[7].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[7].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[7].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[7]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #8 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();

    glRotatef(vaoInfoStruct[8].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[8].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[8].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[8]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    glPopMatrix();

    /* Piano y_1 */
    glPushMatrix();

    glTranslatef(-1.1, 0.0, -1.1);

    /* Cubo #9 */
    glPushMatrix();

    glRotatef(vaoInfoStruct[9].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[9].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[9].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[9]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #10 */
    glTranslatef(1.1, 0.0, 0.0);
    
    glPushMatrix();
    
    glRotatef(vaoInfoStruct[10].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[10].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[10].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[10]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #11 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();
    
    glRotatef(vaoInfoStruct[11].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[11].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[11].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[1]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #12 */
    glTranslatef(-2.2, 0.0, 1.1);

    glPushMatrix();
    
    glRotatef(vaoInfoStruct[12].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[12].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[12].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[12]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #13 */
    glTranslatef(1.1, 0.0, 0.0);
    
    glPushMatrix();

    glRotatef(vaoInfoStruct[13].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[13].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[13].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[13]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #14 */
    glTranslatef(1.1, 0.0, 0.0);
    
    glPushMatrix();

    glRotatef(vaoInfoStruct[14].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[14].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[14].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[14]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #15 */
    glTranslatef(-2.2, 0.0, 1.1);

    glPushMatrix();

    glRotatef(vaoInfoStruct[15].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[15].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[15].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[15]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #16 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();
    
    glRotatef(vaoInfoStruct[16].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[16].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[16].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[16]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #17 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();
    
    glRotatef(vaoInfoStruct[17].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[17].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[17].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[17]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    glPopMatrix();

    /* Piano y_2 */

    glPushMatrix();

    glTranslatef(-1.1, -1.1, -1.1);

    /* Cubo #18 */
    glPushMatrix();

    glRotatef(vaoInfoStruct[18].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[18].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[18].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[18]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #19 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();

    glRotatef(vaoInfoStruct[19].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[19].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[19].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[19]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #20 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();

    glRotatef(vaoInfoStruct[20].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[20].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[20].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[20]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #21 */
    glTranslatef(-2.2, 0.0, 1.1);

    glPushMatrix();

    glRotatef(vaoInfoStruct[21].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[21].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[21].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[21]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #22 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();

    glRotatef(vaoInfoStruct[22].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[22].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[22].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[22]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #23 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();

    glRotatef(vaoInfoStruct[23].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[23].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[23].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[23]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();
    
    /* Cubo #24 */
    glTranslatef(-2.2, 0.0, 1.1);

    glPushMatrix();

    glRotatef(vaoInfoStruct[24].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[24].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[24].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[24]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #25 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();

    glRotatef(vaoInfoStruct[25].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[25].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[25].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[25]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    /* Cubo #26 */
    glTranslatef(1.1, 0.0, 0.0);

    glPushMatrix();

    glRotatef(vaoInfoStruct[26].x_RotateAngle, 1.0, 0.0, 0.0);
    glRotatef(vaoInfoStruct[26].y_RotateAngle, 0.0, 1.0, 0.0);
    glRotatef(vaoInfoStruct[26].z_RotateAngle, 0.0, 0.0, 1.0);

    glBindVertexArray(vao[26]);
    for (faceIndex = 0; faceIndex < N_FACES; faceIndex++)
    {
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
    }
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();

    glFinish();
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
    // glutPostRedisplay();
}

void keyInput(unsigned char key, int x, int y)
{   
    int cubeIndex;
    switch (key)
    {
    case 27:
        exit(0);
        break;
    /* q,w,e -> x */
    case 'q': 
        /* Aggiornamento valori delle posizioni dell'array 
          * VaoDrawingInfoStructure degli indici dei cubi 
          * del piano x_0 
          * Il paino x_0 ruota in senso antiorario
          */
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[x_0[cubeIndex]].x_RotateAngle += 90;
        }
        
        // glutPostRedisplay();
        break;
    case 'Q':
        /* Aggiornamento valori delle posizioni dell'array 
          * VaoDrawingInfoStructure degli indici dei cubi 
          * del piano x_0 
          * Il paino x_0 ruota in senso orario
          */ 
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[x_0[cubeIndex]].x_RotateAngle -= 90;
        }
        
        // glutPostRedisplay();
        break;

    case 'w':
        /* Aggiornamento valori delle posizioni dell'array 
          * VaoDrawingInfoStructure degli indici dei cubi 
          * del piano x_1 
          * Il paino x_1 ruota in senso antiorario
          */ 
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[x_1[cubeIndex]].x_RotateAngle += 90;
        }
        
        // glutPostRedisplay();
        break;
    case 'W':
        /* Aggiornamento valori delle posizioni dell'array 
          * VaoDrawingInfoStructure degli indici dei cubi 
          * del piano x_1 
          * Il paino x_1 ruota in senso orario
          */ 
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[x_1[cubeIndex]].x_RotateAngle -= 90;
        }
        
        // glutPostRedisplay();
        break;
    case 'e':
        /* Aggiornamento valori delle posizioni dell'array 
          * VaoDrawingInfoStructure degli indici dei cubi 
          * del piano x_2 
          * Il paino x_2 ruota in senso antiorario
          */ 
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[x_2[cubeIndex]].x_RotateAngle += 90;
        }
        
        // glutPostRedisplay();
        break;
    case 'E':
        /* Aggiornamento valori delle posizioni dell'array 
          * VaoDrawingInfoStructure degli indici dei cubi 
          * del piano x_2 
          * Il paino x_2 ruota in senso orario
          */
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[x_2[cubeIndex]].x_RotateAngle -= 90;
        }
        
        // glutPostRedisplay();
        break;
    case 'a':
        /* Aggiornamento valori delle posizioni dell'array 
          * VaoDrawingInfoStructure degli indici dei cubi 
          * del piano y_0 
          * Il paino y_0 ruota in senso antiorario
          */
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[y_0[cubeIndex]].y_RotateAngle += 90;
        }
        
        // glutPostRedisplay();
        break;
    case 'A':
        /* Aggiornamento valori delle posizioni dell'array 
          * VaoDrawingInfoStructure degli indici dei cubi 
          * del piano y_0 
          * Il paino y_0 ruota in senso orario
          */
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[y_0[cubeIndex]].y_RotateAngle -= 90;
        }
        
        // glutPostRedisplay();
        break;

    case 's':
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[y_1[cubeIndex]].y_RotateAngle += 90;
        }
        // glutPostRedisplay();
        break;

    case 'S':
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[y_1[cubeIndex]].y_RotateAngle -= 90;
        }
        // glutPostRedisplay();
        break;

    case 'd':
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[y_2[cubeIndex]].y_RotateAngle += 90;
        }
        // glutPostRedisplay();
        break;

    case 'D':
        for(cubeIndex = 0; cubeIndex < CubesPerPlan; cubeIndex++){
            vaoInfoStruct[y_2[cubeIndex]].y_RotateAngle -= 90;
        }
        // glutPostRedisplay();
        break;

    case 'y':
        Yangle += 80.0/360.0 * (2.0 * M_PI);
        // glutPostRedisplay();
        break;

    case 'Y':
        Yangle -= 80.0/360.0 * (2.0 * M_PI);
        // glutPostRedisplay();
        break;
        
    default:
        // Non fare nulla
        break;
    }
}
