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
}

Player::Player(float center_x, float center_y) : Player() {
    _body   = {
        center_x - p_width  / 2,
        center_y - p_height / 2,
        p_width,
        p_height
    };
}

void Player::Update(float deltatime) {
    Move(deltatime);
}
