#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <memory>

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