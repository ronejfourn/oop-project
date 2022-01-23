#pragma once
#include "sprite.h"

class Entity;

class Weapon {
protected:
    Sprite _sprite;
    Entity *_anchor;
    Rectf _box;
    float _radius, _angle;
    Vec2f _dir, _power, _atime;
    SDL_RendererFlip _flip;
public:
    Weapon();
    Weapon(Entity * holder, float rad, std::string name = "");
    void Switch(std::string newweap);
    virtual void Draw(Graphics *g, Vec2f offset);
    virtual void Attack() = 0;
    virtual void Update(float deltatime) = 0;
    virtual void PointTowards(Vec2f target) = 0;
};
