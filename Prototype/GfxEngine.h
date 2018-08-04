#ifndef _GFX_ENGINE_H_
#define _GFX_ENGINE_H_ 

#include <unordered_map>
#include <string>

class Drawable;

class GfxEngine
{
private:
	void initialise();

	std::unordered_map<std::string, Drawable* const> m_mGfx;

public:
	 GfxEngine();
	~GfxEngine();

	void renderAll();

	void addGfx		(const std::string& id, Drawable* const gameObject);
	void removeGfx	(const std::string& id);
};

#endif // !

