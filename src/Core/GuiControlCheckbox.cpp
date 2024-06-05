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
	app->render->DrawText(text.GetString(), bounds.x - 90, bounds.y - 10, 80, 30);
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
			//app->render->DrawRectangle({bounds.x + 550, bounds.y, bounds.w, bounds.h}, 255, 255, 255, 255, true, false);
			app->render->DrawTexture(texture, bounds.x + 550, bounds.y);
			break;
		case GuiControlState::NORMAL:
		case GuiControlState::PRESSED:
			//app->render->DrawRectangle({bounds.x + 550, bounds.y, bounds.w, bounds.h}, 255, 255, 255, 255, true, false);
			app->render->DrawTexture(texture, bounds.x + 550, bounds.y);
			if (checked) {
				// Draw a checkmark or some other indicator when the checkbox is checked
				//app->render->DrawRectangle({bounds.x + 555, bounds.y + 5, bounds.w - 10, bounds.h - 10}, 0, 0, 0, 255, true, false);
				app->render->DrawTexture(textureSelected, bounds.x + 550, bounds.y);
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