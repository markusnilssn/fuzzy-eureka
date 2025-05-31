#pragma once 
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

struct TransformComponent
{
    sf::Vector2f position;
    float rotation;

    void SetPosition(const sf::Vector2f& worldPosition) { position = worldPosition; }
};