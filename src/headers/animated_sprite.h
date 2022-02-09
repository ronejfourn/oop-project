#include "sprite.h"

class AnimatedSprite : public Sprite {
private:
    uint32_t _currentframe;
    float _elapsedtime;
    float _frametime;
    uint32_t *_framecount;
public:
    AnimatedSprite();
    AnimatedSprite(SDL_Texture *texture);
    AnimatedSprite(Graphics *g, std::string file_path);
    ~AnimatedSprite();

    uint32_t GetFPS();

    void SetFPS(uint32_t fps);
    void InitBuffer(uint32_t count) override;
    void AddAnimation(uint32_t state, int x, int y, int w, int h, uint32_t frameCount = 1, Vec2i offset = {0, 0});

    void Animate(float deltatime, uint32_t state);
    void Draw(Graphics *g, uint32_t state, SDL_FRect &dst, SDL_RendererFlip flip, float angle = 0, SDL_FPoint *center = NULL) override;
    void Draw(Graphics *g, uint32_t state, SDL_Rect &dst , SDL_RendererFlip flip, float angle = 0, SDL_Point  *center = NULL) override;
};