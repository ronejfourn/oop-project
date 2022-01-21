#include "headers/graphics.h"
#include "headers/player.h"
#include "headers/camera.h"
#include "headers/utils.h"

Camera::Camera() {
    _anchor = nullptr;
    _ginstance = Graphics::GetInstance();
}

Camera::Camera(Entity *anchor, Recti area) {
    _anchor    = anchor;
    _ginstance = Graphics::GetInstance();
    _drawArea  = area;
    _center    = {
        static_cast<float>(area.pos.x + area.dim.x / 2.0),
        static_cast<float>(area.pos.y + area.dim.y / 2.0),
    };
}

Vec2f Camera::GetCursorPosition() {
    return _ginstance->GetCursorPosition() + _offset;
}

Vec2f Camera::GetOffset() {
    return _offset;
}

void Camera::SetDrawArea(Recti area) {
    _drawArea = area;
    _center    = {
        static_cast<float>(area.pos.x + area.dim.x / 2.0),
        static_cast<float>(area.pos.y + area.dim.y / 2.0),
    };
}

void Camera::SetAnchor(Entity *anchor) {
    _anchor = anchor;
}

void Camera::Update() {
    _offset = _anchor->GetCenter() - _center;
}

void Camera::Render(Player &player) {
    player.Draw(_ginstance, _offset);
}
