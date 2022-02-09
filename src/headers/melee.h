#include "weapon.h"

enum {
    Attacking = 0x1,
    OnHold = 0x2,
    FacingRight = 0x4,
};

class Melee : public Weapon {
    uint8_t _state;
    float _range, _vel;
    float _htime, _atime;
public:
    Melee(Entity * holder, float rad, Weapons name);
	bool IsAttacking() {return _state &Attacking;}
    float GetRange() {return _range;}
    void Attack() override;
    void Draw(Graphics *g, Vec2f offset) override;
    void Update(float deltatime) override;
    void PointTowards(Vec2f target) override;
    bool Collision(Entity *op) override;
	void EndAttack() override;
};
