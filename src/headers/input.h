#pragma once
#include <SDL2/SDL_scancode.h>
#include <map>
#include <functional>

class Input {
    static Input *_instance;
private:
    struct _st{ uint8_t v = 0; };
    std::map<SDL_Scancode, _st> _keyDown;
    std::map<SDL_Scancode, std::function<void()>> _keyBind;
    _st _btnDown[3];
    int _clkx, _clky;
    std::function<void(int, int)> _btnBind[3];
    Input();
public:
    static Input *GetInstance();

    void BindActionToKey(SDL_Scancode key, const std::function<void()>& action, bool hold);
    void BindActionToBtn(int btn, const std::function<void(int, int)>& action, bool hold);

    void Handle();
    void ClearBinds();

    void KeyUp  (SDL_Scancode key);
    void KeyDown(SDL_Scancode key);

    void BtnUp  (int btn);
    void BtnDown(int btn, int x, int y);
};
