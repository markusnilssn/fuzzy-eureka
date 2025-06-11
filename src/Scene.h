#pragma once
#include <SFML/Graphics.hpp>

class Scene 
{
public:
    virtual ~Scene() = default;
    virtual void Start() = 0;
    virtual void Destroy() = 0;
    virtual void Update(const float deltaTime) = 0;
    virtual void Render(sf::RenderWindow& window)= 0;
};