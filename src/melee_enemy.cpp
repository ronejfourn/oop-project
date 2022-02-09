#include "headers/collision.h"
#include "headers/entity.h"
#include "headers/player.h"
#include "headers/melee_enemy.h"
#include "headers/melee.h"

extern SDL_Texture *singleTexture;

MeleeEnemy::MeleeEnemy(M_EnemyType type) {
    _limit_speed = true;
    _alive = true;
    _flip  = SDL_FLIP_NONE;
    _state = EntityState::Idle;
    _accn = {0, 0};
    _vel  = {0, 0};
    _sprite.SetTexture(singleTexture);
    _sprite.InitBuffer(uint32_t(EntityState::_count));
    _box.pos = {100, 100};
    switch (type) {
    case M_EnemyType::MaskedOrc:
        _hp = 80.0f;
        _mass = 15.0f;
        _htime = { 100, 0 };
        _atime = { 1300, 0 };
        _weapon = new Melee(this, 25, Weapons::cleaver);
        _sprite.AddAnimation(uint32_t(EntityState::Idle), 368, 172, 16, 20, 4);
        _sprite.AddAnimation(uint32_t(EntityState::Run), 432, 172, 16, 20, 4);
        _sprite.AddAnimation(uint32_t(EntityState::Hurt), 368, 172, 16, 20, 1);
        _box.dim = { 16 * 2, 20 * 2 };
        break;
    case M_EnemyType::Ogre:
        _hp = 200.0f;
        _mass = 25.0f;
        _htime = { 600, 0 };
        _atime = { 2500, 0 };
        _weapon = new Melee(this, 40, Weapons::baton_with_spikes);
        _sprite.AddAnimation(uint32_t(EntityState::Idle), 16 , 324, 32, 28, 4, { 6, 0 });
        _sprite.AddAnimation(uint32_t(EntityState::Run) , 144, 324, 32, 28, 4, { 6, 0 });
        _sprite.AddAnimation(uint32_t(EntityState::Hurt), 16 , 324, 32, 28, 4, { 6, 0 });
        _box.dim = {20 * 2, 28 * 2};
        break;
    default:
        _hp = 120.0f;
        _mass = 15.0f;
        _htime = {400, 0};
        _atime = {600, 0};
        _weapon = new Melee(this, 25, Weapons::machete);
        _sprite.AddAnimation(uint32_t(EntityState::Idle), 368, 204, 16, 20, 4);
        _sprite.AddAnimation(uint32_t(EntityState::Run ), 432, 204, 16, 20, 4);
        _sprite.AddAnimation(uint32_t(EntityState::Hurt), 368, 204, 16, 20, 1);
        _box.dim = { 16 * 2, 20 * 2 };
        break;
    }
}

void MeleeEnemy::Die() {

}

void MeleeEnemy::Attack() {
    if (!_atkoncd) {
        _weapon->Attack();
        _atkoncd = true;
    }
}

void MeleeEnemy::Seek(Vec2f pos) {
	if (_state == EntityState::Hurt) return;
    _flip = (pos.x < _box.pos.x + _box.dim.x / 2.0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    _weapon->PointTowards(pos);
	Vec2f disp = pos - GetCenter();
	float dist = disp.x * disp.x + disp.y * disp.y;
	if (!_atkoncd && dist <= 1600000) {
		if (ut_abs(disp.x) > _weapon->GetRange()) {
			if (disp.x < 0) {
				_accn.x -= 0.003;
			} else if (disp.x > 0) {
				_accn.x += 0.003;
			}
		}
		if (ut_abs(disp.y) > _weapon->GetRange()) {
			if (disp.y < 0) {
				_accn.y -= 0.003;
			} else if (disp.y > 0) {
				_accn.y += 0.003;
			}
		}
	} else if (!_pause) {
        _pause = dist >= 4 * _weapon->GetRange() * _weapon->GetRange() && !_pause;
		if (disp.x < 0) {
			_accn.x += 0.002;
		} else if (disp.x > 0) {
			_accn.x -= 0.002;
		}
		if (disp.y < 0) {
			_accn.y += 0.002;
		} else if (disp.y > 0) {
			_accn.y -= 0.002;
		}
	}
}

void MeleeEnemy::TakeDamage(float damage) {
	_hp -= damage;
	_atime.y = 0;
    _pause = false;
	_state = EntityState::Hurt;
	if (_hp <= 0)
		_alive = false;
}

void MeleeEnemy::Draw(Graphics *g, Vec2f offset) {
    Rectf b = _weapon->GetBox();
    bool ontop = b.pos.y + b.dim.y / 2 > _box.pos.y + _box.dim.y / 2;
    if (!ontop)
        _weapon->Draw(g, offset);
    SDL_FRect dst = {
        _box.pos.x - offset.x,
        _box.pos.y - offset.y,
        _box.dim.x, _box.dim.y
    };
    if (_state == EntityState::Hurt) {
        dst.y -= 4;
        SDL_SetTextureColorMod(_sprite.GetTexture(), 255, 0, 0);
        _sprite.Draw(g, uint32_t(_state), dst, _flip);
        SDL_SetTextureColorMod(_sprite.GetTexture(), 255, 255, 255);
    } else {
        _sprite.Draw(g, uint32_t(_state), dst, _flip);
    }
    if (ontop)
        _weapon->Draw(g, offset);
	SDL_FRect a = {
		_box.pos.x - offset.x,
		_box.pos.y - offset.y,
		_box.dim.x,
		_box.dim.y
	};
}

void MeleeEnemy::Update(Player &player, float deltatime) {
	if (!_weapon->IsAttacking()) {
		Vec2f pcenter = player.GetCenter();
		Seek(pcenter);
		Vec2f dispvec = pcenter - GetCenter();
		if (Collision::CircleVsRect(player.GetBox(), GetCenter(), _weapon->GetRange()))
			Attack();
	}

	Accelerate(deltatime);
	_weapon->Update(deltatime);

    if (_atkoncd) {
        _atime.y += deltatime;
        if (_atime.y > _atime.x) {
            _atime.y = 0;
            _atkoncd = false;
            _pause = false;
        }
    }

    if (_state == EntityState::Hurt) {
        _htime.y += deltatime;
        if (_htime.y >= _htime.x) {
            _htime.y = 0;
            _state = EntityState::Idle;
        }
    } else if (_vel.x || _vel.y) {
        _state = EntityState::Run;
    } else {
        _state = EntityState::Idle;
    }
    _sprite.Animate(deltatime, uint32_t(_state));
}

void MeleeEnemy::Collision(Player &player, Map &map, float deltatime) {
	CollideAgainstMap(map, deltatime);
    _weapon->UpdatePosition();
	if (_weapon->Collision(&player)) {
		player.TakeDamage(2);
	}
}
