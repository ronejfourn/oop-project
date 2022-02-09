#include "weapon.h"
#include <vector>

struct Projectile {
    Vec2f pos, vel;
    float ang, life;
};

class Range : public Weapon {
    bool _isbow;
    SDL_Rect _projectilesrc;
    std::vector<Projectile> _projectiles;
public:
    Range(Entity * holder, float rad, Weapons name);
    void Attack() override;
    void Update(float deltatime) override;
    void PointTowards(Vec2f target) override;
    void Draw(Graphics *g, Vec2f offset) override;
    bool Collision (Entity *) override {return false;}
};
