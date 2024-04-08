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
    LoadDialogs("bin/dialogs.csv", dialogs);

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

bool DialogManager::LoadDialogs(string path, map<int, Dialog>& dialogs)
{
    bool ret = true;

    // TODO load dialog csv
    rapidcsv::Document doc;

    try {
        rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));
    } catch (const std::ios_base::failure& e) {
        std::cerr << "Failed to open or read file at " << path << ": " << e.what() << std::endl;
        // handle error or exit
        ret = false;
    }

    vector<int> ids = doc.GetColumn<int>("id");
    vector<string> characters = doc.GetColumn<string>("character");
    vector<string> ESs = doc.GetColumn<string>("ES");
    vector<string> ENs = doc.GetColumn<string>("EN");
    vector<string> CATs = doc.GetColumn<string>("CAT");
    vector<string> types = doc.GetColumn<string>("type");
    vector<string> choices = doc.GetColumn<string>("choices");

    for (size_t i = 0; i < ids.size(); i++) {
        Dialog dialog;
        dialog.id = ids[i];
        dialog.character = characters[i];
        dialog.ES = ESs[i];
        dialog.EN = ENs[i];
        dialog.CAT = CATs[i];
        //dialog.type = StringToDialogType(types[i]);

        /*
        stringstream choicesStream(choices[i]);
        string choice;
        while (getline(choicesStream, choice, ',')) {
            dialog.choices.push_back(stoi(choice));
        }
        */

        dialogs[dialog.id] = dialog;
    }

    return ret;
}
