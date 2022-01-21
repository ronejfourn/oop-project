#pragma once
#include "graphics.h"
#include "player.h"
#include <string>

typedef enum state {STATE_MENU, STATE_ALIVE, STATE_DEAD} State;

class ui{
    static ui *_instance;
private:
    SDL_Texture *fontTexture;
    State currentState;
    ui();
public:
    static ui *GetInstance();
    ~ui();

    void SetState(State state);
    void LoadFont(Graphics *g);
    State GetCurrentState();

    SDL_Rect GetCharCoord(const char ch);
    void DisplayText(Graphics *g, std::string msg, SDL_Rect dst);
    void DisplayInfo(Graphics *g, Player &p);
    void Draw(Graphics *g, Player &p);
};