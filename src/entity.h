#include "animated_sprite.h"

class Entity {
protected:
    std::string     _state;
    SDL_RendererFlip _flip;
    SDL_FRect      _body;
    AnimatedSprite _sprite;

    bool _alive, _limit_speed;
    float _hp, _recovertime, _htime;
    float _velx, _vely;
    float _acnx, _acny;

    void Move(float deltatime);
public:
    Entity();

    void AddForce(float acx, float acy, float deltatime);

    SDL_FPoint GetCenter();
    float GetHp();

    void MoveUp   ();
    void MoveLeft ();
    void MoveDown ();
    void MoveRight();
    virtual void Update(float deltatime) = 0;

    virtual void TakeDamage(float damage);
    virtual void Die();
    virtual void FaceTowards(float x, float y);

    virtual void Draw(Graphics *g);
};
