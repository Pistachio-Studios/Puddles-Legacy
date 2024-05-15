#include "Core/Render.h"
#include "Core/App.h"
#include "Core/Audio.h"
#include "Core/Window.h"
#include "Core/GuiControlDropDownBox.h"
#include "Core/GuiControlButton.h"

GuiControlDropDownBox::GuiControlDropDownBox(uint32 id, const char* text, SDL_Rect bounds) : GuiControl(GuiControlType::DROPDOWNBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
	isOpen = false;
	
}

GuiControlDropDownBox::~GuiControlDropDownBox()
{

}

bool GuiControlDropDownBox::Update(float dt)
{
	app->render->DrawText(text.GetString(), bounds.x - 90, bounds.y - 10, 80, 30);
	if (state != GuiControlState::DISABLED)
	{
		app->input->GetMousePosition(mouseX, mouseY);

		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				isOpen = !isOpen; // Toggle the open state when the dropdown box is clicked
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
			if (isOpen) {
				// Update and draw the buttons when the dropdown box is open
				for (GuiControlButton& option : options) {
					option.Update(dt);
				}
			}
			break;
			bool Draw();
		}
	}

	return false;
}

bool GuiControlDropDownBox::Draw()
{
	// Draw the dropdown box
	app->render->DrawRectangle(bounds, 255, 255, 255, 255, true, false);
	app->render->DrawText(text.GetString(), bounds.x, bounds.y, 80, 30);

	if (isOpen) {
		// Draw the options when the dropdown box is open
		for (GuiControlButton& option : options) {
			app->render->DrawRectangle(option.bounds, 255, 255, 255, 255, true, false);
			app->render->DrawText(option.text.GetString(), option.bounds.x, option.bounds.y, 80, 30);
		}
	}

	return false;
}

void GuiControlDropDownBox::AddOption(const std::string& optionText)
{
    // Create a new GuiControlButton with the given text
    GuiControlButton newOption(id + options.size(), {bounds.x, bounds.y + (options.size() + 1) * 30, bounds.w, 30}, optionText.c_str());

    // Add the new option to the list
    options.push_back(newOption);
}

int GuiControlDropDownBox::GetSelectedOption() const
{
	// Iterate the options list to find the selected option
	for (const GuiControlButton& option : options)
	{
		if (option.state == GuiControlState::PRESSED)
		{
			return option.id;
		}
	}

	return -1;
}