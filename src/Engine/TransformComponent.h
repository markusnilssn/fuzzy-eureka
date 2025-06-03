#pragma once 
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

struct TransformComponent
{
    sf::Vector2f position;
    sf::Vector2f size;
    sf::Angle angle;
};