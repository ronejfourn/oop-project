#include "animated_sprite.h"

class Entity {
protected:
    std::string _state;
    Facing      _face;
    SDL_Rect    _body;
    SDL_Point   _center;
    AnimatedSprite _sprite;

    bool _alive, _limit_speed;
    float _hp;
    float _velx, _vely;
    float _acnx, _acny;

    void Move(float deltatime);
public:
    Entity();

    void AddForce(float acx, float acy, float deltatime);

    void MoveUp   ();
    void MoveLeft ();
    void MoveDown ();
    void MoveRight();
    virtual void Update(float deltatime) = 0;

    void TakeDamage(float damage);
    void Die();
    void FaceTowards(float x, float y);

    void Draw(Graphics &g);
};
