#pragma once
#include "animated_sprite.h"
#include "utils.h"

enum class EntityState {
    Idle,
    Run,
    Hurt,
    _count
};

class Entity {
protected:
    EntityState _state;
    SDL_RendererFlip _flip;
    Rectf _box;
    AnimatedSprite _sprite;

    bool _alive, _limit_speed;
    float _hp, _mass, _recovertime, _htime;
    Vec2f _vel, _accn;

    void CollideAgainstMap(Map &map, float deltatime);
    void Accelerate(float deltatime);
public:
    Entity();

    void AddForce(Vec2f op);
    Vec2f GetCenter() {return _box.pos + _box.dim / 2;}
    float GetHp() {return _hp;}
    Rectf GetBox() {return _box;}

    virtual void Update(float deltatime) = 0;
    virtual void Die() = 0;
    virtual void Seek(Vec2f pos) = 0;
    virtual void TakeDamage(float damage) = 0;
    virtual void Draw(Graphics *g, Vec2f offset) = 0;
};
