#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include <unordered_map>
#include <glm\glm.hpp>

class Shader;
class Light;

class ShaderManager
{
private:
	std::unordered_map<std::string, Shader*> m_mShaders;

public:
	ShaderManager();
	~ShaderManager();

	void initialiseShaders(const unsigned int screenW, const unsigned int screenH, const glm::mat4& projection);

	void updateShaders(const glm::mat4& viewMatrix, const glm::vec3& camPos, const Light& light);

	Shader* const getShader(const std::string& id) const;
};

#endif
