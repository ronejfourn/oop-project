#include "headers/graphics.h"
#include "SDL2/SDL_hints.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_mouse.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h"

//TODO: move mouse elsewhere

Graphics *Graphics::_instance = nullptr;

Graphics *Graphics::GetInstance() {
    if (!_instance) _instance = new Graphics;
    return _instance;
}

Graphics::Graphics() {
    _wdim = {1280, 720};

    _dt = 1000.0f / 60;
    _ft = 1000.0f / 60;
    _dbegin = 0;

	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    _window = SDL_CreateWindow(
        "Unnamed Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        _wdim.x, _wdim.y,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (_window == NULL)
        Logger::LogError("Failed to create SDL Window");

    _renderer = SDL_CreateRenderer(
        _window,
        -1,
        SDL_RENDERER_ACCELERATED);

    if (_renderer == NULL)
        Logger::LogError("Failed to create SDL Renderer");

    SDL_RenderSetLogicalSize(_renderer, _wdim.x, _wdim.y);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
}

SDL_Renderer *Graphics::GetRenderer() {
    return _renderer;
}

void Graphics::Clear() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void Graphics::Update() {
    SDL_RenderPresent(_renderer);
    uint32_t a = _dbegin;

    if ((SDL_GetTicks() - _dbegin) < _ft)
        SDL_Delay(_ft - (SDL_GetTicks() - _dbegin));

    _dbegin = SDL_GetTicks();
    _dt = _dbegin - a;
}

void Graphics::SetTitle(const char *title, const char *icon_file) {
    SDL_SetWindowTitle(_window, title);

    if (icon_file != nullptr) {
        SDL_Surface *icon = SDL_LoadBMP(icon_file);
        if (!icon) Logger::LogWarning("Icon not Found");
        SDL_SetWindowIcon(_window, icon);
    }
}

void Graphics::SetTargetFPS(uint32_t fps) {
    if (fps == 0) {
        Logger::LogWarning("FPS cannot be zero");
        fps = 60;
    }
    _ft = 1000.0f / fps;
}

void Graphics::DrawRect(float tl_x, float tl_y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_FRect tmp = {
        tl_x, tl_y,
        width, height
    };
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderDrawRectF(_renderer, &tmp);
}

void Graphics::DrawRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderDrawRect(_renderer, &rect);
}

void Graphics::DrawRect(SDL_FRect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderDrawRectF(_renderer, &rect);
}

void Graphics::FillRect(float tl_x, float tl_y, float width, float height, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_FRect tmp = {
        tl_x, tl_y,
        width, height
    };
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderFillRectF(_renderer, &tmp);
}

void Graphics::FillRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderFillRect(_renderer, &rect);
}

void Graphics::FillRect(SDL_FRect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderFillRectF(_renderer, &rect);
}

void Graphics::DrawPoint(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderDrawPointF(_renderer, x, y);
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint32_t t) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    if (t <= 1) {
        SDL_RenderDrawLineF(_renderer, x1, y1, x2, y2);
    } else {
        float m = - float(x2 - x1) / (y2 - y1);
        float mulx = 1 / sqrt(1 + m * m);
        float muly = m * mulx;
        for (uint32_t i = 0; i < t * 2; i ++) {
            float adx = mulx * i / 2;
            float ady = muly * i / 2;
            SDL_RenderDrawLineF(_renderer, x1 + adx, y1 + ady, x2 + adx, y2 + ady);
            SDL_RenderDrawLineF(_renderer, x1 - adx, y1 - ady, x2 - adx, y2 - ady);
        }
    }
}

void Graphics::DrawTexture(SDL_Texture *texture, SDL_Rect &src, SDL_Rect &dst, SDL_Point *center, float angle, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(_renderer, texture, &src, &dst, angle, center, flip);
}

void Graphics::DrawTexture(SDL_Texture *texture, SDL_Rect &src, SDL_FRect &dst, SDL_FPoint *center, float angle, SDL_RendererFlip flip) {
    SDL_RenderCopyExF(_renderer, texture, &src, &dst, angle, center, flip);
}

float Graphics::GetDeltaTime() {return _dt;}

Vec2i Graphics::GetLogicalResolution() {return _wdim;}

Vec2i Graphics::GetCurrentResolution() {
    Vec2i ret;
    SDL_GetWindowSize(_window, &ret.x, &ret.y);
    return ret;
}

Vec2f Graphics::GetCursorPosition() {
    Vec2i mouse, res;
    SDL_GetMouseState(&mouse.x, &mouse.y);
    res = GetCurrentResolution();
    return {
        mouse.x * _wdim.x / float(res.x),
        mouse.y * _wdim.y / float(res.y)
    };
}

/*
 * yoinked from: https://wiki.libsdl.org/SDL_CreateRGBSurfaceFrom
 */

SDL_Texture *Graphics::LoadImage(const char *image_path) {
    int req_format = STBI_rgb_alpha;
    int width, height, orig_format;
    uint8_t* data = stbi_load(image_path, &width, &height, &orig_format, req_format);
    if (data == NULL) {
        Logger::LogError(stbi_failure_reason());
    }

    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    int depth, pitch;
    depth = 32;
    pitch = 4*width;

    SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)data, width, height, depth, pitch,
                                                rmask, gmask, bmask, amask);

    if (surf == NULL) {
        Logger::LogError("Could not create surface");
    }

    SDL_Texture *ret = SDL_CreateTextureFromSurface(_renderer, surf);

    stbi_image_free(data);
    SDL_FreeSurface(surf);

    return ret;
}

SDL_Texture *Graphics::CreateTexture(int width, int height) {
    SDL_Texture *ret = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, width, height);
    SDL_SetTextureBlendMode(ret, SDL_BLENDMODE_BLEND);
    return ret;
}

void Graphics::BindTexture(SDL_Texture *texture) {
    SDL_SetRenderTarget(_renderer, texture);
}

void __circleHelper(SDL_Renderer *renderer, float xc, float yc, float x, float y) {
    SDL_RenderDrawPointF(renderer, xc+x, yc+y);
    SDL_RenderDrawPointF(renderer, xc-x, yc+y);
    SDL_RenderDrawPointF(renderer, xc+x, yc-y);
    SDL_RenderDrawPointF(renderer, xc-x, yc-y);
    SDL_RenderDrawPointF(renderer, xc+y, yc+x);
    SDL_RenderDrawPointF(renderer, xc-y, yc+x);
    SDL_RenderDrawPointF(renderer, xc+y, yc-x);
    SDL_RenderDrawPointF(renderer, xc-y, yc-x);
}

void Graphics::DrawCircle(float cx, float cy, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    __circleHelper(_renderer, cx, cy, x, y);
    while (y >= x) {
        x++;
        if (d > 0)
            d += 4 * (x - (--y)) + 10;
        else
            d += 4 * x + 6;
        __circleHelper(_renderer, cx, cy, x, y);
    }
}

void Graphics::DrawCircle(Vec2f c, float radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    DrawCircle(c.x, c.y, radius, r, g, b, a);
}
