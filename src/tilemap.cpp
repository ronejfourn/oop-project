#include "tilemap.h"
#include "logger.h"

Tilemap::Tilemap() {}

Tilemap::~Tilemap() {}

Tilemap::Tilemap(SDL_Texture *texture) {
    _texture = texture;
}

Tilemap::Tilemap(Graphics &g, const char *file_path) {
    _texture = g.LoadImage(file_path);
}

void Tilemap::SetTexture(SDL_Texture *texture) {
    _texture = texture;
}

void Tilemap::SetTexture(Graphics &g, const char *file_path) {
    _texture = g.LoadImage(file_path);
}

void Tilemap::AddTile(int x, int y, int w, int h) {
    _tiles.push_back({x, y, w, h});
}

void Tilemap::Draw(Graphics &g, uint32_t index, SDL_Rect &dst) {
    if (index >= _tiles.size())
        Logger::LogError("Tilemap Index Out of Range");
    g.DrawTexture(_texture, _tiles[index], dst, NULL, 0, false);
}
