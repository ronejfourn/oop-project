#include "graphics.h"
#include "logger.h"
#include <SDL2/SDL.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb/stb_image.h>

Graphics::Graphics() {
    _wwidth  = 1280;
    _wheight = 720 ;

    _dt = 1000.0 / 60;

    _offx = 0;
    _offy = 0;

    _window = SDL_CreateWindow(
            "Unnamed Window",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            _wwidth, _wheight,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (_window == NULL)
        Logger::LogError("Failed to create SDL Window");

    _renderer = SDL_CreateRenderer(
            _window,
            -1,
            SDL_RENDERER_ACCELERATED);

    if (_renderer == NULL)
        Logger::LogError("Failed to create SDL Renderer");

    SDL_RenderSetLogicalSize(_renderer, _wwidth, _wheight);
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
}

Graphics::~Graphics() {
    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_renderer);
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
    (SDL_GetTicks() - _dbegin) < _dt ?
        SDL_Delay(_dt - (SDL_GetTicks() - _dbegin)) :
        SDL_Delay(_dt);
    _dbegin = SDL_GetTicks();
}

void Graphics::SetTitle(const char *title, const char *icon_file) {
    SDL_SetWindowTitle(_window, title);

    if (icon_file != nullptr) {
        SDL_Surface *icon = SDL_LoadBMP(icon_file);
        SDL_SetWindowIcon(_window, icon);
    }
}

void Graphics::SetTargetFPS(uint32_t fps) {
    if (fps == 0) {
        Logger::LogWarning("FPS cannot be zero");
        fps = 60;
    }
    _dt = 1000.0 / fps;
}

void Graphics::DrawRect(int tl_x, int tl_y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool offs) {
    SDL_Rect tmp = {
        .x = tl_x - _offx * offs,
        .y = tl_y - _offy * offs,
        .w = width,
        .h = height
    };
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderDrawRect(_renderer, &tmp);
}

void Graphics::DrawRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool offs) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    if (!offs) {
        SDL_RenderDrawRect(_renderer, &rect);
    } else {
        SDL_Rect temp = rect;
        temp.x -= _offx;
        temp.y -= _offy;
        SDL_RenderDrawRect(_renderer, &temp);
    }
}

void Graphics::FillRect(int tl_x, int tl_y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool offs) {
    SDL_Rect tmp = {
        .x = tl_x - _offx * offs,
        .y = tl_y - _offy * offs,
        .w = width,
        .h = height
    };
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderFillRect(_renderer, &tmp);
}

void Graphics::FillRect(SDL_Rect &rect, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool offs) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    if (!offs) {
        SDL_RenderFillRect(_renderer, &rect);
    } else {
        SDL_Rect temp = rect;
        temp.x -= _offx;
        temp.y -= _offy;
        SDL_RenderFillRect(_renderer, &temp);
    }
}

void Graphics::DrawPoint(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool offs) {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    SDL_RenderDrawPoint(_renderer, x - _offx * offs, y - _offy * offs);
}

void Graphics::DrawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint32_t t, bool offs) {
    x1 -= _offx * offs;
    x2 -= _offx * offs;
    y1 -= _offy * offs;
    y2 -= _offy * offs;
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    if (t <= 1) {
        SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
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

void Graphics::DrawTexture(SDL_Texture *texture, SDL_Rect &src, SDL_Rect &dst, bool offs, SDL_Point *center, float angle, bool flip) {
    if (!offs) {
        SDL_RenderCopyEx(_renderer, texture, &src, &dst, angle, center, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    } else {
        SDL_Rect tmp = dst;
        tmp.x -= _offx;
        tmp.y -= _offy;
        SDL_RenderCopyEx(_renderer, texture, &src, &tmp, angle, center, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }
}

float Graphics::GetDeltaTime() {return _dt;}

SDL_Point Graphics::GetCurrentResolution() {
    SDL_Point ret;
    SDL_GetWindowSize(_window, &ret.x, &ret.y);
    return ret;
}

SDL_Point Graphics::GetCursorPosition() {
    SDL_Point res = GetCurrentResolution();
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_Point ret = {
        static_cast<int>(mouse_x * float(_wwidth ) / res.x),
        static_cast<int>(mouse_y * float(_wheight) / res.y)};
    return ret;
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
