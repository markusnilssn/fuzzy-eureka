#pragma once 
#include "Engine/Application.h"
#include <memory>
#include "Grid.h"

class FuzzyEureka final : public Application 
{ 
public:
    FuzzyEureka();

    void Start() override;
    void Destroy() override;
    void Update(const float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    void HandleEvent(const sf::Event& event) override;

private:
    sf::View camera;

    sf::Clock clock;
    sf::Time currentTime;
    sf::Time previousTime;

    sf::Font font;

};