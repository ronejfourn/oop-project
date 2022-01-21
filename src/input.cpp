#include "headers/input.h"

#define isDown 0x01
#define isHeld 0x02
#define wasPressed 0x04

Input *Input::_instance = nullptr;

Input *Input::GetInstance() {
    if (!_instance) _instance = new Input;
    return _instance;
}

Input::Input() {
    _btnBind[0] = nullptr;
    _btnBind[1] = nullptr;
    _btnBind[2] = nullptr;
}

void Input::BindActionToKey(SDL_Scancode key, const std::function<void()>& action, bool hold) {
    _keyBind[key] = action;
    hold ? _keyDown[key].v |= isHeld : _keyDown[key].v &= ~isHeld;
}

void Input::BindActionToBtn(int btn, const std::function<void(int, int)>& action, bool hold) {
    _btnBind[btn] = action;
    hold ? _btnDown[btn].v |= isHeld : _btnDown[btn].v &= ~isHeld;
}

void Input::Handle() {
    for (auto &i : _keyBind) {
        SDL_Scancode key = i.first;
        if (_keyDown[key].v & isDown) {
            i.second();
            if (!(_keyDown[key].v & isHeld)) {
                _keyDown[key].v |= wasPressed;
                _keyDown[key].v &= ~isDown;
            }
        }
    }
    for (int i = 0; i < 3; i ++) {
        if ((_btnDown[i].v &isDown) && _btnBind[i] != nullptr) {
            _btnBind[i](_clkx, _clky);
            if (!(_btnDown[i].v & isHeld)) {
                _btnDown[i].v |= wasPressed;
                _btnDown[i].v &= ~isDown;
            }
        }
    }
}

void Input::ClearBinds() {
    _keyBind.clear();
    _keyDown.clear();
    _btnBind[0] = nullptr;
    _btnBind[1] = nullptr;
    _btnBind[2] = nullptr;
    _btnDown[0].v = 0;
    _btnDown[1].v = 0;
    _btnDown[2].v = 0;
}

void Input::KeyUp(SDL_Scancode key) {
    _keyDown[key].v &= ~wasPressed;
    _keyDown[key].v &= ~isDown;
}

void Input::KeyDown(SDL_Scancode key) {
    if (_keyDown[key].v & isHeld) {
        _keyDown[key].v |= isDown;
    } else if (!(_keyDown[key].v & wasPressed)){
        _keyDown[key].v |= isDown;
    }
}

void Input::BtnUp(int btn) {
    _btnDown[btn - 1].v &= ~wasPressed;
    _btnDown[btn - 1].v &= ~isDown;
}

void Input::BtnDown(int btn, int x, int y) {
    _clkx = x; _clky = y;
    if (_btnDown[btn - 1].v & isHeld) {
        _btnDown[btn - 1].v |= isDown;
    } else if (!(_btnDown[btn - 1].v & wasPressed)){
        _btnDown[btn - 1].v |= isDown;
    }
}
