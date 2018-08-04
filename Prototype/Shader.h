#ifndef _SHADER_H_
#define _SHADER_H_

#include <glew\glew.h>
#include <string>
#include <glm\vec3.hpp>

class Shader
{
private:
	void parseShader	(const char* path, std::string& shaderCode);
	void compileShader	(GLuint& fileID, const GLenum shaderType, const std::string& shaderCode);
	void linkShader		(const GLuint vertID, const GLuint fragID);

public:
	unsigned int m_shaderID;

	 Shader(const GLchar* vertPath, const GLchar* fragPath);
	~Shader();


	void activate();

	void setBool	(const std::string& uniformName, const bool _bool);
	void setFloat	(const std::string& uniformName, const float _float);
	void setInt		(const std::string& uniformName, const int _int);
	void setVec3	(const std::string& uniformName, const glm::vec3& _vec);
	void setMatrixf (const std::string& uniformName, const float* _floats);
};

#endif
