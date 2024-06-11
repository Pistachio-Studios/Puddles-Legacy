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
	
	//QUESTS SAVE
	for (const auto& pair : quests) {
		pugi::xml_node questsAttributes = node.append_child("quests");

		questsAttributes.append_attribute("id") = pair.first; 
		questsAttributes.append_attribute("title") = pair.second->GetTitle().GetString(); 
		questsAttributes.append_attribute("description") = pair.second->GetDescription().GetString(); 
		questsAttributes.append_attribute("completed") = pair.second->IsCompleted();
		questsAttributes.append_attribute("active") = pair.second->IsActive();
	}

	return ret;
}
bool QuestManager::LoadState(pugi::xml_node node)
{
	bool ret = true;

	//QUESTS LOAD
	for (pugi::xml_node questNode = node.child("quest"); questNode; questNode = questNode.next_sibling("quest")) {
		int id = questNode.attribute("id").as_int();
		std::string title = questNode.attribute("title").as_string();
		std::string description = questNode.attribute("description").as_string();
		bool completed = questNode.attribute("completed").as_bool();
		bool active = questNode.attribute("active").as_bool();

		Quest* quest = new Quest(title.c_str(), description.c_str());
		quest->SetCompleted(completed);
		quest->SetActive(active);

		quests[id] = quest;
	}

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
