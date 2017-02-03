// GLSL version
#version 330 core

// maximal number of lights
#define MAX_LIGHTS 10

// actual number of lights
uniform int nbLights;

// light strucutre
uniform struct ShaderLight
{
	vec4 pos;
	vec3 color;
	vec3 attenuation;
	float ambientCoef;

	// cone related
	float coneCosAngle;
	vec3 coneDir;

} sh_lights[MAX_LIGHTS];

// camera position
uniform vec3 camPos;

// transparency
uniform float transparency;

// bright appearance (for specular light)
uniform float shiny;
uniform vec3 specColor;

// fragment shader inputs from the vertex shader
in vec3 in_Normal_frag;
in vec3 in_Vertex_frag;
in vec3 in_Color_frag;

// fragment shader output
out vec4 finalColor;

// compute the color due to one light
vec3 light_compute(ShaderLight cur_light, vec3 vertex_color, vec3 normal, vec3 vertex_pos, vec3 cam_vertex)
{
	float attenuation;
	vec3 cur_light_dir;

	if(cur_light.pos.w == 0.0)
	{
		//directional light
		cur_light_dir = cur_light.pos.xyz;
		attenuation = 1.0; //no attenuation for directional lights
	}
	else
	{
		//point light
		cur_light_dir = normalize(cur_light.pos.xyz - vertex_pos);
		attenuation = 1.0 / (cur_light.attenuation.x + cur_light.attenuation.z * pow(length(cur_light.pos.xyz - vertex_pos), 2));

		//cone restrictions
		if(dot(-cur_light_dir, cur_light.coneDir) < cur_light.coneCosAngle)
		{
			attenuation = 0.0;
		}
	}

	//ambient
	vec3 ambient = cur_light.ambientCoef * vertex_color.rgb * cur_light.color;

	//diffuse
	float diffuseCoefficient = max(0.0, dot(normal, cur_light_dir));
	vec3 diffuse = diffuseCoefficient * vertex_color.rgb * cur_light.color;
	
	//specular
	float specularCoefficient = 0.0;

	if(diffuseCoefficient > 0.0)
	{
		specularCoefficient = pow(max(0.0, dot(cam_vertex, reflect(-cur_light_dir, normal))), shiny);
	}
	vec3 specular = specularCoefficient * specColor * cur_light.color;

	// linear color (color before gamma correction)
	return ambient + attenuation * (diffuse + specular);
}

// main function
void main()
{
	// normal in world space
	vec3 normal = normalize(in_Normal_frag);

	// color
	vec4 vertex_color = vec4(in_Color_frag, transparency);

	// vector from vertex to camera
	vec3 cam_vertex = normalize(camPos - in_Vertex_frag);

	// sum of all the colors due to each light
	vec3 color_sum = vec3(0.0);

	// loop on all the light
	for(int i = 0; i<nbLights; i++)
	{
		color_sum += light_compute(sh_lights[i], vertex_color.rgb, normal, in_Vertex_frag, cam_vertex);
	}
	
	//final color (after gamma correction)
	vec3 gamma = vec3(1.0 / 2.2);
	finalColor = vec4(pow(color_sum, gamma), vertex_color.a);
}
