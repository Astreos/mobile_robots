// GLSL version
#version 330 core

// shader input: 3 vertices for 3D, location is to assign related buffer
layout(location = 0) in vec3 in_Vertex;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec3 in_Normals;

// shader output ; color will be interpolated for each fragment.
out vec3 fragment_color;
out vec3 world_pos;
out vec3 vec_normal;
out vec3 vec_vertex_camera;
out mat4x3 vec_vertex_light;

// multiplication for the whole mesh
uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform int nb_lights;
uniform mat4x3 light_matrix;

// main function
void main()
{
	// final vertex position
	gl_Position = MVP * vec4(in_Vertex, 1.0);

	// color: nothing done here
	fragment_color = in_Color;
	
	// vertex position in worldspace
	world_pos = (M * vec4(in_Vertex, 1.0)).xyz;
	
	// vector from vertex to camera (in camera space)
	vec_vertex_camera = vec3(0.0, 0.0, 0.0) - (V * M * vec4(in_Vertex,1.0)).xyz;
	
	// vertex normal (in camera space)
	vec_normal = ( V * transpose(M) * vec4(in_Normals, 0.0)).xyz;

	// loop on all the lights
	for(int i=0; i<nb_lights; i++)
	{
		// vector that from vertex to light (in camera space)
		vec_vertex_light[i] = (V * vec4(light_matrix[i], 1.0)).xyz + vec_vertex_camera;
	}
}
