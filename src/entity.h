#pragma once
#include "animated_sprite.h"
#include "utils.h"

class Entity {
protected:
    std::string     _state;
    SDL_RendererFlip _flip;
    Vec2f _tl, _dim;
    AnimatedSprite _sprite;

    bool _alive, _limit_speed;
    float _hp, _recovertime, _htime;
    Vec2f _vel, _accn;

    void Move(float deltatime);
public:
    Entity();

    void AddForce(Vec2f op, float deltatime);

    Vec2f GetCenter();
    float GetHp() {return _hp;}

    void MoveUp   ();
    void MoveLeft ();
    void MoveDown ();
    void MoveRight();
    virtual void Update(float deltatime) = 0;

    virtual void TakeDamage(float damage);
    virtual void Die();
    virtual void FaceTowards(Vec2f pos);

    virtual void Draw(Graphics *g);
};
