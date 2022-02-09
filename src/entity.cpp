#include "headers/entity.h"
#include "headers/collision.h"
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

namespace {
    const float friction_constant = 0.0015f;
    const float maxspd  = 0.2f;
    const float maxdspd = maxspd * Q_rsqrt(2);
}

Entity::Entity() {
    _limit_speed = true;
    _alive = true;
    _flip  = SDL_FLIP_NONE;
    _state = EntityState::Idle;
    _accn = {0, 0};
    _vel  = {0, 0};
    _sprite.InitBuffer(uint32_t(EntityState::_count));
}

void Entity::AddForce(Vec2f force) {
    _limit_speed = false;
    _accn += force / _mass;
}

void Entity::Accelerate(float deltatime) {
    _vel += _accn * deltatime;
    _accn.zero();

    float friction = friction_constant * deltatime;
    if (_vel.x != 0) {
        int sx = _vel.x < 0 ? -1 : 1;
        _vel.x -= friction > sx * _vel.x ? _vel.x : sx * friction;
    }
    if (_vel.y != 0) {
        int sy = _vel.y < 0 ? -1 : 1;
        _vel.y -= friction > sy * _vel.y ? _vel.y : sy * friction;
    }

    if (_limit_speed) {
        if (!_vel.y && _vel.x) {
            _vel.x = ut_clamp(_vel.x, -maxspd, maxspd);
        } else if (!_vel.x && _vel.y) {
            _vel.y = ut_clamp(_vel.y, -maxspd, maxspd);
        } else if (_vel.x && _vel.y) {
            _vel.x = ut_clamp(_vel.x, -maxdspd, maxdspd);
            _vel.y = ut_clamp(_vel.y, -maxdspd, maxdspd);
        }
    } else
        _limit_speed = true;
}

bool sort_func_ptr(const std::pair<int, float>& a, const std::pair<int, float>& b) {
	return a.second < b.second;
}

//TODO get this sorted
void Entity::CollideAgainstMap(Map &map, float deltatime) {
	Vec2f cp, cn;
	float t;

    Rectf wall_rect[4] = {
        {0, 0, float(map.dim.x * map.drawsize), float(map.drawsize)},
        {0, 0, float(map.drawsize), float(map.dim.y * map.drawsize)},
        {0, float((map.dim.y - 1) * map.drawsize), float(map.dim.x * map.drawsize), float(map.drawsize)},
        {float((map.dim.x - 1) * map.drawsize), 0, float(map.drawsize), float(map.dim.y * map.drawsize)},
    };

    float steps = (ut_abs(_vel.x) + ut_abs(_vel.y)) * deltatime / map.drawsize;
	if (steps > 1) {
		float st    = deltatime / steps;
		while (steps > 0) {
			std::vector<std::pair<int, float>> z;
			for (int i = 0; i < 4; i++) {
				bool col = Collision::DynamicRectVsRect(&_box, _vel, &wall_rect[i], cp, cn, t, st);
				if (col)
					z.push_back({i, t});
			}

			std::sort(z.begin(), z.end(), sort_func_ptr);

			for (int i = 0; i < z.size(); i++)
				if (Collision::DynamicRectVsRect(&_box, _vel, &wall_rect[z[i].first], cp, cn, t, st))
					_vel += cn * Vec2f(ut_abs(_vel.x), ut_abs(_vel.y)) * (1 - t);

			_box.pos += _vel * st;
			steps --;
		}
	} else {
		std::vector<std::pair<int, float>> z;
		for (int i = 0; i < 4; i++) {
			bool col = Collision::DynamicRectVsRect(&_box, _vel, &wall_rect[i], cp, cn, t, deltatime);
			if (col)
				z.push_back({i, t});
		}

		std::sort(z.begin(), z.end(), sort_func_ptr);

		for (int i = 0; i < z.size(); i++)
			if (Collision::DynamicRectVsRect(&_box, _vel, &wall_rect[z[i].first], cp, cn, t, deltatime))
				_vel += cn * Vec2f(ut_abs(_vel.x), ut_abs(_vel.y)) * (1 - t);

		_box.pos += _vel * deltatime;
		steps --;
	}
}
