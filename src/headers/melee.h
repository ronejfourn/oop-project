#include "weapon.h"

class Melee : public Weapon {
    bool _isattacking;
    bool _isonhold;
    float _range, _vel, _accn;
    bool _facingright;
    Vec2f _htime, _atime;
public:
    Melee();
    Melee(Entity * holder, float rad, Weapons name = Weapons::knife);
    void Attack() override;
    void Draw(Graphics *g, Vec2f offset) override;
    void Update(float deltatime) override;
    void PointTowards(Vec2f target) override;
    bool Collision(Entity *op) override;
};
