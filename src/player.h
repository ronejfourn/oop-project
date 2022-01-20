#pragma once
#include "entity.h"

class Player : public Entity {
public:
    Player();
    Player(float center_x, float center_y);
    void Update(float deltatime) override;
    void Draw(Graphics *g) override;
    void TakeDamage(float damage) override;
};
