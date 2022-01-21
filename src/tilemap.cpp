#include "headers/tilemap.h"

Tilemap::Tilemap() {
    _texture = nullptr;
}

Tilemap::~Tilemap() {}

Tilemap::Tilemap(SDL_Texture *texture) {
    _texture = texture;
}

Tilemap::Tilemap(Graphics *g, std::string file_path) {
    _texture = g->LoadImage(file_path.c_str());
}

void Tilemap::SetTexture(SDL_Texture *texture) {
    _texture = texture;
}

void Tilemap::SetTexture(Graphics *g, std::string file_path) {
    _texture = g->LoadImage(file_path.c_str());
}

void Tilemap::AddTile(int x, int y, int w, int h) {
    _tiles.push_back({x, y, w, h});
}

void Tilemap::Draw(Graphics *g, uint32_t index, SDL_Rect &dst) {
    if (index >= _tiles.size())
        Logger::LogError("Tilemap Index Out of Range");
    g->DrawTexture(_texture, _tiles[index], dst);
}
