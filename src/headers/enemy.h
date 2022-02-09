#include "entity.h"

class Weapon;
class Player;

class Enemy : public Entity {
public:
    virtual void Die() = 0;
    virtual void Attack() = 0;
    virtual void Seek(Vec2f pos) = 0;
    virtual void TakeDamage(float damage) = 0;
    virtual void Draw(Graphics *g, Vec2f offset) = 0;
    virtual void Update(Player &player, float deltatime) = 0;
	virtual void Collision(Player &player, Map&map, float deltatime) = 0;
};
