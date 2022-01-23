#pragma once
#include "graphics.h"
#include "player.h"
#include <string>

enum class GameState {MENU, OPTIONS, ALIVE, DEAD};

const int defaultSize = 18;
const int defaultWidth = 14;
const float fontRatio = 14 / 18.0f;

class UI{
    static UI *_instance;
private:
    uint8_t _selectedOptionIndex;
    SDL_Texture *_fontTexture;
    GameState _currentState;
    UI();
public:
    static UI *GetInstance();
    ~UI();

    void SetState(GameState state);
    void LoadFont(Graphics *g);
    GameState GetCurrentState();

    void ChangeOption(bool up);
    void ChooseOption();

    SDL_Rect GetCharCoord(const char ch);
    void DisplayText(Graphics *g, std::string msg, SDL_Rect dst, uint16_t ftSize = defaultSize);
    void DisplayInfo(Graphics *g, Player &p);
    void DrawMenu(Graphics *g);
    void DrawOptions(Graphics *g);
    void Draw(Graphics *g, Player &p);
};