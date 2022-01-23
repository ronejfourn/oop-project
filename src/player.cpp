#include "headers/player.h"
#include "headers/melee.h"

extern SDL_Texture *singleTexture;

namespace {
    const int s_width  = 16;
    const int s_height = 22;
    const float p_width  = s_width  * 2;
    const float p_height = s_height * 2;
}

Player::Player() : Entity() {
    _weapon  = new Melee(this, 25, "regular_sword");
    _state   = "idle";
    _box.dim = {p_width, p_height};
    _hp = 100;
    _sprite.SetTexture(singleTexture);
    _sprite.AddAnimation("idle", 128, 106, s_width, s_height, 4);
    _sprite.AddAnimation("run" , 192, 106, s_width, s_height, 4);
    _sprite.AddAnimation("hurt", 256, 102, s_width, s_height, 1);
}

Player::Player(float center_x, float center_y) : Player() {
    _box.pos = { center_x - p_width  / 2.0f, center_y - p_height / 2.0f };
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
    if (_state != "hurt") {
        Move(deltatime);
        _weapon->Update(deltatime);
    }

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
    _sprite.Animate(deltatime, _state);
}

void Player::Draw(Graphics *g, Vec2f offset) {
    _weapon->Draw(g, offset);
    SDL_FRect dst = {
        _box.pos.x - offset.x,
        _box.pos.y - offset.y,
        _box.dim.x, _box.dim.y
    };
    if (_state == "hurt") {
        dst.y += 4;
        SDL_SetTextureAlphaMod(_sprite.GetTexture(), 200);
        SDL_SetTextureColorMod(_sprite.GetTexture(), 200, 0, 0);
        _sprite.Draw(g, _state, dst, _flip);
        SDL_SetTextureAlphaMod(_sprite.GetTexture(), 255);
        SDL_SetTextureColorMod(_sprite.GetTexture(), 255, 255, 255);
    } else {
        _sprite.Draw(g, _state, dst, _flip);
    }
}

void Player::Attack() {
    _weapon->Attack();
}
