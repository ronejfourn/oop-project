#pragma once
#include "entity.h"

class Weapon;

class Player : public Entity {
    Weapon *_weapon;
public:
    Player();
    Player(float center_x, float center_y);
    void Update(float deltatime) override;
    void Draw(Graphics *g) override;
    void TakeDamage(float damage) override;
    void FaceTowards(Vec2f pos) override;
};
