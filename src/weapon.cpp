#include "headers/entity.h"
#include "headers/weapon.h"

Weapon::Weapon() {
    _radius = 0;
    _flip   = SDL_FLIP_NONE;
}

Weapon::Weapon(SDL_Texture *texture, Entity *anchor, float rad) : Weapon() {
    _sprite.SetTexture(texture);
    _anchor = anchor;
    _radius = rad;
    _sprite.AddState("default", 325, 180, 7, 25); // PlaceHolder
}

void Weapon::Draw(Graphics *g, Vec2f offset) {
    Vec2f a  = _anchor->GetCenter();
    float an = asin(_dir.x) * 180 / pi;
    an = _dir.y < 0 ? 270 + an : 90  - an;
    Vec2f rd = _dir * _radius;
    SDL_FPoint b = { // Placeholder
        10.5, 37.5
    };
    SDL_FRect dst = {
        a.x + rd.x - b.x - offset.x,
        a.y - rd.y - b.y - offset.y,
        b.x * 2,
        b.y * 2
    };
    g->DrawRect(dst, 255, 0, 0);
    _sprite.Draw(g, "default", dst, _flip, -an, &b);
}

void Weapon::PointTowards(Vec2f target) {
    _dir = target - _anchor->GetCenter();
    _dir = _dir.normalized();
    _dir.y *= -1;
    _flip = _dir.x < 0 ? SDL_FLIP_NONE : SDL_FLIP_VERTICAL;
}
