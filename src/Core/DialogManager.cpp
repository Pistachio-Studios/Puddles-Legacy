#include "DialogManager.h"
#include "rapidcsv.h"

DialogManager::DialogManager() {
    // Constructor implementation
    name.Create("dialogManager");
}

DialogManager::DialogManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("dialogManager");
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
        LOG("Failed to load dialog data from CSV file");
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
    // Convert dialogId to string
    std::string dialogIdStr = std::to_string(dialogId);

    // Check if the dialog exists
    if (dialogs.find(dialogIdStr) != dialogs.end()) {
        // Start the dialog
        currentDialogId = dialogIdStr;
    }
}

void DialogManager::NextDialog() {
    // Get the current dialog
    Dialog& dialog = dialogs[currentDialogId];

    // If the dialog has choices, let the user select a choice
    if (dialog.type == "choose") {
        // Let the user select a choice (this is just an example, replace with your own code)
        int choiceIndex = GetUserChoice();

        // Get the next dialog ID from the selected choice
        std::string nextDialogId = dialog.choices[choiceIndex].second;

        // Start the next dialog
        StartDialog(std::stoi(nextDialogId));
    }
}

void DialogManager::EndDialog() {
    // End the current dialog
    currentDialogId = "";
}