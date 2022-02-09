#include "headers/projectile.h"
#include "headers/entity.h"
#include <cmath>

extern SDL_Texture *singleTexture;

const float ct  = cos(2 * pi / 3);
const float st  = sin(2 * pi / 3);
const float pvel = 1.0f;

Range::Range(Entity * holder, float rad, Weapons name)
	: Weapon(holder, rad, name) {
        if(name == Weapons::bow) {
            _projectilesrc = {308, 186, 7, 21};
            _isbow = true;
        } else {
            _projectilesrc = {335, 175, 15, 15};
            _isbow = false;
        }
    }

void Range::Attack() {
    Projectile pr;
    if(_isbow)
        pr.pos = _box.pos + _box.dim / 4;
    else
        pr.pos = _box.pos;
    pr.ang = atan(_dir.y / _dir.x) - (_dir.x > 0 ? (-pi/2) : pi/2);
    pr.vel.x =  pvel * sin(pr.ang);
    pr.vel.y = -pvel * cos(pr.ang);
    pr.ang *= (180/pi);
    _projectiles.insert(_projectiles.begin(), pr);
}

void Range::Update(float deltatime) {
    Vec2f cen = _anchor->GetCenter();
    _box.pos = cen + _dir * _radius - _box.dim / 2;

    int count = _projectiles.size() - 1;
    for(int i = count; i >= 0; i--) {
        _projectiles[i].pos += _projectiles[i].vel * deltatime;
        _projectiles[i].life += deltatime;
        if (_projectiles[i].life >= 500)
            _projectiles.pop_back();
    }
}

void Range::PointTowards(Vec2f target) {
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
    for(int i = 0; i < _projectiles.size(); i++) {
        projectiledst.x = _projectiles[i].pos.x - offset.x;
        projectiledst.y = _projectiles[i].pos.y - offset.y;
        g->DrawTexture(singleTexture, _projectilesrc, projectiledst, nullptr, _isbow ? _projectiles[i].ang : 0);
    }
}


