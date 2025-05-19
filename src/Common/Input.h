#pragma once
#include "Mouse.h"
#include "Keyboard.h"

class Input final 
{
    friend class Application;
public:
    static Input& GetInstance();

public:
    Mouse& GetMouse();
    Keyboard& GetKeyboard();
    

private:
    Input() = default;
    
    Keyboard keyboard;
    Mouse mouse;
    
    void Update();
};
