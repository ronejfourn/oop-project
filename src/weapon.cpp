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
    Switch("");
}

Weapon::Weapon(Entity * holder, float rad, std::string name) : Weapon() {
    _anchor = holder;
    _radius = rad;
    Switch(name);
}


void Weapon::Draw(Graphics *g, Vec2f offset) {
    SDL_FPoint b = {
        _box.dim.x / 2,
        _box.dim.y / 2
    };
    SDL_FRect dst = {
        _box.pos.x - offset.x,
        _box.pos.y - offset.y,
        _box.dim.x, _box.dim.y
    };
    _sprite.Draw(g, "default", dst, _flip, _angle, &b);
}

void Weapon::Switch(std::string name) {
    if (name == "regular_sword") {
        _sprite.AddState("default", 323, 26, 10, 21);
        _box.dim = {10 * 2, 21 * 2};
        _power = {20, 15};
        _atime = {100, 0};
    } else if (name == "red_gem_sword") {
        _sprite.AddState("default", 339, 26, 10, 21);
        _box.dim = {10 * 2, 21 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else if (name == "hammer") {
        _sprite.AddState("default", 307, 55, 10, 24);
        _box.dim = {10 * 2, 24 * 2};
        _power = {15, 15};
        _atime = {300, 0};
    } else if (name == "baton_with_spikes") {
        _sprite.AddState("default", 323, 57, 10, 22);
        _box.dim = {10 * 2, 22 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else if (name == "mace") {
        _sprite.AddState("default", 339, 55, 10, 24);
        _box.dim = {10 * 2, 24 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else if (name == "katana") {
        _sprite.AddState("default", 293, 82, 6, 29);
        _box.dim = {6 * 2, 29 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else if (name == "saw_sword") {
        _sprite.AddState("default", 307, 86, 10, 25);
        _box.dim = {10 * 2, 25 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else if (name == "axe") {
        _sprite.AddState("default", 341, 90, 9, 21);
        _box.dim = {9 * 2, 21 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else if (name == "machete") {
        _sprite.AddState("default", 294, 121, 5, 22);
        _box.dim = {5 * 2, 22 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else if (name == "cleaver") {
        _sprite.AddState("default", 310, 124, 8, 19);
        _box.dim = {8 * 2, 19 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else if (name == "knight_sword") {
        _sprite.AddState("default", 339, 114, 10, 29);
        _box.dim = {10 * 2, 29 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else if (name == "weapon_red_magic_staff") {
        _sprite.AddState("default", 324, 145, 8, 30);
        _box.dim = {8 * 2, 30 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else if (name == "weapon_bow") {
        _sprite.AddState("default", 325, 180, 7, 25);
        _box.dim = {7 * 2, 25 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    } else { // knife as default
        _sprite.AddState("default", 293, 18, 6, 13);
        _box.dim = {6 * 2, 13 * 2};
        _power = {10, 10};
        _atime = {300, 0};
    }
}
