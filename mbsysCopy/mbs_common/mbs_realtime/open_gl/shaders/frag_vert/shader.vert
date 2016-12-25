// GLSL version
#version 330 core

// vertex shader inputs, location is to assign related buffer
layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec3 in_Normal;

// vertex shader outputs, interpolation will be applied for each fragment
out vec3 in_Vertex_frag;
out vec3 in_Color_frag;
out vec3 in_Normal_frag;

// matrices similar for the whole mesh
uniform mat4 MVP;
uniform mat4 M;
uniform mat3 M_inv_trans;

// main function
void main()
{
	// final vertex position
	gl_Position = MVP * vec4(in_Vertex, 1.0);

	// vertex in the world space
	in_Vertex_frag = vec3(M * vec4(in_Vertex, 1.0));

	// normal in world space
	in_Normal_frag = M_inv_trans * in_Normal;

	// send colors without transformation
	in_Color_frag = in_Color;
}
