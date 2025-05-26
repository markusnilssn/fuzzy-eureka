#pragma once
#include "SFML/Graphics.hpp"    
#include "SFML/System.hpp"

struct AnimatorComponent 
{
    sf::Texture texture;
    sf::IntRect bounds;
    bool rgb = false;
};
