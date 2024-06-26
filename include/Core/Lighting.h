#ifndef __LIGHTING_H__
#define __LIGHTING_H__

#include "Core/Module.h"
#include "Utils/List.h"
#include "Utils/Point.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>

class Light 
{
	public:
	void Draw();
	public: //cambiar a privado en algun momento
	bool active;
	iPoint position;
	float radius = 1.0f;
	SDL_Color color;
	SDL_Texture* texture;
};

class Lighting : public Module
{
public:

	Lighting();

	Lighting (bool startEnabled);

	// Destructor
	virtual ~Lighting();

	// Called before Lighting is available
	bool Awake(pugi::xml_node& conf) override;

	// Called before the first frame
	bool Start() override;

	// Called each loop iteration
	bool PreUpdate() override;
	bool Update(float dt) override;
	bool PostUpdate() override;

	void DrawImGui() override;

	// Called before quitting
	bool CleanUp() override;

	void SetAmbientLight(const SDL_Color& color);

	Light* AddLight(iPoint position, float radius = 1.0f, const SDL_Color& color = { 255, 255, 255, 255 });

	void RemoveLight(Light* light);

	void RemoveAllLights();

private:
	List<Light*> lights;
	SDL_Color ambientLight;
	SDL_Texture* lightTexture;
};

#endif // __LIGHTING_H__