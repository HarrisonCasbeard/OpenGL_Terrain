#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	Normal = mat3(transpose(inverse(model))) * vertexNormal;
    FragPos = vec3(model * vec4(vertexPosition, 1.0));
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
}
