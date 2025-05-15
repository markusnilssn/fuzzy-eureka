#include "ResourceManager.h"

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