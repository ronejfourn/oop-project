#include "weapon.h"
#include "entity.h"
#include <stdint.h>

class Range : public Weapon {
    bool _isbow;
    SDL_Rect _projectilesrc;
    uint8_t _count;
    Vec2f _pos[256], _target;
    float _vel, _ang[256];
public:
    Range();
    Range(Entity * holder, float rad, Weapons name);
    void Attack() override;
    void Update(float deltatime) override;
    void PointTowards(Vec2f target) override;
    void Draw(Graphics *g, Vec2f offset) override;
};
