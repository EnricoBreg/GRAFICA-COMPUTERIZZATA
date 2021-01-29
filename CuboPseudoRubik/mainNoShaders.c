/*
* Draw a cube ... VBO
*/

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
 * 4 faces to draw
*/
#define N_FACES 4

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;

typedef struct Vertex
{
    float coords[N_VERTICES]; // Coordinate omogenee
    float colors[N_VERTICES]; // RGBA
} Vertex;

/** Vertici per una faccia del quadrato */
static Vertex squareVertices[N_VERTICES * N_FACES] = {

    /** Third face */
    { {-0.5, -0.5, 0.5, 1.0},    {0.0, 1.0, 0.0, 1.0} },
    { {-0.5, 0.5, 0.5, 1.0},     {0.0, 1.0, 0.0, 1.0} },
    { {0.5, -0.5, 0.5, 1.0},     {0.0, 1.0, 0.0, 1.0} },
    { {0.5, 0.5, 0.5, 1.0},      {0.0, 1.0, 0.0, 1.0} },
    
    /** First face */
    { {0.5, -0.5, -0.5, 1.0},    {1.0, 0.0, 0.0, 1.0} },
    { {0.5, 0.5, -0.5, 1.0},     {1.0, 0.0, 0.0, 1.0} },
    { {-0.5, -0.5, -0.5, 1.0},   {1.0, 0.0, 0.0, 1.0} },
    { {-0.5, 0.5, -0.5, 1.0},    {1.0, 0.0, 0.0, 1.0} },

    /** Fourth face */
    { {0.5, -0.5, -0.5, 1.0 },   {1.0, 1.0, 0.0, 1.0} },
    { {0.5, 0.5, -0.5, 1.0 },    {1.0, 1.0, 0.0, 1.0} },
    { {0.5, -0.5, 0.5, 1.0 },    {1.0, 1.0, 0.0, 1.0} },
    { {0.5, 0.5, 0.5, 1.0 },     {1.0, 1.0, 0.0, 1.0} },

    /** Second face */
    { {-0.5, -0.5, 0.5, 1.0},    {0.0, 0.0, 1.0, 1.0} },
    { {-0.5, 0.5, 0.5, 1.0},     {0.0, 0.0, 1.0, 1.0} },
    { {-0.5, -0.5, -0.5, 1.0},   {0.0, 0.0, 1.0, 1.0} },
    { {-0.5, 0.5, -0.5, 1.0},    {0.0, 0.0, 1.0, 1.0} }

};

typedef struct Matrix4x4
{
    float entries[16];
} Matrix4x4;

//static Matrix4x4 modelViewMatrix;
static vec4 modelViewMatrix[4];
//static Matrix4x4 projMatrix;
static vec4 projMatrix[4];

static unsigned int programID, vertexShaderID, fragmentShaderID, modelViewMatLoc,
    projMatLoc, buffer[1], vao[1];

/** Display routine */
void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm_mat4_identity(modelViewMatrix);

    glm_translate(modelViewMatrix, (vec3){0.0, 0.0, -1.0});
    glm_rotate(modelViewMatrix, Xangle, (vec3){1.0, 0.0, 0.0});
    glm_rotate(modelViewMatrix, Yangle, (vec3){0.0, 1.0, 0.0});
    glm_rotate(modelViewMatrix, Zangle, (vec3){0.0, 0.0, 1.0});
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)modelViewMatrix);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, N_VERTICES * N_FACES);
    glFlush();
}

// Function to read text file.
char *readTextFile(char *aTextFile)
{
    FILE *filePointer = fopen(aTextFile, "rb");
    char *content = NULL;
    long numVal = 0;

    // estimate buffer length
    fseek(filePointer, 0L, SEEK_END);
    numVal = ftell(filePointer);

    fseek(filePointer, 0L, SEEK_SET);
    content = (char *)malloc((numVal + 1) * sizeof(char));
    fread(content, 1, numVal, filePointer);
    content[numVal] = '\0';
    fclose(filePointer);
    return content;
}

