#pragma once
#include "Mouse.h"
#include "Keyboard.h"

class Input final 
{
    friend class Application;
public:
    Input(sf::RenderWindow& window);

    Mouse& GetMouse();
    Keyboard& GetKeyboard();

private:
    sf::RenderWindow& window;
    
    Keyboard keyboard;
    Mouse mouse;
    
    void Update();
};
