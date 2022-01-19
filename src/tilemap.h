#pragma once
#include "graphics.h"
#include <vector>
#include <SDL2/SDL_rect.h>

class Tilemap {
protected:
    SDL_Texture *_texture;
    std::vector<SDL_Rect> _tiles;
public:
    Tilemap();
    Tilemap(SDL_Texture *texture);
    Tilemap(Graphics &g, const char *file_path);
    ~Tilemap();

    void SetTexture(SDL_Texture *texture);
    void SetTexture(Graphics &g, const char *file_path);

    void AddTile(int x, int y, int w = 16, int h = 16);
    void Draw(Graphics &g, uint32_t index, SDL_Rect &dst);
};
