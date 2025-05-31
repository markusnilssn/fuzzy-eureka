// Keyboard.cpp
#include "Keyboard.h"
#include "Debug.h"

#define DEBUG_MODE 


Keyboard::Keyboard(sf::RenderWindow& window)
    : window(window)
{  
    RegisterKey(sf::Keyboard::Key::Escape);
    RegisterKey(sf::Keyboard::Key::Space);
    RegisterKey(sf::Keyboard::Key::Tab);
    RegisterKey(sf::Keyboard::Key::LShift);
    RegisterKey(sf::Keyboard::Key::RShift);

    RegisterKey(sf::Keyboard::Key::A);
    RegisterKey(sf::Keyboard::Key::D);
    RegisterKey(sf::Keyboard::Key::W);
    RegisterKey(sf::Keyboard::Key::S);
    RegisterKey(sf::Keyboard::Key::Left);
    RegisterKey(sf::Keyboard::Key::Right);
    RegisterKey(sf::Keyboard::Key::Up);
    RegisterKey(sf::Keyboard::Key::Down);
}

void Keyboard::RegisterKey(sf::Keyboard::Key key)
{
    registeredKeys.insert(key);

    currentState[key] = sf::Keyboard::isKeyPressed(key);
    previousState[key] = currentState[key];
}

void Keyboard::UnregisterKey(sf::Keyboard::Key key)
{
    if (registeredKeys.erase(key) == 0)
    {
        Debug::LogWarning("Attempted to unregister a key that wasn't registered");
    }

    currentState.erase(key);
    previousState.erase(key);
}

const bool Keyboard::IsKeyDown(sf::Keyboard::Key key)
{
    if(!window.hasFocus())
    {
        return false;
    }

#ifdef DEBUG_MODE    
    if(registeredKeys.find(key) == registeredKeys.end())
    {
        Debug::LogWarning("Attempted to check a key that wasn't registered");
        return false;
    }
#endif

    auto iterator = currentState.find(key);
    return iterator != currentState.end() && iterator->second;
}

const bool Keyboard::IsKeyUp(sf::Keyboard::Key key)
{
#ifdef DEBUG_MODE    
    if(registeredKeys.find(key) == registeredKeys.end())
    {
        Debug::LogWarning("Attempted to check a key that wasn't registered");
        return false;
    }
#endif

    auto iterator = currentState.find(key);
    return iterator != currentState.end() && !iterator->second;
}

const bool Keyboard::IsKeyPressed(sf::Keyboard::Key key)
{
    if(!window.hasFocus())
    {
        return false;
    }
    
#ifdef DEBUG_MODE    
    if(registeredKeys.find(key) == registeredKeys.end())
    {
        Debug::LogWarning("Attempted to check a key that wasn't registered");
        return false;
    }
#endif

    auto current = currentState.find(key);
    auto previous = previousState.find(key);
    if (current != currentState.end() && previous != previousState.end())
    {
        return current->second && !previous->second;
    }
    return false;
}

const int Keyboard::HorizontalAxis()
{
    bool left  = IsKeyDown(sf::Keyboard::Key::A) || IsKeyDown(sf::Keyboard::Key::Left);
    bool right = IsKeyDown(sf::Keyboard::Key::D) || IsKeyDown(sf::Keyboard::Key::Right);
    
    if (left && !right) return -1;
    if (right && !left) return 1;

    return 0;
}

const int Keyboard::VerticalAxis()
{
    bool up = IsKeyDown(sf::Keyboard::Key::W) || IsKeyDown(sf::Keyboard::Key::Up);
    bool down = IsKeyDown(sf::Keyboard::Key::S) || IsKeyDown(sf::Keyboard::Key::Down);

    if (up && !down) return -1;
    if (down && !up) return 1;

    return 0;
}

void Keyboard::Update()
{
    for (auto key : registeredKeys)
    {
        previousState[key] = currentState[key];
    }

    for (auto key : registeredKeys)
    {
        currentState[key] = sf::Keyboard::isKeyPressed(key);
    }
}
