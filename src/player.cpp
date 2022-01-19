#include "player.h"

extern SDL_Texture *singleTexture;

Player::Player() : Entity() {
    _state  = "idle";
    _flip   = false;
    _body   = {0, 0, 48, 84};

    _hp = 100;

    _sprite.SetTexture(singleTexture);
    _sprite.AddAnimation("idle", 128, 100, 16, 28, 4);
    _sprite.AddAnimation("run" , 192, 100, 16, 28, 4);
}

Player::Player(int center_x, int center_y) : Player() {
    _body   = {
        center_x - 48 / 2,
        center_y - 84 / 2,
        48,
        84
    };
}

void Player::Update(float deltatime) {
    Move(deltatime);
}
