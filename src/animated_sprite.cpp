#include "headers/animated_sprite.h"

AnimatedSprite::AnimatedSprite() {
    _currentframe = 0;
    _elapsedtime  = 0;
    _frametime    = 1000.0f / 12;
}

AnimatedSprite::~AnimatedSprite() {

}

AnimatedSprite::AnimatedSprite(SDL_Texture *texture) : AnimatedSprite() {
    _texture = texture;
}

AnimatedSprite::AnimatedSprite(Graphics *g, std::string file_path) : AnimatedSprite() {
    _texture = g->LoadImage(file_path.c_str());
}

uint32_t AnimatedSprite::GetFPS() {
    return uint32_t(_frametime) * 1000;
}

void AnimatedSprite::SetFPS(uint32_t fps) {
    _frametime = fps ? 1000.0f / fps : _frametime;
}

void AnimatedSprite::AddAnimation(uint32_t state, int x, int y, int w, int h, uint32_t frameCount, Vec2i offset) {
    _framecount[state] = frameCount;
    _src[state].x = x;
    _src[state].y = y;
    _src[state].w = w;
    _src[state].h = h;
    _off[state] = offset;
}

void AnimatedSprite::Animate(float deltatime, uint32_t state) {
    _elapsedtime += deltatime;
    if (_elapsedtime >= _frametime) {
        _elapsedtime = 0;
        _currentframe ++;
		_currentframe %= _framecount[state];
    }
}

void AnimatedSprite::Draw(Graphics *g, uint32_t state, SDL_FRect &dst, SDL_RendererFlip flip, float angle, SDL_FPoint *center) {
    SDL_Rect src = _src[state];
    src.x += _currentframe * src.w + _off[state].x;
    src.y += _off[state].y;
    src.w -= _off[state].x * 2;
    src.h -= _off[state].y * 2;
    g->DrawTexture(_texture, src, dst, center, angle, flip);
}

void AnimatedSprite::Draw(Graphics *g, uint32_t state, SDL_Rect &dst, SDL_RendererFlip flip, float angle, SDL_Point *center) {
    SDL_Rect src = _src[state];
    src.x += _currentframe * src.w + _off[state].x;
    src.y += _off[state].y;
    src.w -= _off[state].x * 2;
    src.h -= _off[state].y * 2;
    g->DrawTexture(_texture, src, dst, center, angle, flip);
}

void AnimatedSprite::InitBuffer(uint32_t count) {
    _src = new SDL_Rect[count];
    _off = new Vec2i[count];
    _framecount = new uint32_t[count];
}