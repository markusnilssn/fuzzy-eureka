#include "ResourceManager.h"

// Spritesheet
// Spritesheet::Spritesheet(const std::string &texturePath, const sf::Vector2u &gridSize, const sf::Vector2f &size)
//     : gridSize(gridSize)
//     , size(size)
// {
//     if (!texture.loadFromFile("../resources/" + texturePath))
//     {
//         throw std::runtime_error("Failed to load texture: " + texturePath);
//     }
// }

// void Spritesheet::SetGridSize(const sf::Vector2u &newGridSize)
// {
//     gridSize = newGridSize;
// }

// void Spritesheet::SetSize(const sf::Vector2f &newSize)
// {
//     size = newSize;
// }

// sf::Sprite Spritesheet::LoadSprite(unsigned int x, unsigned int y)
// {
//     sprite.setTexture(texture);
//     sprite.setTextureRect(sf::IntRect({x * size.x, y * size.y}, { size.x, size.y }));
//     return sprite;
// }

// sf::Sprite Spritesheet::LoadSprite(const sf::Vector2u &position)
// {
//     sprite.setTexture(texture);
//     sprite.setTextureRect(sf::IntRect({ position.x * size.x, position.y * size.y }, { size.x, size.y }));
//     return sprite;
// }

// ResourceManager
ResourceManager::ResourceManager()
{
    fonts.reserve(1 << 1);
    textures.reserve(1 << 5);
}

sf::Texture* ResourceManager::GetTexture(const std::string &texturePath)
{
    auto iterator = textures.find(texturePath);
    if(iterator != textures.end())
    {
        return iterator->second.get();
    }

    std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>();

    // Check if the texture was loaded successfully
    if (!texture->loadFromFile("../resources/" + texturePath))
    {
        throw std::runtime_error("Failed to load texture: " + texturePath);
    }

    sf::Texture* texturePtr = texture.get();
    textures[texturePath] = std::move(texture);
    return texturePtr;
}

sf::Font* ResourceManager::GetFont(const std::string &fontPath)
{
    throw std::runtime_error("Failed to load font: " + fontPath);
}