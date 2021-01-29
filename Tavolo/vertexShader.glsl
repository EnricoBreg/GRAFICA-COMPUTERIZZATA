#version 430 core

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform uint object;

vec4 coords;

void main(void)
{
   gl_Position = projMat * modelViewMat * coords;
}
