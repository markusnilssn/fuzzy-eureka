#pragma once
#include "System.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

class Engine;
class RenderSystem final : public System
{
public:
    explicit RenderSystem(Engine& engine);

    void Start() override;
    void Destroy() override;

    void Render(sf::RenderWindow& window) override;

private:

};