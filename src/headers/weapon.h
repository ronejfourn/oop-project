#pragma once
#include "sprite.h"

class Entity;

enum class Weapons {
    knife,
    regular_sword,
    red_gem_sword,
    hammer,
    baton_with_spikes,
    mace,
    katana,
    saw_sword,
    axe,
    machete,
    cleaver,
    knight_sword,
    red_magic_staff,
    bow
};

class Weapon {
protected:
    Sprite _sprite;
    Entity *_anchor;
    Rectf _box;
    float _radius, _angle;
    Vec2f _dir, _power;
    SDL_RendererFlip _flip;
public:
    Weapon();
    Weapon(Entity * holder, float rad, Weapons name = Weapons::knife);
    void Switch(Weapons newweap);
    virtual void Draw(Graphics *g, Vec2f offset);
    virtual void Attack() = 0;
    virtual void Update(float deltatime) = 0;
    virtual void PointTowards(Vec2f target) = 0;
    Rectf GetBox() {return _box;}
    virtual bool Collision (Entity *) = 0;
    virtual void UpdatePosition();
};
