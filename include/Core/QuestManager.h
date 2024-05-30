#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "Core/Module.h"
#include "Gameplay/Entities/Entity.h"
#include "Utils/List.h"
#include <unordered_map>
#include <functional>

class Quest
{
public:

	Quest() : title(""), description(""), completed(false), active(false) {}
	Quest(const char* name, const char* description) : title(name), description(description), completed(false), active(false) {}

	void Update() {
		if (completionAction != nullptr) {
			if (completionAction()) {
				SetCompleted(true);
			}
		}
	}

	SString GetTitle() const { return title; }
	SString GetDescription() const { return description; }
	int GetCompletionValue() const { return completionValue; }
	bool IsCompleted() const { return completed; }
	bool IsActive() const { return active; }
	void SetActive(bool active) { if(!completed)this->active = active; }
	void SetCompleted(bool completed) { this->completed = completed; this->active = false;}
	void SetCompletionAction(std::function<bool()> action) { completionAction = action; }
	void AddCompletionValue(int value) { completionValue += value; }

private:
	SString title;
	SString description;
	bool completed;
	bool active;
	int completionValue = 0;

	std::function<bool()> completionAction;
};

class QuestManager : public Module
{
public:

	QuestManager();

	QuestManager(bool startEnabled);

	// Destructor
	virtual ~QuestManager();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void DrawImGui() override;

	Quest* GetQuestById(int id);

	// Called when we want to load data from XML
	bool LoadState(pugi::xml_node node);

	// Called when we want to save data from XML
	bool SaveState(pugi::xml_node node);

private:

	void DrawQuest(Quest* quest, int x, int y);

private:

	std::unordered_map<int, Quest*> quests;

};

#endif // __QUESTMANAGER_H__
