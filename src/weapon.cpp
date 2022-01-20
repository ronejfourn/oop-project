#include "entity.h"
#include "weapon.h"
#include <cmath>
#include <iostream>
constexpr float pi = 3.14159f;

Weapon::Weapon() {
    _radius = 0;
    _angle  = 0;
    _flip   = SDL_FLIP_NONE;
}

Weapon::Weapon(SDL_Texture *texture, Entity *anchor, float rad) : Weapon() {
    _sprite.SetTexture(texture);
    _anchor = anchor;
    _radius = rad;
    _sprite.AddState("default", 323, 26, 10, 21); // PlaceHolder
}

void Weapon::Draw(Graphics *g) {
    SDL_FPoint a = _anchor->GetCenter();
    SDL_FPoint b = { // Placeholder
        15, 31.5
    };
    SDL_FRect dst = {
        a.x + float(cos(_angle)) * _radius - b.x,
        a.y - float(sin(_angle)) * _radius - b.y,
        b.x * 2,
        b.y * 2
    };
    g->DrawRect(dst, 255, 0, 0);
    _sprite.Draw(g, "default", dst, _flip, true, -_angle * 180 / pi, &b);
}

void Weapon::PointTowards(SDL_FPoint target) {
    SDL_FPoint a = _anchor->GetCenter();
    float dfx = target.x - a.x;
    float dfy = a.y - target.y;
    _angle = dfx ? std::atan(std::abs(dfy / dfx)) : pi / 2;
    if (dfx >= 0 && dfy >= 0) {
        _angle += 0;
        _flip = SDL_FLIP_NONE;
    } else if (dfx <= 0 && dfy >= 0) {
        _angle = pi - _angle;
        _flip = SDL_FLIP_VERTICAL;
    } else if (dfx <= 0 && dfy <= 0) {
        _angle = pi + _angle;
        _flip = SDL_FLIP_VERTICAL;
    } else if (dfx >= 0 && dfy <= 0) {
        _angle = 2 * pi - _angle;
        _flip = SDL_FLIP_NONE;
    }
}
