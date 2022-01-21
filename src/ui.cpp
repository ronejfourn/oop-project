#include "headers/ui.h"
#include "headers/logger.h"
#include <SDL2/SDL.h>

extern SDL_Texture *singleTexture;

UI *UI::_instance = nullptr;

std::string menuItems[3] = {"Play", "Options", "Exit"};
std::string optionsItems[] = {"Back"};

UI::UI(){
    _currentState = STATE_MENU;
}

UI *UI::GetInstance(){
    if(!_instance) _instance = new UI;
    return _instance;
}

UI::~UI(){}

void UI::LoadFont(Graphics *g) {
    _fontTexture = g->LoadImage("../../res/font.png");
}

State UI::GetCurrentState(){
    return _currentState;
}

void UI::SetState(State state){
    _currentState = state;
    if(_currentState == STATE_MENU)
        _selectedOptionIndex = 0;
    if(_currentState == STATE_OPTIONS)
        _selectedOptionIndex = 0;
}

void UI::ChangeOption(bool up){
    uint8_t max;
    if(_currentState == STATE_MENU)
        max = 2;
    else if(_currentState == STATE_OPTIONS)
        max = 1;
    if(up)
        _selectedOptionIndex = _selectedOptionIndex ? _selectedOptionIndex - 1 : max;
    else
        _selectedOptionIndex = (_selectedOptionIndex == max) ? 0 : _selectedOptionIndex + 1;
}

void UI::ChooseOption(){
    if(_currentState == STATE_MENU){
        if(_selectedOptionIndex == 0)
            SetState(STATE_ALIVE);
        else if(_selectedOptionIndex == 1)
            SetState(STATE_OPTIONS);
        else if(_selectedOptionIndex == 2){
            SDL_Event e;
            e.type = SDL_QUIT;
            SDL_PushEvent(&e);
        }
    }
    else if(_currentState == STATE_OPTIONS){
        if(_selectedOptionIndex == 0)
            SetState(STATE_MENU);
    }
}

SDL_Rect UI::GetCharCoord(const char ch){
    SDL_Rect tmp = {0, 0, 14, 18};         // dimension of each character 14*18
    tmp.x = ((ch - 32) % 18) * tmp.w;
    tmp.y = ((ch - 32) / 18) * tmp.h;

    return tmp;
}

void UI::DisplayText(Graphics *g, std::string msg, SDL_Rect dst, uint16_t ftSize){
    const char *msgStr = msg.c_str();
    uint16_t txtSize = msg.size();

    SDL_Rect chrDst;
    chrDst.h = dst.h < ftSize ? dst.h : ftSize;
    chrDst.w = fontRatio * chrDst.h;

    if((dst.h / chrDst.h) * (dst.w / float(chrDst.w)) < txtSize){
        Logger::LogWarning(("Following message does not fit in the box. Characters will be squished: " + msg).c_str());
        chrDst.w = ((dst.h / chrDst.h) * dst.w) / msg.size();
    }

    chrDst.x = dst.x;
    chrDst.y = dst.y;

    int i = 0;
    if((txtSize - i) * chrDst.w < dst.w)
        chrDst.x += (dst.w - ((txtSize - i) * chrDst.w)) / 2;
    for(; i < txtSize; i++){
        if(chrDst.x >= dst.x + dst.w){
            chrDst.y += chrDst.h;
            chrDst.x = dst.x;
            if((txtSize - i) * chrDst.w < dst.w)
                chrDst.x += (dst.w - ((txtSize - i) * chrDst.w)) / 2;
        }
        SDL_Rect src = GetCharCoord(msgStr[i]);
        g->DrawTexture(_fontTexture, src, chrDst);
        chrDst.x += chrDst.w;
    }
}

void UI::DisplayInfo(Graphics *g, Player &p){
    float hp = p.GetHp();
    SDL_Rect src = {0, 256, 16, 16};
    src.x = 288 * (hp >= 75) + 304 * (hp < 75 && hp >=25) + 320 * (hp < 25);

    SDL_Rect healthDst = {0, 0, int(src.w * 1.5), int(src.h * 1.5)};
    g->DrawTexture(singleTexture, src, healthDst);

    healthDst.x += healthDst.w;
    healthDst.w = 120;
    DisplayText(g, "HP:" + std::to_string(int(hp)), healthDst, healthDst.h );
}

void UI::DrawMenu(Graphics *g){
    Vec2i windowSize = g->GetCurrentResolution();
    SDL_Rect menuBox = {0, 0, windowSize.x / 4, windowSize.y / 4};
    menuBox.x = (windowSize.x - menuBox.w) / 2;
    menuBox.y = (windowSize.y - menuBox.h) / 2;
    menuBox.h /= 3;

    uint16_t ftSize = 35;
    for(const auto &i : menuItems){
        DisplayText(g, i, menuBox, i == menuItems[_selectedOptionIndex] ? 40 : ftSize);
        menuBox.y += menuBox.h;
    }
}

void UI::DrawOptions(Graphics *g){
    Vec2i windowSize = g->GetCurrentResolution();
    SDL_Rect optionsBox = {0, 0, windowSize.x / 4, windowSize.y / 4};
    optionsBox.x = (windowSize.x - optionsBox.w) / 2;
    optionsBox.y = (windowSize.y - optionsBox.h) / 2;
    optionsBox.h /= 3;

    uint16_t ftSize = 28;
    for(const auto &i : optionsItems){
        DisplayText(g, i, optionsBox, i == menuItems[_selectedOptionIndex] ? 35 : ftSize);
        optionsBox.y += optionsBox.h;
    }
}

void UI::Draw(Graphics *g, Player &p){
    if(_currentState == STATE_ALIVE)
        DisplayInfo(g, p);
    else if(_currentState == STATE_MENU)
        DrawMenu(g);
    else if(_currentState == STATE_OPTIONS)
        DrawOptions(g);
}