/** Init Routine */
void init(void)
{

    glClearColor(1.0, 1.0, 1.0, 0.0);

    /** 
     * Creazione e compilazione del Vertex Shader
    */
    char *vertexShader = readTextFile("vertexShader.glsl");
    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderID, 1, (const char **)&vertexShader, NULL);
    glCompileShader(vertexShaderID);

    /** 
     * Creazione e compilazione del Fragment Shader
    */
    char *fragmentShader = readTextFile("fragmentShader.glsl");
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderID, 1, (const char **)&fragmentShader, NULL);
    glCompileShader(fragmentShaderID);

    /**
     * Creato id eseguibile e allegati gli shaders
    */
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    glUseProgram(programID);

    glGenVertexArrays(1, vao);
    glGenBuffers(1, buffer);

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

    /**
     * Specifico come sono costruiti i dati all'interno dell'array dove sono salvati
    */
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(squareVertices[1]), (GLvoid *)sizeof(squareVertices[0].coords));
    glEnableVertexAttribArray(1);

    /**
     * Definizione della matrice di Proiezione
     * Estrazione indirizzo dalla GPU della variabile che conterrà la matrice di proj
     * Caricamento della projMatrix nella GPU nella variabile definita da projMatLoc
    */
    /*Matrix4x4 projMatrix = {
        {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, -2.0, -1.0,
            0.0, 0.0, 0.0, 1.0
        }
    };*/

    projMatLoc = glGetUniformLocation(programID, "projMat");
    glm_frustum(-1.0, 1.0, -1.0, 1.0, 0.2, 10.0, projMatrix);
    glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, (GLfloat *)projMatrix);

    /**
     * Definizione della model View
     * Estrazione indirizzo dalla GPU della variabile che conterrà la matrice modelView
     * Caricamento della modelViewMatrix nella GPU nella variabile definita da modelViewMatLoc
    */
    vec4 modelViewMatrix[4] = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}};
    modelViewMatLoc = glGetUniformLocation(programID, "modelViewMat");
    glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, (GLfloat *)modelViewMatrix);
}

/**
 * Input key processing routine
*/
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'x':
        Xangle += (5.0 / 360.0) * (2.0 * M_PI);
        if (Xangle > (2.0 * M_PI))
            Xangle -= 2.0 * M_PI;
        glutPostRedisplay();
        break;
    case 'X':
        Xangle -= (5.0 / 360.0) * (2.0 * M_PI);
        if (Xangle < 0.0)
            Xangle += 2.0 * M_PI;
        glutPostRedisplay();
        break;
    case 'y':
        Yangle += (5.0 / 360.0) * (2.0 * M_PI);
        if (Yangle > 2.0 * M_PI)
            Yangle -= 2.0 * M_PI;
        glutPostRedisplay();
        break;
    case 'Y':
        Yangle -= (5.0 / 360.0) * (2.0 * M_PI);
        if (Yangle < 0.0)
            Yangle += 2.0 * M_PI;
        glutPostRedisplay();
        break;
    case 'z':
        Zangle += (5.0 / 360.0) * (2.0 * M_PI);
        if (Zangle > 2.0 * M_PI)
            Zangle -= 2.0 * M_PI;
        glutPostRedisplay();
        break;
    case 'Z':
        Zangle -= (5.0 / 360.0) * (2.0 * M_PI);
        if (Zangle < 0.0)
            Zangle += 2.0 * M_PI;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

int main(int argc, char **argv)
{
    /**
     * Prompt
    */
    printf("Interazioni: \n");
    printf("Premi i tasti x, X, y, Y, z, Z per girare la scena.\n");

    glutInit(&argc, argv);

    /**
     * Set della versione della libreria OpenGL
    */
    glutInitContextVersion(4, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

    glEnable(GL_DEPTH_TEST);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(150, 150);
    glutCreateWindow("Prova Cubo Pseudo-Rubik");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);

    glewInit();

    init();

    glutMainLoop();

    return 0;
}

/*
// define vertices
GLfloat vertexArray[NFACES*NVERTICES*3] = {
    // first face - v0,v1,v3,v2
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
    0.5, 0.5, 0.5 // v1
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

// array containing indices
GLubyte elementIndices[NFACES*NVERTICES] =
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
*/