#include "headers/entity.h"

Entity::Entity() {
    _limit_speed = true;
    _hp = 0.0f;
    _alive = true;
    _flip  = SDL_FLIP_NONE;
    _recovertime = 100;
    _htime = 0;
    _accns = 0.003f;
    _maxspd = 0.5f;
    _maxdspd = _maxspd * Q_rsqrt(2);
}

void Entity::MoveUp() {
    _accn.y = -_accns;
}

void Entity::MoveLeft() {
    _accn.x = -_accns;
}

void Entity::MoveDown() {
    _accn.y = +_accns;
}

void Entity::MoveRight() {
    _accn.x = +_accns;
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
    _flip = (pos.x < _box.pos.x + _box.dim.x / 2.0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
}

void Entity::Draw(Graphics *g, Vec2f offset) {
    SDL_FRect tmp = {
        _box.pos.x - offset.x,
        _box.pos.y - offset.y,
        _box.dim.x, _box.dim.y
    };
    _sprite.Draw(g, _state, tmp, _flip, true);
}

void Entity::Move(float deltatime) {
    _vel += _accn * deltatime;
    _accn.zero();

    float friction = 0.002f * deltatime;
    if (_vel.x != 0) {
        int sx = _vel.x < 0 ? -1 : 1;
        _vel.x -= friction > sx * _vel.x ? _vel.x : sx * friction;
    }
    if (_vel.y != 0) {
        int sy = _vel.y < 0 ? -1 : 1;
        _vel.y -= friction > sy * _vel.y ? _vel.y : sy * friction;
    }

    if (_limit_speed) {
        if (!_vel.y && _vel.x) {
            _vel.x = ut_clamp(_vel.x, -_maxspd, _maxspd);
        } else if (!_vel.x && _vel.y) {
            _vel.y = ut_clamp(_vel.y, -_maxspd, _maxspd);
        } else if (_vel.x && _vel.y) {
            _vel.x = ut_clamp(_vel.x, -_maxdspd, _maxdspd);
            _vel.y = ut_clamp(_vel.y, -_maxdspd, _maxdspd);
        }
    } else {
        _limit_speed = true;
    }

    _box.pos += _vel * deltatime;
}

Vec2f Entity::GetCenter() {
    return _box.pos + _box.dim / 2;
}
