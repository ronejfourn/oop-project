#include "utils.h"

namespace Collision {
    bool RayVsRect (Rectf rect, Vec2f center, Vec2f dir, float radius);
    bool RectVsRect(Rectf rect1, Rectf rect2);
    bool CircleVsRect(Rectf rect, Vec2f center, float radius);
    bool RayVsRect(const Vec2f& origin, const Vec2f& dirn, const Rectf* rect, Vec2f& contact_point, Vec2f& contact_normal,float& t_hit_near);
    bool DynamicRectVsRect(const Rectf* rect1, const Vec2f& vel, const Rectf* rect2, Vec2f& contact_point, Vec2f& contact_normal, float& contact_time, const float& dt);
}
