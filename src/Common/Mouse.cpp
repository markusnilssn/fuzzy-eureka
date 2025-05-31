#include "Mouse.h"
#include <iostream>

Mouse::Mouse(sf::RenderWindow& window)
    : window(window)
{
    RegisterMouseButton(sf::Mouse::Button::Left);
    RegisterMouseButton(sf::Mouse::Button::Right);
    RegisterMouseButton(sf::Mouse::Button::Middle);
}

const bool Mouse::IsMouseButtonPressed(sf::Mouse::Button button)
{
    if(!window.hasFocus())
    {
        return false;
    }

    auto iterator = mouseButtonStates.find(button);
    if(iterator == mouseButtonStates.end())
    {
        return false;
    }

    auto previousIterator = previousMouseState.find(button);
    if(previousIterator == mouseButtonStates.end())
    {
        return false;
    }

    return iterator->second && !previousIterator->second;
}

const bool Mouse::IsMouseButtonDown(sf::Mouse::Button button)
{
    if(!window.hasFocus())
    {
        return false;
    }

    auto iterator = mouseButtonStates.find(button);
    if(iterator == mouseButtonStates.end())
    {
        return false;
    }

    return iterator->second;
}

const bool Mouse::IsMouseButtonUp(sf::Mouse::Button button)
{
    return !IsMouseButtonDown(button);
}

const sf::Vector2i Mouse::GetMousePosition()
{
    return sf::Mouse::getPosition();
}

const sf::Vector2f Mouse::GetMousePosition(const sf::RenderWindow& window)
{
    // 1) Raw pixel coords relative to the *backing* surface:
    sf::Vector2i raw = sf::Mouse::getPosition(window);

    // 2) Logical window size (points) vs. view size (world units):
    sf::Vector2u winSize   = window.getSize();          // e.g. {400, 300} on a 800×600 Retina window
    sf::Vector2f viewSize  = window.getView().getSize(); // e.g. {800, 600} if you never zoomed or panned

    // 3) Compute scale factors:
    float scaleX = viewSize.x / static_cast<float>(winSize.x);
    float scaleY = viewSize.y / static_cast<float>(winSize.y);

    // 4) Apply them to your raw pixel coords:
    sf::Vector2f corrected{ raw.x * scaleX, raw.y * scaleY };
    return corrected;
    // return window.mapPixelToCoords(sf::Mouse::getPosition());
}

void Mouse::RegisterMouseButton(sf::Mouse::Button button)
{
    mouseButtonStates.emplace(button, false);
}

void Mouse::Update()
{
    previousMouseState = mouseButtonStates;

    for (auto& [button, state] : mouseButtonStates)
    {
        state = sf::Mouse::isButtonPressed(button);
    }
}