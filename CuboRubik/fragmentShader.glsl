#version 430 core

smooth in vec4 colorsExport;

<<<<<<< HEAD
layout(depth_less) out float gl_FragDepth;
//out float gl_FragColor;
=======
>>>>>>> 7d3847fcc1be659f5bd3edc6e264e1b4fa388b94
out vec4 colorsOut;

void main(void) {
   colorsOut = colorsExport;
<<<<<<< HEAD
   gl_FragDepth = gl_FragCoord.z;
=======
>>>>>>> 7d3847fcc1be659f5bd3edc6e264e1b4fa388b94
}
