#include "Input.h"

Input::Input(sf::RenderWindow &window)
    : window(window)
    , mouse(window)
    , keyboard(window)
{
    
}

Mouse &Input::GetMouse()
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