#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <unordered_map>
#include <string>
#include <memory>

// class Spritesheet {
// public:
// public:
//     Spritesheet(const std::string& texturePath, const sf::Vector2u& gridSize, const sf::Vector2f& size)
//         : gridSize(gridSize)
//         , size(size)
//     {
//         if (!texture.loadFromFile("../resources/" + texturePath))
//         {
//             throw std::runtime_error("Failed to load texture: " + texturePath);
//         }
//     }

// private:
//     sf::Vector2u gridSize;
//     sf::Vector2f size;
//     sf::Texture texture;

// };

class ResourceManager {
public:
    static ResourceManager& GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }

public:
    ResourceManager();

    sf::Texture* GetTexture(const std::string& texturePath);
    sf::Font* GetFont(const std::string& fontPath);

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts;

};