#include "headers/player.h"
#include "headers/weapon.h"

extern SDL_Texture *singleTexture;

namespace {
    constexpr float s_scale  = 3;
    constexpr int   s_width  = 16;
    constexpr int   s_height = 22;
    constexpr float p_width  = s_width  * s_scale;
    constexpr float p_height = s_height * s_scale;
}

Player::Player() : Entity() {
    _weapon  = new Weapon(singleTexture, this, 20);
    _state   = "idle";
    _box.dim = {p_width, p_height};
    _hp = 100;
    _sprite.SetTexture(singleTexture);
    _sprite.AddAnimation("idle", 128, 106, s_width, s_height, 4);
    _sprite.AddAnimation("run" , 192, 106, s_width, s_height, 4);
    _sprite.AddAnimation("hurt", 256, 102, s_width, s_height, 1);
}

Player::Player(float center_x, float center_y) : Player() {
    _box.pos = { center_x - p_width / 2, center_y - p_height / 2 };
    _box.dim = { p_width, p_height };
}

void Player::TakeDamage(float damage) {
    if (_state != "hurt") {
        _hp -= damage;
        _state = "hurt";
        if (_hp <= 0)
            _alive = false;
    }
}

void Player::FaceTowards(Vec2f pos) {
    _flip = (pos.x < _box.pos.x + _box.dim.x / 2.0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    _weapon->PointTowards(pos);
}

void Player::Update(float deltatime) {
    if (_state != "hurt")
        Move(deltatime);

    if (_state == "hurt") {
        _htime += deltatime;
        if (_htime > _recovertime) {
            _htime = 0;
            _state = "idle";
        }
    } else if (_vel.x || _vel.y) {
        _state = "run";
    } else {
        _state = "idle";
    }
}

void Player::Draw(Graphics *g, Vec2f offset) {
    SDL_FRect dst = {
        _box.pos.x - offset.x,
        _box.pos.y - offset.y,
        _box.dim.x, _box.dim.y
    };
    if (_state == "hurt") {
        dst.y += 4;
        SDL_SetTextureAlphaMod(_sprite.GetTexture(), 200);
        SDL_SetTextureColorMod(_sprite.GetTexture(), 200, 0, 0);
        _sprite.Draw(g, _state, dst, _flip, true);
        SDL_SetTextureAlphaMod(_sprite.GetTexture(), 255);
        SDL_SetTextureColorMod(_sprite.GetTexture(), 255, 255, 255);
    } else {
        _sprite.Draw(g, _state, dst, _flip, true);
    }
    _weapon->Draw(g, offset);
}
