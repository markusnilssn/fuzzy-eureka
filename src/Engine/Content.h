
#pragma once
#include <unordered_map>
#include <string> 
#include <typeinfo>

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"


// struct SpriteSheet 
// {
//     sf::Texture& texture;
//     sf::Vector2i tileSize;

//     SpriteSheet(sf::Texture& texture, const sf::Vector2i& tileSize);

//     [[nodiscard]] const int GetRows() const;
//     [[nodiscard]] const int GetColumns() const;
// };

struct Texture2D 
{
    sf::Texture* texture;
};

class Content final
{
public:
    Content(const std::string& relativePath = "resources/");

    // sf::Texture* GetTexture(const std::string& filePath);

    // void LoadTexture(const std::string& filePath);
    // void LoadTextureWithBackground(const std::string& filePath);

    void LoadTexture2D(const std::string& filePath, const sf::Vector2i& tileSize);

private: 
    // std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::string relativePath;
};