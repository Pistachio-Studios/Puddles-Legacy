#include "Core/Render.h"
#include "Core/App.h"
#include "Core/Audio.h"
#include "Core/Window.h"
#include "Core/GuiControlCheckbox.h"

GuiControlCheckbox::GuiControlCheckbox(uint32 id, const char* text, SDL_Rect bounds) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
	checked = false;
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

		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				checked = !checked; // Toggle the checked state when the checkbox is clicked
				NotifyObserver();
			}
		}

		switch (state)
		{
		case GuiControlState::DISABLED:
			app->render->DrawRectangle(bounds, 255, 255, 255, 255, true, false);
			break;
		case GuiControlState::NORMAL:
		case GuiControlState::PRESSED:
			app->render->DrawRectangle(bounds, 255, 255, 255, 255, true, false);
			if (checked) {
				// Draw a checkmark or some other indicator when the checkbox is checked
				app->render->DrawRectangle({bounds.x + 5, bounds.y + 5, bounds.w - 10, bounds.h - 10}, 0, 0, 0, 255, true, false);
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