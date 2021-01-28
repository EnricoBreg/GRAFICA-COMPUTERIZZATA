
/*
* Draw a cube ... VAO
*/

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>

// number of faces
#define NFACES      4
// number of vertices per face
#define NVERTICES   4

// switch between the different methods
// Uncomment ONLY one of the following #define
// #define ARRAYELEMENT
#define DRAWELEMENTS

// Array of buffer ids.
unsigned int buffers[2];

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

// indices of the vertices within each strip
// In this simple case, the integers from 0 to NFACES*NVERTICES-1
GLint vertexIndices[NFACES*NVERTICES];


// info for the lid of the cube

// Array of buffer ids.
unsigned int lidBuffers[2];

GLfloat lidVertexArray[3*3] = {
    // Vertices of the lid
    -0.5, 0.5, 0.5, // v2
    0.5, 0.5, -0.5, // v5
    -0.5, 0.5, -0.5 // v6
};
GLfloat lidColorArray[3*3] = {
    // Colors of the vertices of the lid
    0.5, 0.7, 0.3, // v2
    0.5, 0.7, 0.3, // v5
    0.5, 0.7, 0.3 // v6
};

// indices of the vertices within each strip
// In this simple case, the integers from 0 to NFACES*NVERTICES-1
GLint lidVertexIndices[3];

// --------------------------------------------
// IDs of two Vertex Array Objects
unsigned int vao[2];


// display function
void display(void)
{
    GLint indFace, indVertex;

    // clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // store modelview settings
    glPushMatrix();

    // place the cube in the scene
    glTranslatef(0.0, 0.0, -1.5);
    glTranslatef(0.0, -.8, 0.0);
    glRotatef(30.0, 0.0, 1.0, 0.0);
    glRotatef(50.0, 1.0, 0.0, 0.0);

    // bind the vertex arrat object for the cube
    glBindVertexArray(vao[0]);
    #ifdef ARRAYELEMENT
    // here we draw the triangle strips
    for (indFace=0; indFace<NFACES; indFace++) {
        glBegin(GL_TRIANGLE_STRIP);
        for(indVertex=0; indVertex<NVERTICES; indVertex++) {
            // draw vertex and color
            glArrayElement(indFace*NFACES + indVertex);
        };
        glEnd();
    };
    #endif

    #ifdef DRAWELEMENTS
    // here we draw the triangle strips
    for (indFace=0; indFace<NFACES; indFace++) {
        // glDrawArrays(GL_TRIANGLE_STRIP, indFace*NVERTICES, 4);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, indFace*NVERTICES * sizeof(GLuint));
    };
    #endif

    // bind the vertex arrat object for the lid
    glBindVertexArray(vao[1]);
    // now draw lid
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    // restore model view settings
    glPopMatrix();

    // flush graphics objects immediately
    glFinish();
}


void init (void)
{
    int currInd;
    GLenum glErr;

    // select clearing color
    glClearColor (1.0, 1.0, 1.0, 0.0);

    // initialize viewing values
    glMatrixMode(GL_PROJECTION);

    // set viewing frustum
    glFrustum(-0.2, 0.2, -0.3, 0.1, 0.1, 5.0);

    // ... it does not hurt to check that everything went OK
    if ((glErr=glGetError()) != 0) {
        printf("Errore = %d \n", glErr);
        exit(-1);
    }

    // Turn on wireframe mode (only for debugging purposes)
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // initialize model view transforms
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // IMPORTANT: initialize the depth buffer
    // otherwise things are displayed erratically!
    glClear(GL_DEPTH_BUFFER_BIT);
    glDepthFunc(GL_LESS);

    glDisable(GL_CULL_FACE);

    // generate ids for two VAOs
    glGenVertexArrays(2, vao);

    // ---- BEGIN bind VAO id vao[0] to the cube data
    glBindVertexArray(vao[0]);

    // Enable two vertex arrays: coordinates and colors
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // Generate buffer ids.
    glGenBuffers(2, buffers);

    // buffers[0] is for both vertices and colors
    // bind vertex buffer and reserve space.
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray) + sizeof(colorArray), NULL, GL_STATIC_DRAW);

    // Copy vertex coordinates data into first half of vertex buffer.
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexArray), vertexArray);
    // Copy vertex color data into second half of vertex buffer.
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertexArray), sizeof(colorArray), colorArray);

    // buffers[1] is for indices
    // bind and fill indices buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    // initialize vertex indices
    for(currInd=0; currInd < NFACES*NVERTICES; currInd++) vertexIndices[currInd]=currInd;
    // ... and copy it
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndices), vertexIndices, GL_STATIC_DRAW);

    // Specify vertex and color pointers to the start of the respective data.
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(vertexArray)));
    // ---- END bind VAO id vao[0] to the cube data


    // ---- BEGIN bind VAO id vao[1] to the lid
    glBindVertexArray(vao[1]);

    // Generate buffer ids.
    glGenBuffers(2, lidBuffers);

    // Enable two vertex arrays: coordinates and colors
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    // buffers[0] is for both vertices and colors
    // bind vertex buffer and reserve space.
    glBindBuffer(GL_ARRAY_BUFFER, lidBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lidVertexArray) + sizeof(lidColorArray), NULL, GL_STATIC_DRAW);

    // Copy vertex coordinates data into first half of vertex buffer.
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(lidVertexArray), lidVertexArray);
    // Copy vertex color data into second half of vertex buffer.
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(lidVertexArray), sizeof(lidColorArray), lidColorArray);

    // buffers[1] is for indices
    // bind and fill indices buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lidBuffers[1]);
    // initialize vertex indices
    for(currInd=0; currInd < 3; currInd++) lidVertexIndices[currInd]=currInd;
    // ... and copy it
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lidVertexIndices), lidVertexIndices, GL_STATIC_DRAW);


    // Specify locations for the co-ordinates and color arrays.
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glColorPointer(3, GL_FLOAT, 0, (GLvoid*)(sizeof(lidVertexArray)));
    // ---- END bind VAO id vao[1] to the lid
}

// Window size and mode
int main(int argc, char** argv)
{
    // pass potential input arguments to glutInit
    glutInit(&argc, argv);

    // set display mode
    // GLUT_SINGLE = single buffer window
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize (400, 400);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("My first window");

    // Here we add support for GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        printf("GLEW init failed: %s\n", glewGetErrorString(err));
        exit(1);
    } else {
        printf("GLEW init success\n");
    };

    // Call initialization routinesx
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;   /* ANSI C requires main to return int. */
}