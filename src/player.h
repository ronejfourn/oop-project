#include "entity.h"

class Player : public Entity {
public:
    Player();
    Player(int center_x, int center_y);
    void Update(float deltatime) override;
};
