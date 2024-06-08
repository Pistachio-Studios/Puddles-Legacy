#ifndef __RENDER_H__
#define __RENDER_H__

#include "Core/Module.h"

#include "Utils/Point.h"


#include <SDL.h>
#include <SDL_ttf.h>
#include <limits.h>
#include <vector>


class Entity;

class Sprite {
	public:
		//Constructor
		Sprite();
		Sprite(SDL_Texture* texture, int x, int y, SDL_Rect section, float speed, double angle, float size, uint layer, SDL_RendererFlip flip, int pivotX, int pivotY) : texture(texture), position({x,y}), section(section), speed(speed), angle(angle), size(size), layer(layer), flip(flip), pivotX(pivotX), pivotY(pivotY) {
			
			int w, h;
			if(section.w == 0 && section.h == 0)
				SDL_QueryTexture(texture, NULL, NULL, &w, &h);
			else
				w = section.w, h = section.h;

			center = {x + (w * size) / 2, y + (h * size) / 2};
			pivot = center;

			if(pivotX != INT_MAX)
				pivot.x = pivotX;
			if(pivotY != INT_MAX)
				pivot.y = pivotY;
		}

		//Setters / Getters
		//...//
		//void SetPivotOffset(iPoint offset);
		
	public:
	SDL_Texture* texture;
	iPoint position;
	iPoint center;
	iPoint pivot;
	uint layer;
	
	//some data for the wrapper
	SDL_Rect section;
	float size;
	float speed;
	double angle;
	SDL_RendererFlip flip;
	int pivotX;
	int pivotY;
};

class Render : public Module
{
public:

	Render();

	Render (bool startEnabled);

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	void DrawImGui() override;

	// Called before quitting
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	void cameraInterpolation(Entity* target, float lerpSpeed, float dt, iPoint offset = { 0,0 });

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, float size = 1.0f, uint layer = 1, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivotX = INT_MAX, int pivotY = INT_MAX);
	bool DrawTextureLegacy(SDL_Texture* texture, int x, int y, SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, float size = 1.0f, SDL_RendererFlip flip = SDL_FLIP_NONE, int pivotX = INT_MAX, int pivotY = INT_MAX);
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawText(const char* text, int posX, int posY, int w, int h, SDL_Color color = {255,255,255});

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//
	bool LoadState(pugi::xml_node node);

	//
	bool SaveState(pugi::xml_node node);

	// Set vsync on/off
	void SetVsync(bool vsync);
	bool vsyncEnabled;

private:

	bool DrawSprite(Sprite& sprite) const;

public:

	struct Camera
	{
		int x = 0;
		int y = 0;
		int w;
		int h;

		bool useInterpolation;
		float lerpSpeed;
		Entity* target = nullptr;
		iPoint offset;
	}camera;

	SDL_Renderer* renderer;
	SDL_Rect viewport;
	SDL_Color background;
	TTF_Font* font;

	std::vector <Sprite> sprites;

	SDL_Texture* overlayTarget;

	bool showPivots = false;
};

#endif // __RENDER_H__