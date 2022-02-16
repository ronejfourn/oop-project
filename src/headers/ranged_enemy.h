#include "enemy.h"
/* orc_shaman_idle_anim 368 236 16 20 4 */
/* orc_shaman_run_anim 432 236 16 20 4 */
/* skelet_idle_anim 368 80 16 16 4 */
/* skelet_run_anim 432 80 16 16 4 */

enum class R_EnemyType {
    OrcShaman, Skelly
};

class Weapon;
class Player;

class RangedEnemy : public Enemy {
private:
    Weapon *_weapon;
    Vec2f _atime;
public:
    RangedEnemy(R_EnemyType type);
    void Die() override;
    void Attack() override;
    void Seek(Vec2f pos) override;
    void TakeDamage(float damage) override;
    void Draw(Graphics *g, Vec2f offset) override;
    void Update(Player &player, float deltatime) override;
};
