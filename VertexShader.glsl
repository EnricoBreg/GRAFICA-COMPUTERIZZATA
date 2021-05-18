#version 430 core

layout(location=0) in vec4 cubeCoords;
layout(location=1) in vec4 cubeColors;

uniform mat4 projMat;
uniform mat4 modelViewMat;

smooth out vec4 colorsExport;

void main(void) {
    gl_Position = projMat * modelViewMat * cubeCoords;
    colorsExport = cubeColors;
}
