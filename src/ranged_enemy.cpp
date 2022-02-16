#include "headers/ranged_enemy.h"
#include "headers/projectile.h"

RangedEnemy::RangedEnemy(R_EnemyType type) {
    _limit_speed = true;
    _alive = true;
    _flip  = SDL_FLIP_NONE;
    _state = EntityState::Idle;
    _accn = {0, 0};
    _vel  = {0, 0};
    _sprite.InitBuffer(uint32_t(EntityState::_count));
    switch (type) {
    case R_EnemyType::OrcShaman:
        _hp   = 100.0f;
        _mass = 10.0f;
        _htime = {800, 0};
        _atime = {600, 0};
        _weapon = new Range(this, 25, Weapons::red_magic_staff);
        _sprite.AddAnimation(uint32_t(EntityState::Idle), 368, 236, 16, 20, 4);
        _sprite.AddAnimation(uint32_t(EntityState::Run ), 432, 236, 16, 20, 4);
        _sprite.AddAnimation(uint32_t(EntityState::Hurt), 368, 236, 16, 20, 1);
        break;
    case R_EnemyType::Skelly:
        _hp   = 75.0f;
        _mass = 5.0f;
        _htime = {200, 0};
        _atime = {300, 0};
        _weapon = new Range(this, 25, Weapons::bow);
        _sprite.AddAnimation(uint32_t(EntityState::Idle), 368, 80, 16, 16, 4);
        _sprite.AddAnimation(uint32_t(EntityState::Run ), 432, 80, 16, 16, 4);
        _sprite.AddAnimation(uint32_t(EntityState::Hurt), 368, 80, 16, 16, 1);
        break;
    }
}

void RangedEnemy::Die() {

}

void RangedEnemy::Attack() {

}

void RangedEnemy::Seek(Vec2f pos) {

}

void RangedEnemy::TakeDamage(float damage) {

}

void RangedEnemy::Draw(Graphics *g, Vec2f offset) {

}

void RangedEnemy::Update(Player &player, float deltatime) {

}
