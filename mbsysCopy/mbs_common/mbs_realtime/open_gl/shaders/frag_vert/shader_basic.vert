// GLSL version
#version 330 core

// vertex shader inputs, location is to assign related buffer
layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec3 in_Color;

// vertex shader output, interpolation will be applied for each fragment
out vec3 in_Color_frag;

// matrices similar for the whole mesh
uniform mat4 MVP;

// main function
void main()
{
	// final vertex position
	gl_Position = MVP * vec4(in_Vertex, 1.0);

	// send colors without transformation
	in_Color_frag = in_Color;
}
