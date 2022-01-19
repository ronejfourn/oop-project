#pragma once
#include <cstdint>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Color;
struct SDL_Point;
struct SDL_FPoint;
struct SDL_Rect;
struct SDL_FRect;
struct SDL_Texture;

class Graphics {
    static Graphics *_instance;
private:
    SDL_Window *_window;
    SDL_Renderer *_renderer;
    uint32_t _wwidth, _wheight, _dbegin;
    float _offx, _offy;
    float _dt, _ft;

    Graphics();
public:
    static Graphics *GetInstance();

    SDL_Renderer *GetRenderer();

    void Clear();
    void Update();

    SDL_Point GetCurrentResolution();
    SDL_FPoint GetCursorPosition(bool offseted = false);

    void SetTitle(const char *title, const char *icon_file = nullptr);
    void SetTargetFPS(uint32_t fps);
    void SetOffset(SDL_FPoint &offset);
    void SetOffset(float ox, float oy);

    float GetDeltaTime();

    void DrawRect(float tl_x, float tl_y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, bool offs = true);
    void FillRect(float tl_x, float tl_y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, bool offs = true);
    void DrawRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    void FillRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    void DrawRect(SDL_FRect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, bool offs = true);
    void FillRect(SDL_FRect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, bool offs = true);

    void DrawPoint(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, bool offs = true);
    void DrawLine(float x1, float y1, float x2, float y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255, uint32_t t = 1, bool offs = true);

    void DrawTexture(SDL_Texture *texture, SDL_Rect &src, SDL_Rect &dst , SDL_Point *center = nullptr , float angle = 0, bool flip = false);
    void DrawTexture(SDL_Texture *texture, SDL_Rect &src, SDL_FRect &dst, bool offs = true, SDL_FPoint *center = nullptr, float angle = 0, bool flip = false);

    SDL_Texture *LoadImage(const char *image_path);
    SDL_Texture *CreateTexture(int width, int height);
    void BindTexture(SDL_Texture *texture);
};
