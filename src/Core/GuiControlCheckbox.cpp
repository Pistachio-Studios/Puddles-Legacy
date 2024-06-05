#include "Core/Render.h"
#include "Core/App.h"
#include "Core/Audio.h"
#include "Core/Window.h"
#include "Core/GuiControlCheckbox.h"
#include "Core/Textures.h"

GuiControlCheckbox::GuiControlCheckbox(uint32 id, const char* text, SDL_Rect bounds) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
	checked = false;

	texture = app->tex->Load("Assets/UI/Checkbox/normalBox.png");
	textureSelected = app->tex->Load("Assets/UI/Checkbox/selectedBox.png");
}

GuiControlCheckbox::~GuiControlCheckbox()
{

}

bool GuiControlCheckbox::Update(float dt)
{
	int textureWidth, textureHeight;
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	int textureSelectedWidth, textureSelectedHeight;
	SDL_QueryTexture(textureSelected, NULL, NULL, &textureSelectedWidth, &textureSelectedHeight);

	app->render->DrawText(text.GetString(), bounds.x - 90, bounds.y - 10, 80, 30); // Keep text at original position
	if (state != GuiControlState::DISABLED)
	{
		app->input->GetMousePosition(mouseX, mouseY);

		if (mouseX > (bounds.x + 550) && mouseX < (bounds.x + 550) + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				checked = !checked; // Toggle the checked state when the checkbox is clicked
				NotifyObserver();
			}
		}

		switch (state)
		{
		case GuiControlState::DISABLED:
			app->render->DrawTexture(texture, bounds.x + bounds.w / 2 - textureWidth / 2 + 550, bounds.y + bounds.h / 2 - textureHeight / 2);
			break;
		case GuiControlState::NORMAL:
		case GuiControlState::PRESSED:
			app->render->DrawTexture(texture, bounds.x + bounds.w / 2 - textureWidth / 2 + 550, bounds.y + bounds.h / 2 - textureHeight / 2);
			if (checked) {
				app->render->DrawTexture(textureSelected, bounds.x + bounds.w / 2 - textureSelectedWidth / 2 + 550, bounds.y + bounds.h / 2 - textureSelectedHeight / 2);
			}
			break;
		}
	}

	return false;
}

bool GuiControlCheckbox::Draw()
{
	return false;
}