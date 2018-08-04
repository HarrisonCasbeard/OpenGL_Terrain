#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

uniform float maxHeight;

out VS_OUT
{
	vec3 fragPos;
	vec2 UV;
	vec3 normal;
	vec4 fragPosLightSpace;

	float yPos;
	float fragMaxHeight;
} vsOut;

void main()
{
	/*gl_Position = projection * view * model * vec4(vertexPosition_modelspace, 1.0);
	UV = vertexUV;
	normal = vertexNormal;*/

	vsOut.fragPos = vec3(model * vec4(vertexPosition_modelspace, 1.0f));
	vsOut.normal = transpose(inverse(mat3(model))) * vertexNormal;
	vsOut.UV = vertexUV;
	vsOut.fragPosLightSpace = lightSpaceMatrix * vec4(vsOut.fragPos, 1.0f);

	vsOut.yPos = vertexPosition_modelspace.y;
	vsOut.fragMaxHeight = maxHeight;

	gl_Position = projection * view * model * vec4(vertexPosition_modelspace, 1.0f);
}