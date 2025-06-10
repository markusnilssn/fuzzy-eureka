#pragma once 
#include "Engine/Application.h"
#include <memory>
#include "Grid.h"
#include <vector>
#include <map>
#include <sstream>

struct TileSet
{
    sf::Texture texture;
    int tileSize;

    std::map<std::string, std::unique_ptr<sf::Texture>> combinedTextures;
    // std::vector<std::unique_ptr<sf::Texture>> combinedTextures;

    TileSet(const std::string& filePath, int tileSize)
        : texture(filePath)
        , tileSize(tileSize)
    {

    }

    sf::Sprite CreateSprite(int x, int y)
    {
        return sf::Sprite(texture, sf::IntRect({x*tileSize, y*tileSize}, {tileSize, tileSize}));
    }

    sf::Sprite CreateSpriteWitBackground(int x, int y, const sf::Sprite other)
    {
        auto combineTextureKey = [&](int x, int y, const sf::Sprite& sprite) -> std::string
        {
            std::stringstream ss;
            ss << "texture_" << x << "_" << y;

            const sf::Texture* otherTexture = &sprite.getTexture();
            if (otherTexture) {
                ss << "_bgTexAddr_" << reinterpret_cast<uintptr_t>(otherTexture);
            } else {
                ss << "_bgTexNull"; // Indicate if background has no texture (shouldn't happen with valid sprites)
            }

            sf::IntRect otherRect = other.getTextureRect();
                ss << "_bgRect_" << otherRect.position.x << "_" << otherRect.position.y << "_"
                    << otherRect.size.x << "_" << otherRect.size.y;


            return ss.str();
        };

        auto key = combineTextureKey(x, y, other);
        auto iterator = combinedTextures.find(key);
        if(iterator != combinedTextures.end())
        {
            return sf::Sprite(*(*iterator).second);
        }

        sf::RenderTexture renderTexture(sf::Vector2u(tileSize, tileSize));
        renderTexture.clear(sf::Color::Transparent);
        renderTexture.draw(other);
        renderTexture.draw(CreateSprite(x, y));
        renderTexture.display();

        auto texture = std::make_unique<sf::Texture>();
        *texture = renderTexture.getTexture(); 
        combinedTextures[key] = std::move(texture);
        return sf::Sprite(*combinedTextures[key]);
        // combinedTextures.emplace_back(std::move(texture));
        // return sf::Sprite(*combinedTextures[combinedTextures.size() - 1]);
    }
};


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
    int width;
    int height;
    // sf::Color** world;
    std::vector<sf::Sprite> world;

    sf::Vector2i nodeSize;

    Entity animal;
    Entity building;
    std::shared_ptr<Grid> grid;

};