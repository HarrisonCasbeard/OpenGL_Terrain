#include "ShaderManager.h"
#include "Shader.h"
#include "Light.h"

#include <glm\gtc\matrix_transform.hpp>

ShaderManager::ShaderManager()
{
	m_mShaders.emplace("basic",			new Shader("BasicVertexShader.glsl",		"BasicFragmentShader.glsl"));
	m_mShaders.emplace("lighting",		new Shader("LightingVertexShader.glsl",		"LightingFragmentShader.glsl"));
	m_mShaders.emplace("framebuffer",	new Shader("screenVertexShader.glsl",		"screenFragmentShader.glsl"));
	m_mShaders.emplace("skybox",		new Shader("SkyboxVertexShader.glsl",		"SkyboxFragmentShader.glsl"));
	m_mShaders.emplace("reflective",	new Shader("ReflectiveVertexShader.glsl",	"ReflectiveFragmentShader.glsl"));
	m_mShaders.emplace("sprite",		new Shader("SpriteVertexShader.glsl",		"SpriteFragmentShader.glsl"));
	m_mShaders.emplace("text",			new Shader("TextVertexShader.glsl",			"TextFragmentShader.glsl"));
	m_mShaders.emplace("terrain",		new Shader("TerrainVertexShader.glsl",		"TerrainFragmentShader.glsl"));
	m_mShaders.emplace("shadow",		new Shader("SimpleShadowVertexShader.glsl",	"SimpleShadowFragmentShader.glsl"));
	m_mShaders.emplace("water",			new Shader("WaterVertexShader.glsl",		"WaterFragmentShader.glsl"));
}

