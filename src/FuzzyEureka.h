#pragma once 
#include "Engine/Application.h"
#include "Content.h"

#include "Common/Random.h"
#include "Grid.h"

#include <memory>
#include <vector>
#include <map>
#include <sstream>



class AStarSystem;
class FuzzyEureka final : public Application 
{ 
public:
    FuzzyEureka();
    ~FuzzyEureka();

    void Start() override;
    void Destroy() override;
    void Update(const float deltaTime) override;
    void Render(sf::RenderWindow& window) override;
    void HandleEvent(const std::optional<sf::Event>& event) override;

private:
    sf::View camera;

    // sf::Clock clock;
    // sf::Time currentTime;
    // sf::Time previousTime;

    TileSet cliff;
    TileSet cliffWater;
    TileSet winter;
    TileSet grass;
    TileSet shore;
    void LoadFromPearlyNoise(const int width, const int height);

    // sf::Font font;
    int width{};
    int height{};
    // sf::Color** world;
    std::vector<sf::Sprite> world;

    Random random;

    sf::Vector2i nodeSize;



    Entity animal;
    Entity building;
    std::shared_ptr<Grid> grid;

};