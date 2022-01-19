#pragma once
#include <SDL2/SDL_scancode.h>
#include <map>
#include <functional>

class Input {
    static Input *_instance;
private:
    std::map<SDL_Scancode, bool> _keyDown;
    std::map<SDL_Scancode, std::function<void()>> _keyBind;
    std::map<SDL_Scancode, bool> _holdKey;
    bool _btnDown[3];
    bool _holdBtn[3];
    std::function<void()> _btnBind[3];
    Input();
public:
    static Input *GetInstance();

    void BindActionToKey(SDL_Scancode key, const std::function<void()>& action, bool hold);
    void BindActionToBtn(int btn, const std::function<void()>& action, bool hold);

    void Handle();
    void Flush();
    void ClearBinds();

    void KeyUp  (SDL_Scancode key);
    void KeyDown(SDL_Scancode key);

    void BtnUp  (int btn);
    void BtnDown(int btn);
};
