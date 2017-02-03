// GLSL version
#version 330 core

// shader inputs
in vec3 fragment_color;
in vec3 world_pos;
in vec3 vec_normal;
in vec3 vec_vertex_camera;
in mat4x3 vec_vertex_light;

// shader output: pixel final color
out vec3 out_Color;

// values constant for whole mesh
uniform mat4 MV;
uniform int nb_lights;
uniform mat4x3 light_matrix;

// main function
void main()
{
	// light emission properties
	vec3 color_power = 3.0 * vec3(1.0, 1.0, 1.0);
	
	// specualar
	vec3 specular_emission = vec3(0.3, 0.3, 0.3) * color_power;

	// diffuse
	vec3 diffuse_emission = fragment_color * color_power;

	// normal of the computed fragment (in camera space)
	vec3 n = normalize( vec_normal );

	// eye vector (towards the camera)
	vec3 E = normalize(vec_vertex_camera);

	// ambient : simulates indirect lighting
	vec3 ambiant_color  = vec3(0.1, 0.1, 0.1) * fragment_color;

	// pixel color: one ambiant light
	out_Color = ambiant_color;

	// loop on all the lights
	for(int i=0; i<nb_lights; i++)
	{
		// direction of the light (from fragment to light)
		vec3 l = normalize( vec_vertex_light[i] );

		// cosine of the angle between the normal and the light direction
		float cosTheta = clamp(dot(n,l), 0.0, 1.0);
		
		// light reflection direction
		vec3 R = reflect(-l,n);

		// cosine of the angle between the eye vector and the reflect vector
		float cosAlpha = clamp(dot(E,R), 0.0 , 1.0);

		// distance to light
		float distance = length( light_matrix[i] - world_pos );

		// pixel color increment
		out_Color += 
			// diffuse : "color" of the object
			diffuse_emission * cosTheta / (distance * distance) +
			// specular : reflective highlight, like a mirror
			specular_emission * pow(cosAlpha,5) / (distance * distance);
	}
}
