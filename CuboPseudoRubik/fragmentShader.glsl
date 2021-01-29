#version 430 core

smooth in vec4 colorsExport;

layout(depth_less) out float gl_FragDepth;
//out float gl_FragColor;
out vec4 colorsOut;

void main(void) {
   colorsOut = colorsExport;
   gl_FragDepth = gl_FragCoord.z;
}
