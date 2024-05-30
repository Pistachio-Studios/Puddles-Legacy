#include "Core/QuestManager.h"
#include "Core/App.h"
#include "Core/Textures.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Core/DebugUI.h"

#include "Utils/Defs.h"
#include "Utils/Log.h"
#include <tracy/Tracy.hpp>
#include <rapidcsv.h>

#include <imgui.h>

QuestManager::QuestManager() : Module()
{
	name.Create("questmanager");
}

QuestManager::QuestManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("questmanager");
}

// Destructor
QuestManager::~QuestManager()
{}

// Called before render is available
bool QuestManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	std::string questsPath = config.child("quests").attribute("path").as_string();

	//Load Quests from CSV file
	rapidcsv::Document doc(questsPath, rapidcsv::LabelParams(0, -1));

	// Iterate over all rows and add a Quest to the quest list
	for (size_t i = 0; i < doc.GetRowCount(); i++) {
		std::string questTitle = doc.GetCell<std::string>("title", i);
		std::string questDescription = doc.GetCell<std::string>("description", i);

		// Create a new Quest object and add it to the quest list
		Quest* quest = new Quest(questTitle.c_str(), questDescription.c_str());

		quests[i] = quest;
	}

	return ret;
}

bool QuestManager::Start() {

	bool ret = true; 

	return ret;
}

// Called before quitting
bool QuestManager::CleanUp()
{
	bool ret = true;
	return ret;
}

bool QuestManager::Update(float dt)
{
	// OPTICK PROFILIN
	ZoneScoped;

	int activeQuests = 0;
	//Draw the active quests
	for (auto& quest : quests) {
		if (quest.second->IsActive()) {
			quest.second->Update();
			DrawQuest(quest.second, 25, 25 + 50 * activeQuests);
			activeQuests++;
		}
	}

	bool ret = true;

	return ret;
}

void QuestManager::DrawImGui()
{
	ImGui::Begin("Quests");
	for (auto& quest : quests) {
		ImGui::Text("Quest: %s", quest.second->GetTitle().GetString());
		ImGui::Text("Description: %s", quest.second->GetDescription().GetString());
		ImGui::Text("Completed: %s", quest.second->IsCompleted() ? "true" : "false");
		ImGui::Text("Active: %s", quest.second->IsActive() ? "true" : "false");
		ImGui::Separator();
	}
	ImGui::End();
}

Quest *QuestManager::GetQuestById(int id)
{
    return quests[id];
}
bool QuestManager::SaveState(pugi::xml_node node)
{
    bool ret = true;

	return ret;
}

void QuestManager::DrawQuest(Quest *quest, int x, int y)
{
	// Draw the quest background
	app->render->DrawRectangle({x - 5, y - 5, 500, 45},25, 50, 50, 255, true, false);

	//Draw the quest completion status
	app->render->DrawRectangle({x - 5, y - 5, quest->GetCompletionValue() * 5, 45},128, 128, 50, 255, true, false);

	// Draw the quest title
	app->render->DrawText(quest->GetTitle().GetString(), x, y, 20, 20, { 255, 0, 0, 255 });

	// Draw the quest description
	app->render->DrawText(quest->GetDescription().GetString(), x, y + 20, 20, 20);

}

bool QuestManager::LoadState(pugi::xml_node node)
{
	bool ret = true;

	return ret;
}