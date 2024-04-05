#include "Core/App.h"
#include "Core/DialogManager.h"
#include "rapidcsv.h"
#include <Utils/Log.h>

DialogManager::DialogManager() {
    name.Create("dialog_manager");
}

DialogManager::DialogManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("dialog_manager");
}

DialogManager::~DialogManager() {
    // Destructor implementation
}

bool DialogManager::Awake(pugi::xml_node config) {

    LOG("Loading Dialog Manager");
    bool ret = true;

    // Load dialog data from CSV file using rapidcsv
    LoadDialogs("dialogs.csv", dialogs);

    // Check if the dialogs were loaded successfully
    if (dialogs.empty()) {
        // Handle the case when the file failed to load
        LOG("Failed to load dialog data from dialogs.csv file");
        ret = false;
    }

    return ret;
}

bool DialogManager::Start() {
    // Initialization code
    return true;
}

bool DialogManager::PreUpdate() {
    // Pre-update code
    return true;
}

bool DialogManager::Update(float dt) {
    // Update code
    return true;
}

bool DialogManager::PostUpdate() {
    // Post-update code
    return true;
}

bool DialogManager::CleanUp() {
    // Cleanup code
    return true;
}

bool DialogManager::LoadState(pugi::xml_node&) {
    // Load state code
    return true;
}

bool DialogManager::SaveState(pugi::xml_node&) const {
    // Save state code
    return true;
}

void DialogManager::StartDialog(int dialogId) {

}

void DialogManager::NextDialog() {

}

void DialogManager::EndDialog() {

}

bool DialogManager::LoadDialogs(string path, map<int, string> dialogs)
{
    bool ret = true;

    // TODO load dialog csv
    rapidcsv::Document doc(path);

    return ret;
}
