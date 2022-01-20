#include "entity.h"
#include "weapon.h"

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
    Vec2f a;
    a = _anchor->GetCenter();
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

void Weapon::PointTowards(Vec2f target) {
    Vec2f a;
    a = _anchor->GetCenter();
    _angle = tau - a.angleBetn(target);
    _flip  = (_angle > halfpi && _angle < thalfpi) ? SDL_FLIP_VERTICAL : SDL_FLIP_HORIZONTAL;
}
