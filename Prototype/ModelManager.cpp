#include "ModelManager.h"

ModelManager::ModelManager()
{
	m_mModels =
	{
		{ "spaceship",	Model("Assets/Models/spaceship/spaceship.obj")},
		{ "cube",		Model("Assets/Models/defaultShapes/cube.obj") },
		{ "meteor",		Model("Assets/Models/defaultShapes/comet.obj")},
	};

	m_defModel = Model("Assets/Models/defaultShapes/cube.obj");
}

ModelManager::~ModelManager()
{
}

//-------------------------------------------------------------------------------------------------
const Model& ModelManager::getModel(const std::string& id) const
{
	auto itr = m_mModels.find(id);

	if (itr != m_mModels.end())
		return itr->second;

	printf("WARNING: %s MODEL NOT FOUND! USING DEFAULT\n", id.c_str());
	return m_defModel;
}