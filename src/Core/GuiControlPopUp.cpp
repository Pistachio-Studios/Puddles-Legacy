#include "Core/GuiControlPopUp.h"
#include "Core/Render.h"
#include "Core/App.h"
#include "Core/Audio.h"
#include "Core/Window.h"

GuiControlPopUp::GuiControlPopUp(uint32 id) : GuiControl(GuiControlType::POPUP, id)
{
	canClick = true;
	drawBasic = false;
}

GuiControlPopUp::~GuiControlPopUp()
{

}

bool GuiControlPopUp::Update(float dt)
{
	app->win->GetWindowSize(windowW, windowH);
	app->render->DrawRectangle({static_cast<int>(windowW/ 2 - 200), static_cast<int>(windowH / 2 - 50), 350,300 }, 192, 103, 252, 240);

	return true;
}

