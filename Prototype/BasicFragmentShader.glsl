#version 330 core

in vec2 UV;
in vec3 Normal;

//Output data
out vec4 colour;

//Values that stay constant for the entire mesh
uniform sampler2D textureSampler;

void main()
{
	colour = texture( textureSampler, UV);
	//colour = vec3(1.0f, 0.0f, 0.0f);
}