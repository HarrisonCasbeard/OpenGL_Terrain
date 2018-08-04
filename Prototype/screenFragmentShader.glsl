#version 330 core
out vec4 colour;
  
in vec2 UV;

uniform sampler2D screenTexture;

void main()
{ 
    colour = texture(screenTexture, UV);
	//vec3 inverse = 1.0f - texture(screenTexture, UV).rgb;
	//float avg = (colour.r + colour.g + colour.b) / 3.0f;

	//colour = vec4(avg, avg, avg, 1.0f);
}