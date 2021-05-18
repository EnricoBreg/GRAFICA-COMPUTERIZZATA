
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

const GLfloat CwRotValue  =  90.0; 
const GLfloat CcwRotValue = -90.0; 

/**
 * inputKey, serve per escludere rotazioni involontarie sugli assi
 * Possibile soluzione al problema dovuto alla struttura dati VaoDrawingInfoStructure
 * Si aggiorna ad ogni pressione dei tasti dedicati alle rotazioni
 * delle facce del cubo di rubick, valore iniziale NULL.
 * Il valore di inputKey viene controllato prima di effettuare delle rotazioni
 * nella routine display.
 */
char inputKey;

// Valori per la funzione glOrtho o glFrustum
const GLdouble left = -0.2;
const GLdouble right = 0.2;
const GLdouble bottom = -0.3;
const GLdouble top = 0.1;
const GLdouble nearVal = 0.08;
const GLdouble farVal = 100.0;

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

GLuint vao[27];    // VAO - Vertex Array Object
GLuint buffers[2]; // VBO - Vertex Buffer Ojbect
VaoDrawingInfoStructure_t vaoInfoStruct[27];

// Funzioni
void init(void);
void display(void);
void translationKeyInput(int key, int x, int y);
void keyInput(unsigned char key, int x, int y);
void initIndices();
void drawCube();
void displayLost();
void addRotation(int cubeIndex, char key);

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

    glFrustum(left, right, bottom, top, nearVal, farVal);

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

    initIndices();
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

    /* Origine disegno */
    glTranslatef(-1.1, 1.1, -1.1);

    // Disegno il cubo
    drawCube();

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
    int tmp;
    switch (key)
    {
    case 27: // ESC
        exit(0);
        break;

    case 'q':

        /* rotazione oraria */
        
        /* aggiornamento angoli */
        tmp = cubeIndices[0];
        cubeIndices[0] = cubeIndices[18];
        cubeIndices[18] = cubeIndices[24];
        cubeIndices[24] = cubeIndices[6];
        cubeIndices[6] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[3];
        cubeIndices[3] = cubeIndices[9];
        cubeIndices[9] = cubeIndices[21];
        cubeIndices[21] = cubeIndices[15];
        cubeIndices[15] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_0[i]], 'q');
        }

        // glutPostRedisplay();

        break;

    case 'Q':
        /* rotazione anti-oraria */

        /* aggiornamento angoli */
        tmp = cubeIndices[0];
        cubeIndices[0] = cubeIndices[6];
        cubeIndices[6] = cubeIndices[24];
        cubeIndices[24] = cubeIndices[18];
        cubeIndices[18] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[3];
        cubeIndices[3] = cubeIndices[15];
        cubeIndices[15] = cubeIndices[21];
        cubeIndices[21] = cubeIndices[9];
        cubeIndices[9] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_0[i]], 'Q');
        }

        // glutPostRedisplay();

        break;

    case 'w':
        /* rotazione oraria */

        /* aggiornamento angoli*/
        tmp = cubeIndices[7];
        cubeIndices[7] = cubeIndices[1];
        cubeIndices[1] = cubeIndices[19];
        cubeIndices[19] = cubeIndices[25];
        cubeIndices[25] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[4];
        cubeIndices[4] = cubeIndices[16];
        cubeIndices[16] = cubeIndices[22];
        cubeIndices[22] = cubeIndices[10];
        cubeIndices[10] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_1[i]], 'w');
        }

        // glutPostRedisplay();

        break;

    case 'W':
        /* rotazione anti-oraria */

        /* aggiornamento angoli*/
        tmp = cubeIndices[7];
        cubeIndices[7] = cubeIndices[25];
        cubeIndices[25] = cubeIndices[19];
        cubeIndices[19] = cubeIndices[1];
        cubeIndices[1] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[4];
        cubeIndices[4] = cubeIndices[10];
        cubeIndices[10] = cubeIndices[22];
        cubeIndices[22] = cubeIndices[16];
        cubeIndices[16] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_1[i]], 'W');
        }

        // glutPostRedisplay();

        break;

    case 'e':
        /* rotazione oraria */

        /* aggiornamento indici */
        /* aggiornamento angoli */
        tmp = cubeIndices[8];
        cubeIndices[8] = cubeIndices[2];
        cubeIndices[2] = cubeIndices[20];
        cubeIndices[20] = cubeIndices[26];
        cubeIndices[26] = tmp; 

        /* aggiornamento quelli di mezzo (Mordor) */
        tmp = cubeIndices[5];
        cubeIndices[5] = cubeIndices[11];
        cubeIndices[11] = cubeIndices[23];
        cubeIndices[23] = cubeIndices[17];
        cubeIndices[17] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_2[i]], 'e');
        }

        // glutPostRedisplay();

        break;

    case 'E':
        /* rotazione oraria */

        /* aggiornamento indici */
        /* aggiornamento angoli */
        tmp = cubeIndices[8];
        cubeIndices[8] = cubeIndices[26];
        cubeIndices[26] = cubeIndices[20];
        cubeIndices[20] = cubeIndices[2];
        cubeIndices[2] = tmp; 

        /* aggiornamento quelli di mezzo (Mordor) */
        tmp = cubeIndices[5];
        cubeIndices[5] = cubeIndices[17];
        cubeIndices[17] = cubeIndices[23];
        cubeIndices[23] = cubeIndices[11];
        cubeIndices[11] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[x_2[i]], 'E');
        }

        // glutPostRedisplay();

        break;

    case 'a':
        /* rotazione oraria */

        /* aggiornamento angoli */
        tmp = cubeIndices[0];
        cubeIndices[0] = cubeIndices[6];
        cubeIndices[6] = cubeIndices[8];
        cubeIndices[8] = cubeIndices[2];
        cubeIndices[2] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[1];
        cubeIndices[1] = cubeIndices[3];
        cubeIndices[3] = cubeIndices[7];
        cubeIndices[7] = cubeIndices[5];
        cubeIndices[5] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_0[i]], 'a');
        }

        // glutPostRedisplay();

        break;

    case 'A':
        /* rotazione anti-oraria */        

        /* aggiornamento angoli */
        tmp = cubeIndices[0];
        cubeIndices[0] = cubeIndices[2];
        cubeIndices[2] = cubeIndices[8];
        cubeIndices[8] = cubeIndices[6];
        cubeIndices[6] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[1];
        cubeIndices[1] = cubeIndices[5];
        cubeIndices[5] = cubeIndices[7];
        cubeIndices[7] = cubeIndices[3];
        cubeIndices[3] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_0[i]], 'A');
        }

        // glutPostRedisplay();

        break;
    
    case 's':
        /* rotazione oraria */        

        /* aggiornamento angoli */
        tmp = cubeIndices[9];
        cubeIndices[9] = cubeIndices[15];
        cubeIndices[15] = cubeIndices[17];
        cubeIndices[17] = cubeIndices[11];
        cubeIndices[11] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[12];
        cubeIndices[12] = cubeIndices[16];
        cubeIndices[16] = cubeIndices[14];
        cubeIndices[14] = cubeIndices[10];
        cubeIndices[10] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_1[i]], 's');
        }

        // glutPostRedisplay();

        break;
    
    case 'S':

        /* rotazione anti-oraria */        

        /* aggiornamento angoli */
        tmp = cubeIndices[9];
        cubeIndices[9] = cubeIndices[11];
        cubeIndices[11] = cubeIndices[17];
        cubeIndices[17] = cubeIndices[15];
        cubeIndices[15] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[12];
        cubeIndices[12] = cubeIndices[10];
        cubeIndices[10] = cubeIndices[14];
        cubeIndices[14] = cubeIndices[16];
        cubeIndices[16] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_1[i]], 'S');
        }

        // glutPostRedisplay();

        break;
    
    case 'd':
        /* rotazione oraria */        

        /* aggiornamento angoli */
        tmp = cubeIndices[18];
        cubeIndices[18] = cubeIndices[24];
        cubeIndices[24] = cubeIndices[26];
        cubeIndices[26] = cubeIndices[20];
        cubeIndices[20] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[21];
        cubeIndices[21] = cubeIndices[25];
        cubeIndices[25] = cubeIndices[23];
        cubeIndices[23] = cubeIndices[19];
        cubeIndices[19] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_2[i]], 'd');
        }

        // glutPostRedisplay();

        break;

    case 'D':
        /* rotazione anti-oraria */        

        /* aggiornamento angoli */
        tmp = cubeIndices[18];
        cubeIndices[18] = cubeIndices[20];
        cubeIndices[20] = cubeIndices[26];
        cubeIndices[26] = cubeIndices[24];
        cubeIndices[24] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[19];
        cubeIndices[19] = cubeIndices[23];
        cubeIndices[23] = cubeIndices[25];
        cubeIndices[25] = cubeIndices[21];
        cubeIndices[21] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[y_2[i]], 'D');
        }

        // glutPostRedisplay();

        break;

    case 'z':
        /* rotazione oraria */
        
        /* Aggiornamento indici */

        /* aggiornamento angoli */
        tmp = cubeIndices[6];
        cubeIndices[6] = cubeIndices[24];
        cubeIndices[24] = cubeIndices[26];
        cubeIndices[26] = cubeIndices[8];
        cubeIndices[8] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[7];
        cubeIndices[7] = cubeIndices[15];
        cubeIndices[15] = cubeIndices[25];
        cubeIndices[25] = cubeIndices[17];
        cubeIndices[17] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_0[i]], 'z');
        }

        // glutPostRedisplay();

        break;

    case 'Z':
        /* Rotazione antioraria */

        /* Aggirnamento indici */
        
        /* Aggirnamento angoli */
        tmp = cubeIndices[6];
        cubeIndices[6] = cubeIndices[8];
        cubeIndices[8] = cubeIndices[26];
        cubeIndices[26] = cubeIndices[24];
        cubeIndices[24] = tmp;

        /* Aggirnamento quelli di mezzo (Mordor) */
        tmp = cubeIndices[7];
        cubeIndices[7] = cubeIndices[17];
        cubeIndices[17] = cubeIndices[25];
        cubeIndices[25] = cubeIndices[15];
        cubeIndices[15] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_0[i]], 'Z');
        }

        break;
    
    case 'x':
        /* rotazione oraria */        

        /* aggiornamento angoli */
        tmp = cubeIndices[3];
        cubeIndices[3] = cubeIndices[21];
        cubeIndices[21] = cubeIndices[23];
        cubeIndices[23] = cubeIndices[5];
        cubeIndices[5] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[4];
        cubeIndices[4] = cubeIndices[12];
        cubeIndices[12] = cubeIndices[22];
        cubeIndices[22] = cubeIndices[14];
        cubeIndices[14] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_1[i]], 'x');
        }

        // glutPostRedisplay();

        break;
    
    case 'X':
        /* rotazione anti-oraria */        

        /* aggiornamento angoli */
        tmp = cubeIndices[3];
        cubeIndices[3] = cubeIndices[5];
        cubeIndices[5] = cubeIndices[23];
        cubeIndices[23] = cubeIndices[21];
        cubeIndices[21] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[4];
        cubeIndices[4] = cubeIndices[14];
        cubeIndices[14] = cubeIndices[22];
        cubeIndices[22] = cubeIndices[12];
        cubeIndices[12] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {            
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_1[i]], 'X');
        }

        // glutPostRedisplay();

        break;
    
    case 'c':
        /* rotazione oraria */        

        /* aggiornamento angoli */
        tmp = cubeIndices[0];
        cubeIndices[0] = cubeIndices[18];
        cubeIndices[18] = cubeIndices[20];
        cubeIndices[20] = cubeIndices[2];
        cubeIndices[2] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[1];
        cubeIndices[1] = cubeIndices[9];
        cubeIndices[9] = cubeIndices[19];
        cubeIndices[19] = cubeIndices[11];
        cubeIndices[11] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_2[i]], 'c');
        }

        // glutPostRedisplay();

        break;
    
    case 'C':
        /* rotazione anti-oraria */        

        /* aggiornamento angoli */
        tmp = cubeIndices[0];
        cubeIndices[0] = cubeIndices[2];
        cubeIndices[2] = cubeIndices[20];
        cubeIndices[20] = cubeIndices[18];
        cubeIndices[18] = tmp;

        /* aggiorniamo quelli di mezzo (Mordor) */
        tmp = cubeIndices[1];
        cubeIndices[1] = cubeIndices[11];
        cubeIndices[11] = cubeIndices[19];
        cubeIndices[19] = cubeIndices[9];
        cubeIndices[9] = tmp;

        for (unsigned int i = 0; i < 9; i++)
        {
            // Aggiornamento ultimo tasto premuto
            addRotation(cubeIndices[z_2[i]], 'C');
        }

        // glutPostRedisplay();

        break;
        
    case 'y':
        Yangle += (90.0 / 360.0) * (2 * M_PI);
        // glutPostRedisplay();
        break;// glutPostRedisplay();

    case 'Y':
        Yangle -= (90.0 / 360.0) * (2 * M_PI);
        // glutPostRedisplay();
        break;

    case 'n':
        if (cubesDistance <= MAX_CUBE_DISTANCE)
        {
            cubesDistance += CUBE_DIST_ADD_STEP;
            // glutPostRedisplay();
        }
        
        break;

    case 'm':
        if (cubesDistance > MIN_CUBE_DISTANCE)
        {
            cubesDistance -= CUBE_DIST_ADD_STEP;
            // glutPostRedisplay();
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
        glPushMatrix();
        for (j = 0; j < 3; j++)
        {
            glPushMatrix();

            for (k = 0; k < 3; k++)
            {
                // Un cubo per fila

                glPushMatrix();
                // disegno un cubo
                for (int rot = vaoInfoStruct[cubeIndices[cubeNo]].rotation.lastRotation; rot >=0 ; rot--)
                {
                    inputKey = vaoInfoStruct[cubeIndices[cubeNo]].rotation.rotHistory[rot];

                    // Rotazione asse X
                    if (inputKey == 'q' || inputKey == 'w' || inputKey == 'e')
                    {
                        glRotatef(CwRotValue, 1.0, 0.0, 0.0);
                    }
                    
                    if (inputKey == 'Q' || inputKey == 'W' || inputKey == 'E')
                    {
                        glRotatef(CcwRotValue, 1.0, 0.0, 0.0);
                    }

                    // Rotazione asse Y
                    if (inputKey == 'a' || inputKey == 's' || inputKey == 'd')
                    {
                        glRotatef(CcwRotValue, 0.0, 1.0, 0.0);
                    }
                    
                    if (inputKey == 'A' || inputKey == 'S' || inputKey == 'D')
                    {
                        glRotatef(CwRotValue, 0.0, 1.0, 0.0);
                    }

                    // Rotazioni asse Z
                    if (inputKey == 'z' || inputKey == 'x' || inputKey == 'c')
                    {
                        glRotatef(CcwRotValue, 0.0, 0.0, 1.0);
                    }
                    
                    if (inputKey == 'Z' || inputKey == 'X' || inputKey == 'C')
                    {
                        glRotatef(CwRotValue, 0.0, 0.0, 1.0);
                    }
                }

                glBindVertexArray(vao[cubeIndices[cubeNo]]);

                for (int faceIndex = 0; faceIndex < N_FACES; faceIndex++)
                {
                    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid *)(faceIndex * N_VERTICES * sizeof(GLuint)));
                }
                // mi sposto a destra
                glPopMatrix();

                glTranslatef(cubesDistance, 0.0, 0.0);

                // Incremento cubeNo
                cubeNo++;
            }
            glPopMatrix();

            glTranslatef(0.0, 0.0, cubesDistance);
        }
        glPopMatrix();

        glTranslatef(0.0, -cubesDistance, 0.0);
    }

    // glutPostRedisplay();
}

void displayLost(){
    glColor3f(1.0, 0.0, 0.0);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, "O");
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
        displayLost();
        exit(EXIT_FAILURE);
    }
}

/**
 * array indici-piano
*/
/* 
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
