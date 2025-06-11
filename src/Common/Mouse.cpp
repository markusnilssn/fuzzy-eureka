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

const bool Mouse::IsMouseButtonReleased(sf::Mouse::Button button)
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

    return !iterator->second && previousIterator->second;
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
    sf::Vector2i raw = sf::Mouse::getPosition(window);

    // sf::Vector2u windowSize   = window.getSize();          
    // sf::Vector2f viewSize  = window.getView().getSize(); 

    // float scaleX = viewSize.x / static_cast<float>(windowSize.x);
    // float scaleY = viewSize.y / static_cast<float>(windowSize.y);

            const sf::Vector2f worldPosition = window.mapPixelToCoords(
            sf::Vector2i(static_cast<int>(raw.x), static_cast<int>(raw.y)),
            window.getView()
        );
        return worldPosition;


    // sf::Vector2f corrected { raw.x * scaleX, raw.y * scaleY };
    // return corrected;
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