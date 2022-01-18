#pragma once
#include "graphics.h"
#include <map>
#include <string>
#include <SDL2/SDL_rect.h>

enum Facing {LEFT, RIGHT};

class Sprite {
protected:
    SDL_Texture *_texture;
    std::map<std::string, SDL_Rect>_src;
public:
    Sprite();
    Sprite(SDL_Texture *texture);
    Sprite(Graphics &g, std::string file_path);
    ~Sprite();

    void SetTexture(SDL_Texture *texture);
    void SetTexture(Graphics &g, std::string file_path);

    void AddState(std::string state, int x, int y, int w, int h);

    virtual void Draw(Graphics &g, std::string state, SDL_Rect *dst, Facing face, float angle = 0, SDL_Point *center = NULL);
};
