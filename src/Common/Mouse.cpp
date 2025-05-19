#include "Mouse.h"

const bool Mouse::IsMouseButtonPressed(sf::Mouse::Button button)
{
    auto iterator = mouseButtonStates.find(button);
    if (iterator != mouseButtonStates.end())
    {
        return iterator->second;
    }

    return false;
}

const bool Mouse::IsMouseButtonDown(sf::Mouse::Button button)
{
    auto iterator = mouseButtonStates.find(button);
    if (iterator != mouseButtonStates.end())
    {
        return iterator->second;
    }

    return false;
}

const bool Mouse::IsMouseButtonUp(sf::Mouse::Button button)
{
    auto iterator = mouseButtonStates.find(button);
    if (iterator != mouseButtonStates.end())
    {
        return !iterator->second;
    }

    return false;
}

sf::Vector2i Mouse::GetMousePosition()
{
    return mousePosition;
}

sf::Vector2f Mouse::GetMousePosition(const sf::RenderWindow& window)
{
    return window.mapPixelToCoords(mousePosition);
}

void Mouse::Update()
{
    for (auto& [button, state] : mouseButtonStates)
    {
        state = sf::Mouse::isButtonPressed(button);
    }

    mousePosition = sf::Mouse::getPosition();
}