#include "entity.h"

class Player : public Entity {
public:
    Player();
    Player(float center_x, float center_y);
    void Update(float deltatime) override;
};
