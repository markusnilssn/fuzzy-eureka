#pragma once
#include <SFML/Graphics.hpp>    
#include <SFML/System.hpp>

struct SpriteComponent 
{
    sf::Texture texture;
    uint_fast16_t sortLayer;
};
