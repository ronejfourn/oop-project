#pragma once
#include "graphics.h"
#include <vector>
#include <string>

class Tilemap {
protected:
    SDL_Texture *_texture;
    std::vector<SDL_Rect> _tiles;
public:
    Tilemap();
    Tilemap(SDL_Texture *texture);
    Tilemap(Graphics *g, std::string file_path);
    ~Tilemap();

    void SetTexture(SDL_Texture *texture);
    void SetTexture(Graphics *g, std::string file_path);

    void AddTile(int x, int y, int w = 16, int h = 16);
    void Draw(Graphics *g, uint32_t index, SDL_Rect &dst);
};
