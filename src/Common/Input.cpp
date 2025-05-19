#include "Input.h"

Input& Input::GetInstance()
{
    static Input instance;
    return instance;
}

Mouse& Input::GetMouse()
{
    return mouse;
}

Keyboard& Input::GetKeyboard()
{
    return keyboard;
}

void Input::Update()
{
    keyboard.Update();
    mouse.Update();
}