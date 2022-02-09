#include "headers/collision.h"
#include <math.h>

bool Collision::CircleVsRect(Rectf rect, Vec2f center, float radius) {
    Vec2f rectCenter = rect.pos + rect.dim / 2;
    Vec2f circleDiff = {
        ut_abs(center.x - rectCenter.x),
        ut_abs(center.y - rectCenter.y),
    };
    if (circleDiff.x  > rect.dim.x / 2 + radius) return false;
    if (circleDiff.y  > rect.dim.y / 2 + radius) return false;
    if (circleDiff.x <= rect.dim.x / 2 + radius && circleDiff.y <= rect.dim.y / 2) return true ;
    if (circleDiff.y <= rect.dim.y / 2 + radius && circleDiff.x <= rect.dim.x / 2) return true ;

    float corner = (circleDiff.x - rect.dim.x / 2) * (circleDiff.x - rect.dim.x / 2) +
                   (circleDiff.y - rect.dim.y / 2) * (circleDiff.y - rect.dim.y / 2);
    return corner <= radius * radius;
}

bool Collision::RayVsRect(Rectf rect, Vec2f origin, Vec2f dir, float length) {
    if (!CircleVsRect(rect, origin, length) && length != 0) return false;
	Vec2f t_near = (rect.pos - origin) / (dir * length);
	Vec2f t_far  = (rect.pos + rect.dim - origin) / (dir * length);

	if(std::isnan(t_near.y) || std::isnan(t_near.x)) return false;
	if(std::isnan(t_far.y ) || std::isnan(t_far.x )) return false;

	if(t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if(t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	if(t_near.x > t_far.y || t_near.y > t_far.x) return false;

	float t_hit_far  = (t_far.x  < t_far.y ) ? t_far.x  : t_far.y ;
	if(t_hit_far < 0) return false;

	return true;
}

bool Collision::RayVsRect(const Vec2f& origin, const Vec2f& dirn, const Rectf* rect, Vec2f& contact_point, Vec2f& contact_normal,float& t_hit_near) {
	contact_point = {0,0};
	contact_normal = {0,0};

	Vec2f t_near = (rect->pos - origin) / dirn;
	Vec2f t_far = (rect->pos + rect->dim - origin) / dirn;

	if(std::isnan(t_near.y) || std::isnan(t_near.x)) return false;
	if(std::isnan(t_far.y) || std::isnan(t_far.x)) return false;

	if(t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if(t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	if(t_near.x > t_far.y || t_near.y > t_far.x) return false;

	t_hit_near = (t_near.x > t_near.y) ? t_near.x : t_near.y;
	float t_hit_far  = (t_far.x  < t_far.y ) ? t_far.x  : t_far.y ;

	if(t_hit_far < 0) return false;

	contact_point = origin + dirn * t_hit_near;

	if(t_near.x > t_near.y) {
		if(dirn.x < 0)
			contact_normal = {1, 0};
		else
			contact_normal = {-1, 0};
	} else if( t_near.x < t_near.y) {
		if(dirn.y < 0)
			contact_normal = {0, 1};
		else
			contact_normal = {0, -1};
	}
	return true;
}

bool Collision::DynamicRectVsRect(const Rectf* rect1, const Vec2f& vel, const Rectf* rect2, Vec2f& contact_point, Vec2f& contact_normal, float& contact_time, const float& dt) {
	if(vel.x == 0 && vel.y == 0) return false;

	Rectf expanded_rect;
    expanded_rect.pos = rect2->pos - rect1->dim / 2;
    expanded_rect.dim = rect2->dim + rect1->dim;

	return (RayVsRect(rect1->pos  + rect1->dim / 2, vel * dt, &expanded_rect, contact_point, contact_normal, contact_time) && contact_time >= -1.0f && contact_time <= 1.0f);

}

bool Collision::RectVsRect(Rectf rect1, Rectf rect2) {
    return rect1.pos.x <= rect2.pos.x + rect2.dim.x && rect1.pos.x + rect1.dim.x >= rect2.pos.x
        && rect1.pos.y <= rect2.pos.y + rect2.dim.y && rect1.pos.y + rect1.dim.y >= rect2.pos.y;
}

