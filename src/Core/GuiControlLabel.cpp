#include "Core/GuiControlLabel.h"
#include "Core/Render.h"
#include "Core/App.h"
#include "Core/Audio.h"
#include "Core/Window.h"

#include "Core/Map.h"
#include "Core/Animation.h"
#include "Utils/Easings.h"

GuiControlLabel::GuiControlLabel(uint32_t id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
}

GuiControlLabel::~GuiControlLabel()
{

}

bool GuiControlLabel::Update(float dt)
{
	Easings easings = Easings();
	float controlEase = easings.easeInQuad(controlEaseTimer.ReadMSec() / 500);

	//L15: DONE 4: Draw the button according the GuiControl State
	/* switch (state)
	{
	case GuiControlState::DISABLED:
		app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		break;
	case GuiControlState::FOCUSED:
		app->render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		app->render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		break;
	} */


	if(texture != nullptr)app->render->DrawTexture(texture, bounds.x * controlEase,bounds.y * controlEase, &section, 0.0f);
	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);

	return false;
}

