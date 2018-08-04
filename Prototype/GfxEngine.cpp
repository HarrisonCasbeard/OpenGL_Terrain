#include "GfxEngine.h"
#include "Drawable.h"

#include <glew\glew.h>

GfxEngine::GfxEngine()
{
	initialise();
}

GfxEngine::~GfxEngine()
{
}

//-------------------------------------------------------------------------------------------------
void GfxEngine::initialise()
{
	//----- Z Buffer Setup -----
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//----- Stencil Buffer -----
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}
//-------------------------------------------------------------------------------------------------
void GfxEngine::renderAll()
{
	for (auto& gfx : m_mGfx)
	{
		gfx.second->render();
	}
}
//-------------------------------------------------------------------------------------------------
void GfxEngine::addGfx(const std::string& id, Drawable* const gameObject)
{
	auto itr = m_mGfx.insert({ id, gameObject });
	if (!itr.second)
	{
		printf("%s already exists in map so cannot add again!", id.c_str());
	}
}
//-------------------------------------------------------------------------------------------------
void GfxEngine::removeGfx(const std::string& id)
{
	m_mGfx.erase(id);
}