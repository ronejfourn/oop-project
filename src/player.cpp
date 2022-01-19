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

Player::Player(int x, int y) : Player() {
    _body   = {0, 0, 48, 84};
}

void Player::Update(float deltatime) {
    Move(deltatime);
}
