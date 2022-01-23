#include "SDL2/SDL_render.h"
#include "headers/entity.h"
#include "headers/melee.h"
#include <cmath>

const float ct  = cos(2 * pi / 3);
const float st  = sin(2 * pi / 3);

Melee::Melee()
	: Weapon() {
        _isattacking = false;
        _isonhold    = false;
        _facingright = false;
        _htime = {30000, 0};
    }

Melee::Melee(Entity * holder, float rad, std::string name)
	: Weapon(holder, rad, name) {
        _isattacking = false;
        _isonhold    = false;
        _facingright = false;
        _htime = {300, 0};
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
            _angle = atan(tmp.y / tmp.x) * 180 / pi + 60;
            _facingright = true;
        } else {
            _dir.x =  tmp.x * ct - tmp.y * st;
            _dir.y =  tmp.x * st + tmp.y * ct;
            _flip = SDL_FLIP_HORIZONTAL;
            _angle = atan(tmp.y / tmp.x) * 180 / pi - 60;
            _facingright = false;
        }
        _box.pos = cen + _dir * _radius - _box.dim / 2;
	}
}

void Melee::Update(float deltatime) {
	if (_isattacking) {
        Vec2f cen = _anchor->GetCenter();
        float ang = 4 / 3 * pi * deltatime / _atime.x;
        float ca = cos(ang), sa = sin(ang);
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
