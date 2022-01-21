#pragma once
#include "utils.h"
class Graphics;
class Entity;
class Player;

class Camera {
private:
    Entity *_anchor;
    Vec2f _offset, _center;
    Graphics *_ginstance;
    Recti _drawArea;
public:
    Camera();
    Camera(Entity *anchor, Recti area);

    Vec2f GetCursorPosition();
    Vec2f GetOffset();

    void SetDrawArea(Recti area);
    void SetAnchor(Entity *anchor);

    void Update();
    void Render(Player &player);
};
