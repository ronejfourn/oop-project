#pragma once
#include "graphics.h"
#include "player.h"
#include <string>

enum class GameState {MENU, OPTIONS, PAUSE, ALIVE, DEAD};

const int defaultSize = 18;
const int defaultWidth = 14;
const float fontRatio = 14 / 18.0f;

class UI{
    static UI *_instance;
private:
    uint8_t _selectedOptionIndex;
    SDL_Texture *_fontTexture;
    GameState _currentState;
    Vec2i _charCoords[95];
    Vec2i _charDim;
    UI();
public:
    static UI *GetInstance();
    ~UI();

    void SetState(GameState state);
    void LoadFont(Graphics *g);
    GameState GetCurrentState();

    void Pause();
    void ChangeOption(bool up);
    void ChooseOption(bool *restart);

    void DisplayText(Graphics *g, std::string msg, SDL_Rect dst, uint16_t ftSize = defaultSize);
    void DisplayInfo(Graphics *g, Player &p);
    void DrawMenu(Graphics *g);
    void DrawOptions(Graphics *g);
    void DrawPauseMenu(Graphics *g);
    void Draw(Graphics *g, Player &p);
};