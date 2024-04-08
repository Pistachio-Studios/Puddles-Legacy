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

bool DialogManager::Awake(pugi::xml_node& config) {

    LOG("Loading Dialog Manager");
    bool ret = true;

    // Load dialog data from CSV file using rapidcsv
    LoadDialogs("dialogs.csv", dialogs);

    /*
    // TODO remove this debug code
    for (const auto& pair : dialogs) {
        const Dialog& dialog = pair.second;
        std::cout << "Dialog ID: " << dialog.id << std::endl;
        std::cout << "Character: " << dialog.character << std::endl;
        std::cout << "ES: " << dialog.ES << std::endl;
        std::cout << "EN: " << dialog.EN << std::endl;
        std::cout << "CAT: " << dialog.CAT << std::endl;
        std::cout << "Type: " << (dialog.type == DialogType::DIALOG ? "DIALOG" : "CHOICE") << std::endl;
        std::cout << "Choices: ";
        std::cout << std::endl << "-------------------" << std::endl;
    }
    */

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

bool DialogManager::LoadDialogs(string path, map<int, Dialog>& dialogs)
{
    bool ret = true;

    try {
        rapidcsv::Document doc(path, rapidcsv::LabelParams(0, -1));

        vector<int> ids = doc.GetColumn<int>("id");
        vector<string> characters = doc.GetColumn<string>("character");
        vector<string> ESs = doc.GetColumn<string>("ES");
        vector<string> ENs = doc.GetColumn<string>("EN");
        vector<string> CATs = doc.GetColumn<string>("CAT");
        vector<string> types = doc.GetColumn<string>("type");
        vector<string> choices = doc.GetColumn<string>("choices");

        for (size_t i = 0; i < characters.size(); i++) {
            Dialog dialog;
            dialog.id = ids[i];
            dialog.character = characters[i];
            dialog.ES = ESs[i];
            dialog.EN = ENs[i];
            dialog.CAT = CATs[i];

            // Check the type and parse it to the DialogType enum
            if (types[i] == "D") {
                dialog.type = DialogType::DIALOG;
            } else if (types[i] == "C") {
                dialog.type = DialogType::CHOICE;
            } else {
                std::cerr << "Invalid dialog type: " << types[i] << std::endl;
                continue;
            }

            stringstream choicesStream(choices[i]);
            string choice;
            while (getline(choicesStream, choice, ',')) {
                dialog.choices.Add(stoi(choice));
            }

            dialogs[dialog.id] = dialog;
        }
    } catch (const std::ios_base::failure& e) {
        std::cerr << "Failed to open or read file at " << path << ": " << e.what() << std::endl;
        ret = false;
    }

    return ret;
}