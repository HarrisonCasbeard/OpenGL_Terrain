#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <glm\glm.hpp>

class Shader;
class Drawable
{
protected:
	bool	m_is3D = true;
	Shader* m_pShader = nullptr;

public:
	virtual ~Drawable() {};

	virtual void render	() = 0;
			bool is3D	() const { return m_is3D; }
};

#endif
