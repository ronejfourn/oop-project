#include "input.h"
Input::Input() {
    _btnDown[0] = false;
    _btnDown[1] = false;
    _btnDown[2] = false;
    _btnBind[0] = nullptr;
    _btnBind[1] = nullptr;
    _btnBind[2] = nullptr;
}

void Input::BindActionToKey(SDL_Scancode key, const std::function<void()>& action, bool hold) {
    _keyBind[key] = action;
    _holdKey[key] = hold;
}

void Input::Handle() {
    for (auto &i : _keyBind) {
        SDL_Scancode key = i.first;
        if (_keyDown[key]) {
            _keyBind[key]();
            _keyDown[key] = _holdKey[key];
        }
    }
    for (int i = 0; i < 3; i ++) {
        if (_btnDown[i] && _btnBind[i] != nullptr) {
            _btnBind[i]();
            _btnDown[i] = _holdBtn[i];
        }
    }
}

void Input::ClearBinds() {
    _keyBind.clear();
    _holdKey.clear();
    _btnBind[0] = nullptr;
    _btnBind[1] = nullptr;
    _btnBind[2] = nullptr;
}

void Input::Flush() {
    _keyDown.clear();
}

void Input::KeyUp(SDL_Scancode key) {
    _keyDown[key] = false;
}

void Input::KeyDown(SDL_Scancode key) {
    _keyDown[key] = true;
}

void Input::BtnUp(int btn) {
    _btnDown[btn - 1] = false;
}

void Input::BtnDown(int btn) {
    _btnDown[btn - 1] = true;
}
