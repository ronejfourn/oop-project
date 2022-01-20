#include "sprite.h"

class Entity;

class Weapon {
private:
    Sprite _sprite;
    Entity *_anchor;
    float _radius, _angle;
    SDL_RendererFlip _flip;
public:
    Weapon();
    Weapon(SDL_Texture *texture, Entity *anchor, float rad);

    void Draw(Graphics *g);
    void PointTowards(SDL_FPoint target);
};
