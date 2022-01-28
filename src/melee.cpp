#include "SDL2/SDL_render.h"
#include "headers/entity.h"
#include "headers/melee.h"
#include "headers/collision.h"
#include <cmath>

const float ct  = cos(2 * pi / 3);
const float st  = sin(2 * pi / 3);

Melee::Melee()
	: Weapon() {
        _isattacking = false;
        float sqr = (_radius * _radius + _box.dim.y * _box.dim.y / 4) - (_radius * _box.dim.y * cos(5.0f / 6 * pi));
        _range = Q_rsqrt(sqr) * sqr;
        _isonhold    = false;
        _facingright = false;
        _htime = {500, 0};
        _atime = {180, 0};
        _vel = 0; _accn = 4 * 4 / 3.0f * pi / (_atime.x * _atime.x);
    }

Melee::Melee(Entity * holder, float rad, Weapons name)
	: Weapon(holder, rad, name) {
        _isattacking = false;
        _isonhold    = false;
        float sqr = (_radius * _radius + _box.dim.y * _box.dim.y / 4) - (_radius * _box.dim.y * cos(5.0f / 6 * pi));
        _range = Q_rsqrt(sqr) * sqr;
        _facingright = false;
        _htime = {500, 0};
        _atime = {180, 0};
        _vel = 0; _accn = 4 * 4 / 3.0f * pi / (_atime.x * _atime.x);
    }

void Melee::Attack() {
    if (!_isattacking) {
        _isattacking = true;
    }
}

void Melee::PointTowards(Vec2f target) {
	if (!_isattacking) {
        Vec2f cen = _anchor->GetCenter();
        Vec2f tmp = (target - cen).normalized();
        if (tmp.x < 0 && !_isonhold) {
            _dir.x = tmp.x * ct - tmp.y * st;
            _dir.y = tmp.x * st + tmp.y * ct;
            _flip = SDL_FLIP_HORIZONTAL;
            _angle = atan(tmp.y / tmp.x) * 180 / pi + 60;
            _facingright = false;
        } else if (!_isonhold) {
            _dir.x =  tmp.x * ct + tmp.y * st;
            _dir.y = -tmp.x * st + tmp.y * ct;
            _flip = SDL_FLIP_NONE;
            _angle = atan(tmp.y / tmp.x) * 180 / pi - 60;
            _facingright = true;
        } else if (tmp.x < 0) {
            _dir.x =  tmp.x * ct + tmp.y * st;
            _dir.y = -tmp.x * st + tmp.y * ct;
            _flip = SDL_FLIP_NONE;
            _angle = atan(tmp.y / tmp.x) * 180 / pi + 120;
            _facingright = true;
        } else {
            _dir.x =  tmp.x * ct - tmp.y * st;
            _dir.y =  tmp.x * st + tmp.y * ct;
            _flip = SDL_FLIP_HORIZONTAL;
            _angle = atan(tmp.y / tmp.x) * 180 / pi - 120;
            _facingright = false;
        }
        _box.pos = cen + _dir * _radius - _box.dim / 2;
	}
}

void Melee::Update(float deltatime) {
	if (_isattacking) {
        _vel += (_atime.y < _atime.x / 2 ? 1 : -1) * _accn * deltatime;
        Vec2f cen = _anchor->GetCenter();
        float ang = _vel * deltatime;
        float ca  = cos(ang), sa = sin(ang);
        Vec2f tmp = _dir;
        if (_facingright) {
            _dir.x = tmp.x * ca - tmp.y * sa;
            _dir.y = tmp.x * sa + tmp.y * ca;
            _flip = SDL_FLIP_NONE;
            _angle += ang / pi * 180;
        } else {
            _dir.x =  tmp.x * ca + tmp.y * sa;
            _dir.y = -tmp.x * sa + tmp.y * ca;
            _flip  = SDL_FLIP_HORIZONTAL;
            _angle -= ang / pi * 180;
        }
        _box.pos = cen + _dir * _radius - _box.dim / 2;
        _atime.y += deltatime;
        if (_atime.y >= _atime.x) {
            _vel = 0;
            _atime.y = 0;
            _htime.y = 0;
            _isattacking = false;
            _isonhold = !_isonhold;
        }
    } else if (_isonhold) {
        _htime.y += deltatime;
        if (_htime.y > _htime.x) {
            _htime.y = 0;
            _isonhold = false;
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
    if (!_isattacking) return false;
    return Collision::RayVsRect(entity->GetBox(), _anchor->GetCenter(), _dir, _range);
}
