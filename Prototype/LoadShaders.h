#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <glad.h>
#include <glfw3.h>

GLuint LoadShaders(const std::string& vertFilePath, const std::string& fragFilePath)
{
	//Create the shaders
	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	//Parse Vertex Shader Code
	std::string		vertShaderCode;
	std::ifstream	vertShaderStream(vertFilePath.c_str(), std::ios::in);

	//If can open the file
	if (vertShaderStream.is_open())
	{
		std::string line;
		while (std::getline(vertShaderStream, line))
		{
			vertShaderCode += "\n" + line;
		}
		vertShaderStream.close();
	}

	//Parse Fragment Shader Code
	std::string		fragShaderCode;
	std::ifstream	fragShaderStream(fragFilePath.c_str(), std::ios::in);

	//If can open the file
	if (fragShaderStream.is_open())
	{
		std::string line;
		while (std::getline(fragShaderStream, line))
		{
			fragShaderCode += "\n" + line;
		}
		fragShaderStream.close();
	}

	//God knows what these do atm
	GLint result = GL_FALSE;
	int infoLogLength;

	//Start compiling Vertex Shader code
	printf("Compiling Vertex Shader ( %s )\n", vertFilePath.c_str());
	char const * vertSourcePtr = vertShaderCode.c_str();
	glShaderSource(vertShaderID, 1, &vertSourcePtr, NULL);
	glCompileShader(vertShaderID);

	glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertShaderID, infoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	//Start compiling Fragment Shader code
	printf("Compiling Vertex Shader ( %s )\n", fragFilePath.c_str());
	char const * fragSourcePtr = fragShaderCode.c_str();
	glShaderSource(fragShaderID, 1, &fragSourcePtr, NULL);
	glCompileShader(fragShaderID);

	glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertShaderID, infoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	//Link Shader to Program
	printf("Linking Program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertShaderID);
	glAttachShader(programID, fragShaderID);
	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength > 0)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(programID, vertShaderID);
	glDetachShader(programID, fragShaderID);

	glDeleteShader(vertShaderID);
	glDeleteShader(fragShaderID);

	return programID;
}