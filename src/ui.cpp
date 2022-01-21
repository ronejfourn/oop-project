#include "headers/ui.h"
#include "headers/logger.h"
#include <SDL2/SDL.h>

extern SDL_Texture *singleTexture;

ui *ui::_instance = nullptr;

ui::ui(){
    currentState = STATE_MENU;
}

ui *ui::GetInstance(){
    if(!_instance) _instance = new ui;
    return _instance;
}

ui::~ui(){}

void ui::LoadFont(Graphics *g) {
    fontTexture = g->LoadImage("../../res/font.png");
}

State ui::GetCurrentState(){
    return currentState;
}

void ui::SetState(State state){
    currentState = state;
}

SDL_Rect ui::GetCharCoord(const char ch){
    SDL_Rect tmp = {0, 0, 14, 18};         // dimension of each character 14*18
    tmp.x = ((ch - 32) % 18) * tmp.w;
    tmp.y = ((ch - 32) / 18) * tmp.h;

    return tmp;
}

void ui::DisplayText(Graphics *g, std::string msg, SDL_Rect dst){
    if((dst.h / 18) * (dst.w / 14) < msg.size())
        Logger::LogWarning("Message does not fit in the box. Characters will be squished");
    const char *msgStr = msg.c_str();

    SDL_Rect chrDst;
    chrDst.h = dst.h < 18 ? dst.h : 18;
    chrDst.w = (dst.w / msg.size()) < 14 ? dst.w / msg.size() : 14;
    chrDst.x = dst.x;
    chrDst.y = dst.y;

    for(int i = 0; i < msg.size(); i++){
        SDL_Rect src = GetCharCoord(msgStr[i]);
        g->DrawTexture(fontTexture, src, chrDst);
        chrDst.x += chrDst.w;
    }
}

void ui::DisplayInfo(Graphics *g, Player &p){
    float hp = p.GetHp();
    SDL_Rect src = {0, 256, 16, 16};
    src.x = 288 * (hp >= 75) + 304 * (hp < 75 && hp >=25) + 320 * (hp < 25);

    SDL_Rect healthDst = {0, 0, src.w, src.h};
    g->DrawTexture(singleTexture, src, healthDst);

    healthDst.x += src.w;
    healthDst.w = 100;
    DisplayText(g, "HP:", healthDst);

    healthDst.x += 14 * 3;
    DisplayText(g, std::to_string(int(hp)), healthDst);
}

void ui::Draw(Graphics *g, Player &p){
    if(currentState == STATE_ALIVE)
        DisplayInfo(g, p);
}
