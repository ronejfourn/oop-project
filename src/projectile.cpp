#include "headers/projectile.h"
#include <cmath>
#include <string>

extern SDL_Texture *singleTexture;

const float ct  = cos(2 * pi / 3);
const float st  = sin(2 * pi / 3);

Range::Range()
	: Weapon() {
        _count = 0;
        _vel = 1.0f;
    }

Range::Range(Entity * holder, float rad, Weapons name)
	: Weapon(holder, rad, name) {
        if(name == Weapons::bow) {
            _projectilesrc = {308, 186, 7, 21};
            _isbow = true;
        }
        else {
            _projectilesrc = {335, 175, 15, 15};
            _isbow = false;
        }
        _count = 0;
        _vel = 1.0f;
    }

void Range::Attack() {
    if(_isbow)
        _pos[_count] = _box.pos + _box.dim / 4;
    else
        _pos[_count] = _box.pos;
    Vec2f tmp = _target - _pos[_count];
    _ang[_count] = atan(tmp.y / tmp.x) * 180 / pi - (tmp.x > 0 ? (-90) : 90);
    _count++; 
}

void Range::Update(float deltatime) {
    Vec2f cen = _anchor->GetCenter();
    _box.pos = cen + _dir * _radius - _box.dim / 2;

    for(int i = 0; i < _count; i++) {
        _pos[i].x += deltatime * _vel * sin(_ang[i] * pi / 180);
        _pos[i].y += deltatime * -_vel * cos(_ang[i] * pi / 180);
    }
}

void Range::PointTowards(Vec2f target) {
    _target = target;
    Vec2f cen = _anchor->GetCenter();
    _dir = (target - cen).normalized();
    _angle = atan(_dir.y / _dir.x) * 180 / pi;
    _flip = _dir.x < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
}

void Range::Draw(Graphics *g, Vec2f offset) {
    SDL_FPoint b = {
        _box.dim.x / 2,
        _box.dim.y / 2
    };
    SDL_FRect dst = {
        _box.pos.x - offset.x,
        _box.pos.y - offset.y,
        _box.dim.x, _box.dim.y
    };
    _sprite.Draw(g, 0, dst, _flip, _angle, &b);

    SDL_Rect projectiledst = {0, 0, _projectilesrc.w * 2, _projectilesrc.h * 2};
    for(int i = 0; i < _count; i++) {
        projectiledst.x = _pos[i].x - offset.x;
        projectiledst.y = _pos[i].y - offset.y;
        g->DrawTexture(singleTexture, _projectilesrc, projectiledst, nullptr, _isbow ? _ang[i] : 0);
    }
}


