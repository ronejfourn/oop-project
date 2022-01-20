#include "entity.h"

Entity::Entity() {
    _alive = true;
    _flip  = SDL_FLIP_NONE;
    _recovertime = 100;
    _htime = 0;
}

void Entity::MoveUp() {
    _accn.y = -0.03;
}

void Entity::MoveLeft() {
    _accn.x = -0.03;
}

void Entity::MoveDown() {
    _accn.y = +0.03;
}

void Entity::MoveRight() {
    _accn.x = +0.03;
}

void Entity::AddForce(Vec2f op, float deltatime) {
    _limit_speed = false;
    _accn += op * deltatime;
}

void Entity::TakeDamage(float damage) {
    _hp -= damage;
    _state = "hurt";
    if (_hp <= 0) {
        _alive = false;
    }
}

void Entity::Die() {

}

void Entity::FaceTowards(Vec2f pos) {
    _flip = (pos.x < _tl.x + _dim.x / 2.0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
}

void Entity::Draw(Graphics *g) {
    SDL_FRect tmp = {
        _tl.x , _tl.y,
        _dim.x, _dim.y
    };
    _sprite.Draw(g, _state, tmp, _flip, true);
}

void Entity::Move(float deltatime) {
    _vel += _accn * deltatime;
    _accn.zero();

    float friction = 0.018 * deltatime;
    int sx = _vel.x < 0 ? -1 : 1;
    int sy = _vel.y < 0 ? -1 : 1;
    _vel.x -= friction > sx * _vel.x ? _vel.x : sx * friction;
    _vel.y -= friction > sy * _vel.y ? _vel.y : sy * friction;

    if (_limit_speed) {
        if (!_vel.y && _vel.x) {
            _vel.x = ut_clamp(_vel.x, -5, 5);
        } else if (!_vel.x && _vel.y) {
            _vel.y = ut_clamp(_vel.y, -5, 5);
        } else if (_vel.x && _vel.y) {
            _vel.x = ut_clamp(_vel.x, -3.5, 3.5);
            _vel.y = ut_clamp(_vel.y, -3.5, 3.5);
        }
    } else {
        _limit_speed = true;
    }

    _tl += _vel;
}

Vec2f Entity::GetCenter() {
    return {
        _tl.x + _dim.x / 2,
        _tl.y + _dim.y / 2,
    };
}
