#include "entity.h"

class Player : public Entity {
public:
    Player();
    Player(int x, int y);
    void Update(float deltatime) override;
};
