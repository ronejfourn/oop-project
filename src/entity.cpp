#include "entity.h"

Entity::Entity() {
    _velx = 0;
    _vely = 0;
    _acnx = 0;
    _acny = 0;
    _alive = true;
    _flip  = false;
}

void Entity::MoveUp() {
    _acny = -0.03;
}

void Entity::MoveLeft() {
    _acnx = -0.03;
}

void Entity::MoveDown() {
    _acny = +0.03;
}

void Entity::MoveRight() {
    _acnx = +0.03;
}

void Entity::AddForce(float acx, float acy, float deltatime) {
    _limit_speed = false;
    _acnx += acx * deltatime;
    _acny += acy * deltatime;
}

void Entity::TakeDamage(float damage) {
    _hp -= damage;
    if (_hp <= 0) {
        _alive = false;
    }
}

void Entity::Die() {

}

void Entity::FaceTowards(float x, float y) {
    _flip = (x < _body.x + _body.w / 2.0);
}

void Entity::Draw(Graphics &g) {
    _sprite.Draw(g, _state, _body, _flip, true);
}

void Entity::Move(float deltatime) {
    _vely += _acny * deltatime;
    _velx += _acnx * deltatime;

    _acnx = 0;
    _acny = 0;

    float friction = 0.018 * deltatime;
    int sx = _velx < 0 ? -1 : _velx > 0 ? 1 : 0;
    int sy = _vely < 0 ? -1 : _vely > 0 ? 1 : 0;
    _velx -= (friction > sx * _velx ? _velx : sx * friction);
    _vely -= (friction > sy * _vely ? _vely : sy * friction);

    if (!_velx && !_vely) {
        _state = "idle";
    } else {
        _state = "run";
    }

    if (_limit_speed) {
        if (!_vely && _velx) {
            _velx = _velx > 5 ? 5 : _velx < -5 ? -5 : _velx;
        } else if (!_velx && _vely) {
            _vely = _vely > 5 ? 5 : _vely < -5 ? -5 : _vely;
        } else if (_velx && _vely) {
            _velx = _velx > 3.5 ? 3.5 : _velx < -3.5 ? -3.5 : _velx;
            _vely = _vely > 3.5 ? 3.5 : _vely < -3.5 ? -3.5 : _vely;
        }
    } else {
        _limit_speed = true;
    }

    _body.x += _velx;
    _body.y += _vely;
}

SDL_Point Entity::GetCenter() {
    return {
        _body.x + _body.w / 2,
        _body.y + _body.h / 2
    };
}
