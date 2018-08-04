#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include <string>
#include <vector>
#include <glew\glew.h>
#include <glm\glm.hpp>

class Shader;
class Skybox
{
private:
	void loadBuffers();
	void loadTextures(const std::vector<std::string>& filepaths);

	GLuint m_textureID;
	GLuint m_VAO;
	GLuint m_VBO;

	Shader* m_pShader;

public:
	 Skybox();
	~Skybox();

	void initialise(const std::vector<std::string>& filepaths, Shader& shader);
	void render(const glm::mat4& view, const glm::mat4& projection);

};

#endif
