#include "Core/Render.h"
#include "Core/App.h"
#include "Core/Audio.h"
#include "Core/Window.h"
#include "Core/GuiControlSlider.h"
#include "Core/Textures.h"

GuiControlSlider::GuiControlSlider(uint32 id, const char* text, SDL_Rect bounds, int minValue, int maxValue) : GuiControl(GuiControlType::SLIDER, id), minValue(minValue), maxValue(maxValue)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	currentPos = bounds.x;
	currentValue = minValue;

	texture = app->tex->Load("Assets/UI/Slider/slider.png");		// Line from the slider
	textureSelected = app->tex->Load("Assets/UI/Slider/slide.png");	// Slider object that moves
}

GuiControlSlider::~GuiControlSlider()
{

}

bool GuiControlSlider::Update(float dt)
{
	int textureWidth, textureHeight;
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	int textureSelectedWidth, textureSelectedHeight;
	SDL_QueryTexture(textureSelected, NULL, NULL, &textureSelectedWidth, &textureSelectedHeight);

	app->render->DrawText(text.GetString(), bounds.x - 90, bounds.y - 10, 80, 30); // Add offset to text
	if (state != GuiControlState::DISABLED)
	{
		app->input->GetMousePosition(mouseX, mouseY);
		rect2 = { currentPos - textureSelectedWidth / 2 + 550, bounds.y + bounds.h / 2 - textureSelectedHeight / 2 };

		if (mouseX > bounds.x + 550 && mouseX < bounds.x + bounds.w + 550 && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				isPressed = true;
				currentValue = int(((maxValue - minValue) * ((mouseX - (bounds.x + 550)) / float(bounds.w))) + minValue);
				currentPos = mouseX - 550; // Subtract the offset when updating currentPos
				rect2 = { currentPos - textureSelectedWidth / 2 + 550, bounds.y + bounds.h / 2 - textureSelectedHeight / 2 };
				state = GuiControlState::PRESSED;
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				isPressed = false;
				NotifyObserver();
			}
		}
		else {
			isPressed = false;
			state = GuiControlState::NORMAL;
		}

		switch (state)
		{
		case GuiControlState::DISABLED:
			app->render->DrawTexture(texture, bounds.x + bounds.w / 2 - textureWidth / 2 + 550, bounds.y + bounds.h / 2 - textureHeight / 2);
			app->render->DrawTexture(textureSelected, rect2.x, rect2.y);
			break;
		case GuiControlState::NORMAL:
			app->render->DrawTexture(texture, bounds.x + bounds.w / 2 - textureWidth / 2 + 550, bounds.y + bounds.h / 2 - textureHeight / 2);
			app->render->DrawText(std::to_string(currentValue).c_str(), bounds.x + bounds.w + 60 + 550, bounds.y, bounds.w, bounds.h); // Add offset to text
			app->render->DrawTexture(textureSelected, rect2.x, rect2.y);
			break;
		case GuiControlState::PRESSED:
			app->render->DrawTexture(texture, bounds.x + bounds.w / 2 - textureWidth / 2 + 550, bounds.y + bounds.h / 2 - textureHeight / 2);
			app->render->DrawText(std::to_string(currentValue).c_str(), bounds.x + bounds.w + 60 + 550, bounds.y, bounds.w, bounds.h); // Add offset to text
			app->render->DrawTexture(textureSelected, rect2.x, rect2.y);
			break;
		}
	}

	return false;
}

bool GuiControlSlider::Draw()
{
	return false;
}
