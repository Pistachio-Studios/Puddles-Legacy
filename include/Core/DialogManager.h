#ifndef DIALOG_MANAGER_H
#define DIALOG_MANAGER_H

#include "Core/Module.h"
#include "Utils/List.h"
#include "Utils/Timer.h"
#include <SDL_ttf.h>
#include <map>
#include <string>
#include <vector>
#include <Utils/Point.h>

using namespace std;

enum DialogType {
    DIALOG,
    CHOICE
};

struct Dialog {
    int id;
    string character;
    string ES;
    string EN;
    string CAT;
    DialogType type;
    vector<int> choices;
};

class DialogManager : public Module {
public:

    DialogManager();

    DialogManager(bool startEnabled);

    // Destructor
    virtual ~DialogManager();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool PreUpdate();

    // Called each loop iteration
    bool Update(float dt);

    // Called each loop iteration
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

    // Load / Save
    bool LoadState(pugi::xml_node&);
    bool SaveState(pugi::xml_node&) const;

    // Dialog methods
    void StartDialog(int dialogId);
    void NextDialog();
    void EndDialog();
    void ShowDialog(int x = 0, int y = 0);
    int ChoiceSelector(vector<int> choices);

    SDL_Texture* CreateTextTexture(TTF_Font* font, const char* text, SDL_Color color, int textBoundWidth);

    bool LoadDialogs(string, map<int, Dialog>& dialogs);

private:
    map<int, Dialog> dialogs;   // Map of dialog ID to Dialog data
    Dialog* currentDialog;
    int currentDialogId;
    string currentDialogLine;

    // Font
    TTF_Font* font;
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color
    SDL_Color selectedColor = { 255, 0, 255, 255 };
    SDL_Rect textRect;

    int indexText;
    Timer charTimer;

    int charTimeMS = 100;

    DialogType currentDialogType;

    SDL_Texture* background = nullptr;
    map<string, SDL_Texture*> characterTextures;
    
    pugi::xml_node parameters;
};

#endif // DIALOG_MANAGER_H