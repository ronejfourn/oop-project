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

void Camera::Render(Player &player, Map &map) {
    Vec2f ppos  = player.GetCenter();
    Vec2i tl = {
        (int(ppos.x) - _drawArea.dim.x / 2) / map.drawsize,
        (int(ppos.y) - _drawArea.dim.y / 2) / map.drawsize,
    };
    Vec2i len = {
        int(_drawArea.dim.x / float(map.drawsize)) + 1,
        int(_drawArea.dim.y / float(map.drawsize)) + 2,
    };
    for (int x = tl.x < 0 ? 0 : tl.x; x < ((tl.x + len.x) < map.dim.x ? (tl.x + len.x) : map.dim.x); x ++) {
        for (int y = tl.y < 0 ? 0 : tl.y; y < ((tl.y + len.y) < map.dim.y ? (tl.y + len.y) : map.dim.y); y ++) {
            int i = abs(map.indices[x * map.dim.y + y]) - 1;
            SDL_Rect  src = {
                map.tilemap[i].x, map.tilemap[i].y,
                map.tilesize    , map.tilesize
            };
            SDL_FRect dst = {
                x * map.drawsize - _offset.x,
                y * map.drawsize - _offset.y,
                float(map.drawsize),
                float(map.drawsize)
            };
            _ginstance->DrawTexture(map.texture, src, dst);
        }
    }
    player.Draw(_ginstance, _offset);
}
