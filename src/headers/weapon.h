#pragma once
#include "sprite.h"

class Entity;

class Weapon {
private:
    Sprite _sprite;
    Entity *_anchor;
    float _radius;
    Vec2f _dir;
    SDL_RendererFlip _flip;
public:
    Weapon();
    Weapon(SDL_Texture *texture, Entity *anchor, float rad);

    virtual void Draw(Graphics *g, Vec2f offset);
    void PointTowards(Vec2f target);
};
