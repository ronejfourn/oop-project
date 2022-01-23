#include "weapon.h"

class Melee : public Weapon {
    bool _isattacking;
    bool _isonhold;
    bool _facingright;
    Vec2f _htime;
public:
    Melee();
    Melee(Entity * holder, float rad, Weapons name = Weapons::knife);
    void Attack() override;
    void Update(float deltatime) override;
    void PointTowards(Vec2f target) override;
};
