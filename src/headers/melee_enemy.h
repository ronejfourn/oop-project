#include "enemy.h"
/* masked_orc_idle_anim 368 172 16 20 4 */
/* masked_orc_run_anim 432 172 16 20 4 */
/* orc_warrior_idle_anim 368 204 16 20 4 */
/* orc_warrior_run_anim 432 204 16 20 4 */
/* ogre_idle_anim  16 320 32 32 4 */
/* ogre_run_anim 144 320 32 32 4 */

enum class M_EnemyType {
    MaskedOrc, OrcWarrior, Ogre,
};

class Melee;
class Player;

class MeleeEnemy : public Enemy {
private:
    Melee *_weapon;
    Vec2f _atime;
    bool _atkoncd, _pause;
public:
    MeleeEnemy(M_EnemyType type);
    void Die() override;
    void Attack() override;
    void Seek(Vec2f pos) override;
    void TakeDamage(float damage) override;
    void Draw(Graphics *g, Vec2f offset) override;
    void Update(Player &player, float deltatime) override;
    void Collision(Player &player, Map &map, float deltatime) override;
};
/* muddy_idle_anim 368 112 16 16 4 */
/* muddy_run_anim 368 112 16 16 4 */
/* chort_idle_anim 368 328 16 24 4 */
/* chort_run_anim 432 328 16 24 4 */
/* big_demon_idle_anim  16 364 32 36 4 */
/* big_demon_run_anim 144 364 32 36 4 */
