// GLSL version
#version 330 core

// fragment shader input from the vertex shader
in vec3 in_Color_frag;

// fragment shader output
out vec4 finalColor;

// main function
void main()
{
	//final color
	finalColor = vec4(in_Color_frag, 1.0);
}
