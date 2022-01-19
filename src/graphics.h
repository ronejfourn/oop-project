#pragma once
#include <cstdint>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Color;
struct SDL_Point;
struct SDL_Rect;
struct SDL_Texture;

class Graphics {
private:
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    uint32_t _wwidth, _wheight, _dbegin;
    int _offx, _offy;
    float _dt;
public:
    Graphics();
    ~Graphics();

    SDL_Renderer *GetRenderer();

    void Clear();
    void Update();

    SDL_Point GetCurrentResolution();
    SDL_Point GetCursorPosition();

    void SetTitle(const char *title, const char *icon_file = nullptr);
    void SetTargetFPS(uint32_t fps);

    float GetDeltaTime();

    void DrawRect(int tl_x, int tl_y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, bool offs = true);
    void DrawRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, bool offs = true);
    void FillRect(int tl_x, int tl_y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, bool offs = true);
    void FillRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, bool offs = true);
    void DrawPoint(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, bool offs = true);
    void DrawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, uint32_t t = 1, bool offs = true);
    void DrawTexture(SDL_Texture *texture, SDL_Rect &src, SDL_Rect &dst, bool offs = true, SDL_Point *center = nullptr, float angle = 0, bool flip = false);

    SDL_Texture *LoadImage(const char *image_path);
};
