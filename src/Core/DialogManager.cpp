#include "Core/App.h"
#include "Core/DialogManager.h"
#include "rapidcsv.h"
#include <Utils/Log.h>
#include "Core/Render.h"

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

    // Load the font for rendering the dialog text
    font = TTF_OpenFont("arial.ttf", 24);

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
    StartDialog(1);
    ShowDialog();
    return true;
}

bool DialogManager::PostUpdate() {
    // Post-update code
    return true;
}

bool DialogManager::CleanUp() {
    // Cleanup code
    dialogs.clear();
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
    currentDialogId = dialogId;
    // Set the current dialog text to the first line of the dialog
    currentDialogLine = dialogs.at(dialogId).ES;
}

void DialogManager::NextDialog() {
    // Get the current dialog
    Dialog& dialog = dialogs.at(currentDialogId);
    // Advance to the next line or choice in the dialog
    // Set the current dialog text to the next line or choice
    if (dialog.type == DialogType::DIALOG) {
        // If the dialog is a dialog, advance to the next line
        // If there are no more lines, end the dialog
        // Otherwise, set the current dialog text to the next line
    } else if (dialog.type == DialogType::CHOICE) {
        // If the dialog is a choice, advance to the next choice
        // If there are no more choices, end the dialog
        // Otherwise, set the current dialog text to the next choice
    }
}

void DialogManager::EndDialog() {
    // Clear the current dialog
    currentDialogId = -1;
    currentDialogLine.clear();
}

void DialogManager::ShowDialog() {
    if (currentDialogId != -1) {
        // Render the current dialog text on the screen
        // You'll need to replace this with your actual rendering code
        SDL_Surface* surface = TTF_RenderText_Solid(font, currentDialogLine.c_str(), textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);
        SDL_RenderCopy(app->render->renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
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