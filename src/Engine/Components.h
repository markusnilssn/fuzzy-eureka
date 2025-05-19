#pragma once 
#include <array>    
#include <stdint.h>
#include "SFML/Graphics.hpp"    
#include "SFML/System.hpp"

struct SpriteComponent
{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::IntRect bound;
};

struct TransformComponent
{
    sf::Vector2f position;
    sf::Angle rotation;
};