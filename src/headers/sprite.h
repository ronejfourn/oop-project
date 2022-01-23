#pragma once
#include "graphics.h"
#include <map>
#include <string>

class Sprite {
protected:
    SDL_Texture *_texture;
    SDL_Rect *_src;
public:
    Sprite();
    Sprite(SDL_Texture *texture);
    Sprite(Graphics *g, std::string file_path);
    ~Sprite();

    void SetTexture(SDL_Texture *texture);
    void SetTexture(Graphics *g, std::string file_path);

    SDL_Texture *GetTexture();

    void AddFrame(uint32_t state, int x, int y, int w, int h);

    virtual void InitBuffer(uint32_t count);
    virtual void Draw(Graphics *g, uint32_t state, SDL_FRect &dst, SDL_RendererFlip flip, float angle = 0, SDL_FPoint *center = NULL);
    virtual void Draw(Graphics *g, uint32_t state, SDL_Rect  &dst, SDL_RendererFlip flip, float angle = 0, SDL_Point  *center = NULL);
};
