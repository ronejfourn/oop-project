#include "headers/animated_sprite.h"

AnimatedSprite::AnimatedSprite() {
    _currentframe = 0;
    _elapsedtime  = 0;
    _frametime    = 1000.0 / 12;
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
    return _frametime * 1000;
}

void AnimatedSprite::SetFPS(uint32_t fps) {
    _frametime = fps ? 1000.0 / fps : _frametime;
}

void AnimatedSprite::AddAnimation(std::string state, int x, int y, int w, int h, uint32_t frameCount, uint32_t fps) {
    _framecount[state] = frameCount;
    _src[state].x = x;
    _src[state].y = y;
    _src[state].w = w;
    _src[state].h = h;
    _frametime = 1000.0 / (fps ? fps : 12);
}

void AnimatedSprite::Draw(Graphics *g, std::string state, SDL_FRect &dst, SDL_RendererFlip flip, float angle, SDL_FPoint *center) {
    _currentframe %= _framecount[state];
    SDL_Rect src = _src[state];
    src.x += _currentframe * src.w;
    g->DrawTexture(_texture, src, dst, center, angle, flip);
    _elapsedtime += g->GetDeltaTime();
    if (_elapsedtime >= _frametime) {
        _elapsedtime = 0;
        _currentframe ++;
    }
}

void AnimatedSprite::Draw(Graphics *g, std::string state, SDL_Rect &dst, SDL_RendererFlip flip, float angle, SDL_Point *center) {
    SDL_Rect src = _src[state];
    src.x += _currentframe * src.w;
    g->DrawTexture(_texture, src, dst, center, angle, flip);
    _elapsedtime += g->GetDeltaTime();
    if (_elapsedtime >= _frametime) {
        _elapsedtime = 0;
        _currentframe = (_currentframe + 1) % _framecount[state];
    }
}
