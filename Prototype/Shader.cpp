#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(const GLchar* vertPath, const GLchar* fragPath)
{
	//These are going to contain the GLSL code
	std::string vertCode;
	std::string fragCode;

	//Parses the files given and puts the code into the variables above
	parseShader(vertPath, vertCode);
	parseShader(fragPath, fragCode);

	//Create and compiler the shaders
	GLuint vertID, fragID;
	compileShader(vertID, GL_VERTEX_SHADER, vertCode);
	compileShader(fragID, GL_FRAGMENT_SHADER, fragCode);

	//Link the shaders to the program
	linkShader(vertID, fragID);

	//Can now delete the shaders as they're linked to the program
	glDeleteShader(vertID);
	glDeleteShader(fragID);
}

Shader::~Shader()
{
}

//-------------------------------------------------------------------------------------------------
void Shader::parseShader(const char* path, std::string& code)
{
	//Open a file
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	//Try and get the code from the file
	try
	{
		file.open(path);

		std::stringstream stream;
		stream << file.rdbuf();

		file.close();

		code = stream.str();
	}
	//If it errors, output why
	catch (std::ifstream::failure e)
	{
		printf("FAILED TO READ SHADER : %s \n", path);
	}
}
//-------------------------------------------------------------------------------------------------
void Shader::compileShader(GLuint& fileID, const GLenum shaderType, const std::string& shaderCode)
{
	int success;
	const unsigned int logLength = 512;
	char infoLog[logLength];

	//Create and compile the shader
	fileID = glCreateShader(shaderType);
	const char* code = shaderCode.c_str();
	glShaderSource(fileID, 1, &code, NULL);
	glCompileShader(fileID);

	//If any errors, output them
	glGetShaderiv(fileID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fileID, logLength, NULL, infoLog);
		printf("FAILED TO COMPILE SHADER : %s \n", infoLog);
	}
}
//-------------------------------------------------------------------------------------------------
void Shader::linkShader(const GLuint vertID, const GLuint fragID)
{
	int success;
	const unsigned int logLength = 512;
	char infoLog[logLength];

	//Create a unifying program
	m_shaderID = glCreateProgram();
	//Attach shaders to unifier
	glAttachShader(m_shaderID, vertID);
	glAttachShader(m_shaderID, fragID);
	glLinkProgram(m_shaderID);

	//If any errors in the linking, output them
	glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderID, logLength, NULL, infoLog);
		printf("ERROR LINKING SHADER PROGRAM : %s \n", infoLog);
	}
}
//-------------------------------------------------------------------------------------------------
void Shader::activate()
{
	glUseProgram(m_shaderID);
}
//-------------------------------------------------------------------------------------------------
void Shader::setBool(const std::string& uniformName, const bool value)
{
	glUniform1i(glGetUniformLocation(m_shaderID, uniformName.c_str()), (int)value);
}
//-------------------------------------------------------------------------------------------------
void Shader::setInt(const std::string& uniformName, const int value)
{
	glUniform1i(glGetUniformLocation(m_shaderID, uniformName.c_str()), value);
}
//-------------------------------------------------------------------------------------------------
void Shader::setFloat(const std::string& uniformName, const float value)
{
	glUniform1f(glGetUniformLocation(m_shaderID, uniformName.c_str()), value);
}
//-------------------------------------------------------------------------------------------------
void Shader::setVec3(const std::string& uniformName, const glm::vec3& _vec3)
{
	glUniform3fv(glGetUniformLocation(m_shaderID, uniformName.c_str()), 1, &_vec3[0]);
}
//-------------------------------------------------------------------------------------------------
void Shader::setMatrixf(const std::string& uniformName, const float* _floats)
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderID, uniformName.c_str()), 1, GL_FALSE, _floats);
}