ShaderManager::~ShaderManager()
{
	for (auto& shader : m_mShaders)
	{
		delete shader.second;
		shader.second = nullptr;
	}
}
//-------------------------------------------------------------------------------------------------
void ShaderManager::updateShaders(const glm::mat4& viewMatrix, const glm::vec3& camPos, const Light& light)
{
	Shader* tmpShader;

	tmpShader = m_mShaders["lighting"];
	tmpShader->activate();
	tmpShader->setMatrixf("view", &viewMatrix[0][0]);
	tmpShader->setVec3("viewPos", camPos);

	tmpShader = m_mShaders["basic"];
	tmpShader->activate();
	tmpShader->setMatrixf("view", &viewMatrix[0][0]);

	tmpShader = m_mShaders["terrain"];
	tmpShader->activate();
	tmpShader->setMatrixf("view", &viewMatrix[0][0]);
	tmpShader->setMatrixf("lightSpaceMatrix", &light.spaceMatrix()[0][0]);
	tmpShader->setVec3("lightPos", light.position());

	tmpShader = m_mShaders["water"];
	tmpShader->activate();
	tmpShader->setMatrixf("view", &viewMatrix[0][0]);

	tmpShader = m_mShaders["shadow"];
	tmpShader->activate();
	tmpShader->setMatrixf("lightSpaceMatrix", &light.spaceMatrix()[0][0]);
}
//-------------------------------------------------------------------------------------------------
void ShaderManager::initialiseShaders(const unsigned int screenW, const unsigned int screenH, const glm::mat4& projection)
{
	Shader* tmpShader;

	//***** Sprite Shader *****
	tmpShader = m_mShaders["sprite"];
	glm::mat4 spriteProjection = glm::ortho(0.0f, (GLfloat)screenW, (GLfloat)screenH, 0.0f, -1.0f, 1.0f);
	tmpShader->activate();
	tmpShader->setInt("image", 0);
	tmpShader->setMatrixf("projection", &spriteProjection[0][0]);

	//***** Basic Shader *****
	tmpShader = m_mShaders["basic"];
	tmpShader->activate();
	tmpShader->setMatrixf("projection", &projection[0][0]);

	//***** Terrain Shader *****
	tmpShader = m_mShaders["terrain"];
	tmpShader->activate();
	tmpShader->setMatrixf("projection", &projection[0][0]);
	tmpShader->setInt("grassTexture", 0);
	tmpShader->setInt("rockTexture", 1);
	tmpShader->setInt("snowTexture", 2);
	tmpShader->setInt("shadowMap", 3);

	//***** Reflective Shader *****
	tmpShader = m_mShaders["reflective"];
	tmpShader->activate();
	tmpShader->setMatrixf("projection", &projection[0][0]);

	//***** Water Shader *****
	tmpShader = m_mShaders["water"];
	tmpShader->activate();
	tmpShader->setMatrixf("projection", &projection[0][0]);

	//***** Lighting Shader *****
	tmpShader = m_mShaders["lighting"];
	tmpShader->activate();
	tmpShader->setMatrixf("projection", &projection[0][0]);

	tmpShader->setInt("material.diffuse", 0);
	tmpShader->setVec3("material.specular", { 0.5f, 0.5f, 0.5f });
	tmpShader->setFloat("material.shininess", 32.0f);

	tmpShader->setVec3("directionalLight.position", { -4.0f, 4.0f, 4.0f });
	tmpShader->setVec3("directionalLight.ambient", { 0.1f, 0.1f, 0.1f });
	tmpShader->setVec3("directionalLight.diffuse", { 0.5f, 0.5f, 0.5f });
	tmpShader->setVec3("directionalLight.specular", { 1.0f, 1.0f, 1.0f });

	tmpShader->setVec3("spotLight.position", { 0.0f, 16.0f, 0.0f });
	tmpShader->setVec3("spotLight.direction", { 0.0f, -1.0f, 0.0f });
	tmpShader->setFloat("spotLight.cutoff", glm::cos(glm::radians(30.0f)));

	tmpShader->setVec3("spotLight.ambient", { 0.1f, 0.1f, 0.1f });
	tmpShader->setVec3("spotLight.diffuse", { 0.5f, 0.5f, 0.5f });
	tmpShader->setVec3("spotLight.specular", { 1.0f, 1.0f, 1.0f });

	tmpShader->setFloat("spotLight.constant", 1.0f);
	tmpShader->setFloat("spotLight.linear", 0.09f);
	tmpShader->setFloat("spotLight.quadratic", 0.032f);

	std::string pointLight = "pointLights[";
	for (unsigned int i = 0; i < 4; i++)
	{
		tmpShader->setFloat(pointLight + std::to_string(i) + "]" + ".constant", 1.0f);
		tmpShader->setFloat(pointLight + std::to_string(i) + "]" + ".linear", 0.09f);
		tmpShader->setFloat(pointLight + std::to_string(i) + "]" + ".quadratic", 0.032f);

		tmpShader->setVec3(pointLight + std::to_string(i) + "]" + ".ambient", { 0.1f, 0.1f, 0.1f });
		tmpShader->setVec3(pointLight + std::to_string(i) + "]" + ".diffuse", { 0.5f, 0.5f, 0.5f });
		tmpShader->setVec3(pointLight + std::to_string(i) + "]" + ".specular", { 1.0f, 1.0f, 1.0f });
	}

	tmpShader->setVec3("pointLights[0].position", { 0.0f,  3.0f, -2.0f });
	tmpShader->setVec3("pointLights[1].position", { 0.0f,  3.0f,  2.0f });
	tmpShader->setVec3("pointLights[2].position", { 0.0f, 10.0f, -2.0f });
	tmpShader->setVec3("pointLights[3].position", { 0.0f, 10.0f,  2.0f });
}
//-------------------------------------------------------------------------------------------------
Shader* const ShaderManager::getShader(const std::string& id) const
{
	auto itr = m_mShaders.find(id);

	if (itr != m_mShaders.end())
		return itr->second;
	else
		printf("COULD NOT FIND SHADER %s !!!", id.c_str());

	return m_mShaders.begin()->second;
}