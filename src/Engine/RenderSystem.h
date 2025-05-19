#pragma once
#include "System.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

class RenderSystem : public System
{
public:
    RenderSystem(Engine* engine);

    void Start() override;
    void Destroy() override;

    void Render(sf::RenderWindow& window) override;

private:
    

};