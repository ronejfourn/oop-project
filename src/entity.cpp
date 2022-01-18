#include "entity.h"

Entity::Entity() {
    _velx = 0;
    _vely = 0;
    _acnx = 0;
    _acny = 0;
    _alive = true;
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
    _face = (x >= _center.x) ? Facing::RIGHT : Facing::LEFT;
}

void Entity::Draw(Graphics &g) {
    _sprite.Draw(g, _state, &_body, _face);
}

void Entity::Move(float deltatime) {
    _vely += _acny * deltatime;
    _velx += _acnx * deltatime;

    float friction = 0.018 * deltatime;
    if (_velx) {
        int sign = _velx < 0 ? -1 : 1;
        _velx -= (friction > sign * _velx ? _velx : sign * friction);
    }
    if (_vely) {
        int sign = _vely < 0 ? -1 : 1;
        _vely -= (friction > sign * _vely ? _vely : sign * friction);
    }

    if (!_velx && !_vely) {
        _state = "idle";
    } else {
        _state = "run";
    }

    if (_limit_speed) {
        _velx = _velx > 5 ? 5 : _velx < -5 ? -5 : _velx;
        _vely = _vely > 5 ? 5 : _vely < -5 ? -5 : _vely;
    } else {
        _limit_speed = true;
    }

    _body.x += _velx;
    _body.y += _vely;
    _center.x += _velx;
    _center.y += _vely;

    _acnx = 0;
    _acny = 0;
}
