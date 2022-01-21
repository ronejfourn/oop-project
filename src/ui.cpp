#include "headers/ui.h"
#include "headers/logger.h"
#include <SDL2/SDL.h>

extern SDL_Texture *singleTexture;

UI *UI::_instance = nullptr;

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
        Logger::LogWarning("Message does not fit in the box. Characters will be squished");
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
    healthDst.w = 100;
    DisplayText(g, "HP:" + std::to_string(int(hp)), healthDst, healthDst.h );
}

void UI::DrawMenu(Graphics *g){
    Vec2i windowSize = g->GetCurrentResolution();
    SDL_Rect menuBox = {0, 0, windowSize.x / 4, windowSize.y / 4};
    menuBox.x = (windowSize.x - menuBox.w) / 2;
    menuBox.y = (windowSize.y - menuBox.h) / 2;
    menuBox.h /= 3;
    DisplayText(g, "Play", menuBox, 35);
    menuBox.y += menuBox.h;
    DisplayText(g, "Options", menuBox, 35);
    menuBox.y += menuBox.h;
    DisplayText(g, "Exit", menuBox, 35);
}

void UI::Draw(Graphics *g, Player &p){
    if(_currentState == STATE_ALIVE)
        DisplayInfo(g, p);
    else if(_currentState == STATE_MENU)
        DrawMenu(g);
}
