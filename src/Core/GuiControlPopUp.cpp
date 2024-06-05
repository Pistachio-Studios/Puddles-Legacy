#include "Core/GuiControlPopUp.h"
#include "Core/Render.h"
#include "Core/App.h"
#include "Core/Audio.h"
#include "Core/Window.h"
#include "Core/Textures.h"

GuiControlPopUp::GuiControlPopUp(uint32 id) : GuiControl(GuiControlType::POPUP, id)
{
	canClick = true;
	drawBasic = false;

	texture = app->tex->Load("Assets/UI/PopUps/settingsBackground.png");
}

GuiControlPopUp::~GuiControlPopUp()
{

}

bool GuiControlPopUp::Update(float dt)
{
	app->win->GetWindowSize(windowW, windowH);
	//app->render->DrawRectangle({static_cast<int>(windowW/ 2 - 200), static_cast<int>(windowH / 2 - 50), 350,300 }, 192, 103, 252, 240);
	int textureWidth = 1440; // TODO ver como sacar esto dinamicamente de la textura o foto
	int textureHeight =  952;
	int posX = static_cast<int>(windowW / 2 - textureWidth / 2);
	int posY = static_cast<int>(windowH / 2 - textureHeight / 2);
	app->render->DrawTexture(texture, posX, posY);

	return true;
}

