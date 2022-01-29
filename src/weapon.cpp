#include "headers/entity.h"
#include "headers/weapon.h"

extern SDL_Texture *singleTexture;

//TODO: put proper values for power(damage and knockback) and aspd(atime)

Weapon::Weapon() {
    _sprite.SetTexture(singleTexture);
    _anchor = nullptr;
    _radius = 0;
    _angle = 0;
    _flip = SDL_FLIP_NONE;
    _sprite.InitBuffer(1);
    Switch(Weapons::knife);
}

Weapon::Weapon(Entity * holder, float rad, Weapons name) : Weapon() {
    _anchor = holder;
    _radius = rad;
    Switch(name);
}

void Weapon::UpdatePosition() {
    _box.pos = _anchor->GetCenter() + _dir * _radius - _box.dim / 2;
}

void Weapon::Switch(Weapons name) {
    switch(name) {
    case Weapons::regular_sword:
        _sprite.AddFrame(0, 323, 26, 10, 21);
        _box.dim = {10 * 2, 21 * 2};
        _power = {20, 15};
        break;
    case Weapons::baton_with_spikes:
        _sprite.AddFrame(0, 323, 57, 10, 22);
        _box.dim = {10 * 2, 22 * 2};
        _power = {10, 10};
        break;
    case Weapons::machete:
        _sprite.AddFrame(0, 294, 121, 5, 22);
        _box.dim = {5 * 2, 22 * 2};
        _power = {10, 10};
        break;
    case Weapons::cleaver:
        _sprite.AddFrame(0, 310, 124, 8, 19);
        _box.dim = {8 * 2, 19 * 2};
        _power = {10, 10};
        break;
    case Weapons::red_magic_staff:
        _sprite.AddFrame(0, 324, 145, 8, 30);
        _box.dim = {8 * 2, 30 * 2};
        _power = {10, 10};
        break;
    case Weapons::bow:
        _sprite.AddFrame(0, 325, 180, 7, 25);
        _box.dim = {7 * 2, 25 * 2};
        _power = {10, 10};
        break;
    default:
        _sprite.AddFrame(0, 293, 18, 6, 13);
        _box.dim = {6 * 2, 13 * 2};
        _power = {10, 10};
        break;
    }
}
