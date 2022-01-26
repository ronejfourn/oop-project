#include "headers/player.h"
#include "headers/melee.h"
#include "headers/collision.h"
#include "headers/projectile.h"
#include <vector>
#include <algorithm>

extern SDL_Texture *singleTexture;

namespace {
    const int s_width  = 16;
    const int s_height = 22;
    const float p_width  = s_width  * 2;
    const float p_height = s_height * 2;
}

Player::Player() : Entity() {
    _weapon  = new Range(this, 15, Weapons::bow);
    _state   = EntityState::Idle;
    _box.dim = {p_width, p_height};
    _hp = 100;
    _sprite.SetTexture(singleTexture);
    _sprite.AddAnimation(uint32_t(EntityState::Idle), 128, 106, s_width, s_height, 4);
    _sprite.AddAnimation(uint32_t(EntityState::Run ), 192, 106, s_width, s_height, 4);
    _sprite.AddAnimation(uint32_t(EntityState::Hurt), 256, 102, s_width, s_height, 1);
}

Player::Player(float center_x, float center_y) : Player() {
    _box.pos = { center_x - p_width  / 2.0f, center_y - p_height / 2.0f };
    _box.dim = { p_width, p_height };
}

void Player::TakeDamage(float damage) {
    if (_state != EntityState::Hurt) {
        _hp -= damage;
        _state = EntityState::Hurt;
        if (_hp <= 0)
            _alive = false;
    }
}

void Player::FaceTowards(Vec2f pos) {
    _flip = (pos.x < _box.pos.x + _box.dim.x / 2.0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    _weapon->PointTowards(pos);
}

void Player::Update(float deltatime) {
    Move(deltatime);
    _weapon->Update(deltatime);

    if (_state == EntityState::Hurt) {
        _htime += deltatime;
        if (_htime > _recovertime) {
            _htime = 0;
            _state = EntityState::Idle;
        }
    } else if (_vel.x || _vel.y) {
        _state = EntityState::Run;
    } else {
        _state = EntityState::Idle;
    }
    _sprite.Animate(deltatime, uint32_t(_state));
}

void Player::Draw(Graphics *g, Vec2f offset) {
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
    g->DrawRect(dst, 255, 0, 0);
}

void Player::Attack() {
    _weapon->Attack();
}

bool sort_func_ptr(const std::pair<int, float>& a, const std::pair<int, float>& b) {
	return a.second < b.second;
}

// TODO: move this into room

void Player::Collision(Entity *enemies, Map &map, float deltatime) {
    _box.pos -= _vel * deltatime;

	Vec2f cp, cn;
	float t;

	std::vector<std::pair<int, float>> z;

    Rectf wall_rect[4] = {
        {0, 0, static_cast<float>(map.dim.x * map.drawsize), static_cast<float>(map.drawsize)},
        {0, 0, static_cast<float>(map.drawsize), static_cast<float>(map.dim.y * map.drawsize)},
        {0, static_cast<float>((map.dim.y - 1) * map.drawsize), static_cast<float>(map.dim.x * map.drawsize), static_cast<float>(map.drawsize)},
        {static_cast<float>((map.dim.x - 1) * map.drawsize), 0, static_cast<float>(map.drawsize), static_cast<float>(map.dim.y * map.drawsize)},
    };

    int steps = int(ceil(_vel.magnitude() * deltatime / map.drawsize));

    if (steps > 0) {
        float st = deltatime / steps;
        while (steps > 0) {
            for (int i = 0; i < 4; i++)
                if (Collision::DynamicRectVsRect(&_box, _vel, &wall_rect[i], cp, cn, t, st))
                    z.push_back({i, t});

            std::sort(z.begin(), z.end(), sort_func_ptr);

            for (int i = 0; i < z.size(); i++)
                if (Collision::DynamicRectVsRect(&_box, _vel, &wall_rect[z[i].first], cp, cn, t, st))
                    _vel += cn * Vec2f(ut_abs(_vel.x), ut_abs(_vel.y)) * (1 - t);
            _box.pos += _vel * st;
            z.clear();
            steps --;
        }
    } else {
        for (int i = 0; i < 4; i++)
            if (Collision::DynamicRectVsRect(&_box, _vel, &wall_rect[i], cp, cn, t, deltatime))
                z.push_back({i, t});

        std::sort(z.begin(), z.end(), sort_func_ptr);

        for (int i = 0; i < z.size(); i++)
            if (Collision::DynamicRectVsRect(&_box, _vel, &wall_rect[z[i].first], cp, cn, t, deltatime))
                _vel += cn * Vec2f(ut_abs(_vel.x), ut_abs(_vel.y)) * (1 - t);
        _box.pos += _vel * deltatime;
    }
}
