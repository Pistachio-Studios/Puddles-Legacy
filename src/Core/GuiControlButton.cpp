#include "Core/GuiControlButton.h"
#include "Core/Render.h"
#include "Core/App.h"
#include "Core/Audio.h"
#include "Core/Window.h"
#include "Core/Textures.h"

GuiControlButton::GuiControlButton(uint32_t id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	textureSelectedLeft = app->tex->Load("Assets/UI/Buttons/selectedLeft.png");
	textureSelectedRight = app->tex->Load("Assets/UI/Buttons/selectedRight.png");
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);
		mouseX *= app->win->GetScale();
		mouseY *= app->win->GetScale();

		//If the position of the mouse if inside the bounds of the button 
		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
		
			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}
			
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

		//L15: DONE 4: Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:
			app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
			break;
		case GuiControlState::NORMAL:
			// app->render->DrawRectangle(bounds, 160, 160, 160, 255, true, false);
			break;
		case GuiControlState::FOCUSED:
			// TODO fix bounds positions to fit the text size
			app->render->DrawTexture(textureSelectedLeft, bounds.x - 10, bounds.y);
			app->render->DrawTexture(textureSelectedRight, bounds.x + bounds.w - 10, bounds.y);
			break;
		case GuiControlState::PRESSED:
			app->render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
			break;
		}

		app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);

	}

	return false;
}

