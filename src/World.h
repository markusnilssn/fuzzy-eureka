#pragma once
#include "SFML/Graphics.hpp"
#include "Engine/Content.h"
#include "Grid.h"
#include "Engine/Engine.h"

#include <vector>

class World
{
public:
    World(Content &content, Engine &engine, Grid &grid, int width, int height, const sf::Vector2i &nodeSize);

    void Render(sf::RenderWindow &window);

private:
    Content &content;
    Grid &grid;
    Engine &engine;
    std::vector<sf::Sprite> world;
    int width;
    int height;
    sf::Vector2i nodeSize;

    void LoadFromPearlyNoise(int width, int height);
};