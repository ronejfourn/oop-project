#include "SDL2/SDL_render.h"
#include "headers/entity.h"
#include "headers/melee.h"
#include "headers/collision.h"
#include <cmath>

namespace {
    const float ct  = cos(2 * pi / 3);
    const float st  = sin(2 * pi / 3);
    const float hldtime  = 500;
    const float atktime  = 120;
    const float omega    = 4.0f / 3.0f * pi / atktime;
};

Melee::Melee(Entity * holder, float rad, Weapons name)
	: Weapon(holder, rad, name) {
        _state = 0;
        float sqr = (_radius * _radius + _box.dim.y * _box.dim.y / 4) - (_radius * _box.dim.y * cos(5.0f / 6 * pi));
        _range = Q_rsqrt(sqr) * sqr;
        _htime = 0;
        _atime = 0;
        _vel = 0;
    }

void Melee::Attack() {
    if (!(_state & Attacking)) {
        _state |= Attacking;
    }
}

void Melee::PointTowards(Vec2f target) {
	if (!(_state & Attacking)) {
        Vec2f cen = _anchor->GetCenter();
        Vec2f tmp = (target - cen).normalized();
        if (tmp.x < 0 && !(_state & OnHold)) {
            _dir.x = tmp.x * ct - tmp.y * st;
            _dir.y = tmp.x * st + tmp.y * ct;
            _flip = SDL_FLIP_HORIZONTAL;
            _angle = atan(tmp.y / tmp.x) * 180 / pi + 60;
            _state &= ~FacingRight;
        } else if (!(_state & OnHold)) {
            _dir.x =  tmp.x * ct + tmp.y * st;
            _dir.y = -tmp.x * st + tmp.y * ct;
            _flip = SDL_FLIP_NONE;
            _angle = atan(tmp.y / tmp.x) * 180 / pi - 60;
            _state |= FacingRight;
        } else if (tmp.x < 0) {
            _dir.x =  tmp.x * ct + tmp.y * st;
            _dir.y = -tmp.x * st + tmp.y * ct;
            _flip = SDL_FLIP_NONE;
            _angle = atan(tmp.y / tmp.x) * 180 / pi + 120;
            _state |= FacingRight;
        } else {
            _dir.x =  tmp.x * ct - tmp.y * st;
            _dir.y =  tmp.x * st + tmp.y * ct;
            _flip = SDL_FLIP_HORIZONTAL;
            _angle = atan(tmp.y / tmp.x) * 180 / pi - 120;
            _state &= ~FacingRight;
        }
        _box.pos = cen + _dir * _radius - _box.dim / 2;
	}
}

void Melee::Update(float deltatime) {
	if (_state & Attacking) {
        Vec2f cen = _anchor->GetCenter();
        float ta = omega * deltatime;
        float ca  = cos(ta), sa = sin(ta);
        Vec2f tmp = _dir;
        if (_state & FacingRight) {
            _dir.x = tmp.x * ca - tmp.y * sa;
            _dir.y = tmp.x * sa + tmp.y * ca;
            _flip = SDL_FLIP_NONE;
            _angle += ta / pi * 180;
        } else {
            _dir.x =  tmp.x * ca + tmp.y * sa;
            _dir.y = -tmp.x * sa + tmp.y * ca;
            _flip  = SDL_FLIP_HORIZONTAL;
            _angle -= ta / pi * 180;
        }
        _box.pos = cen + _dir * _radius - _box.dim / 2;
        _atime += deltatime;
        if (_atime >= atktime) {
            _vel   = 0;
            _atime = 0;
            _htime = 0;
            _state &= ~Attacking;
            _state ^= OnHold;
        }
    } else if (_state & OnHold) {
        _htime += deltatime;
        if (_htime > hldtime) {
            _htime = 0;
            _state &= ~OnHold;
        }
    }
}

void Melee::Draw(Graphics *g, Vec2f offset) {
    SDL_FPoint b = {
        _box.dim.x / 2,
        _box.dim.y / 2
    };
    SDL_FRect dst = {
        _box.pos.x - offset.x,
        _box.pos.y - offset.y,
        _box.dim.x, _box.dim.y
    };
    Vec2f dc = _anchor->GetCenter() - offset;
    Vec2f ac = _dir * _range + dc;
    _sprite.Draw(g, 0, dst, _flip, _angle, &b);
}

bool Melee::Collision (Entity *entity) {
    if (!(_state & Attacking)) return false;
    return Collision::RayVsRect(entity->GetBox(), _anchor->GetCenter(), _dir, _range);
}

void Melee::EndAttack() {
	_atime = 0;
	_state &= ~Attacking;
}
