#include "player.h"

extern SDL_Texture *singleTexture;

namespace {
    constexpr float s_scale  = 3;
    constexpr int   s_width  = 16;
    constexpr int   s_height = 22;
    constexpr float p_width  = s_width  * s_scale;
    constexpr float p_height = s_height * s_scale;
}

Player::Player() : Entity() {
    _state  = "idle";
    _body   = {0, 0, p_width, p_height};

    _hp = 100;

    _sprite.SetTexture(singleTexture);
    _sprite.AddAnimation("idle", 128, 106, s_width, s_height, 4);
    _sprite.AddAnimation("run" , 192, 106, s_width, s_height, 4);
    _sprite.AddAnimation("hurt", 256, 102, s_width, s_height, 1);
}

Player::Player(float center_x, float center_y) : Player() {
    _body   = {
        center_x - p_width  / 2,
        center_y - p_height / 2,
        p_width,
        p_height
    };
}

void Player::TakeDamage(float damage) {
    if (_state != "hurt") {
        _hp -= damage;
        _state = "hurt";
        if (_hp <= 0)
            _alive = false;
    }
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
    } else if (_velx || _vely) {
        _state = "run";
    } else {
        _state = "idle";
    }
}

void Player::Draw(Graphics *g) {
    if (_state == "hurt") {
        _body.y -= 4;
        SDL_SetTextureAlphaMod(_sprite.GetTexture(), 200);
        SDL_SetTextureColorMod(_sprite.GetTexture(), 200, 0, 0);
        _sprite.Draw(g, _state, _body, _flip, true);
        SDL_SetTextureAlphaMod(_sprite.GetTexture(), 255);
        SDL_SetTextureColorMod(_sprite.GetTexture(), 255, 255, 255);
        _body.y += 4;
    } else {
        _sprite.Draw(g, _state, _body, _flip, true);
    }
}
