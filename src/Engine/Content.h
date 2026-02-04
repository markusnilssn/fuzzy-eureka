
#pragma once
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

struct TextureSheet
{
    std::unique_ptr<sf::Texture> texture;
    sf::Vector2i textureSize;
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> combinedTextures;

    TextureSheet(std::unique_ptr<sf::Texture> texture, const sf::Vector2i &textureSize);

    [[nodiscard]] int GetRows() const;
    [[nodiscard]] int GetColumns() const;

    [[nodiscard]] sf::Sprite Clip(int x, int y);
    [[nodiscard]] sf::Sprite ClipWithBackground(int x, int y, const sf::Sprite &background);
};

class Content final
{
    using Key = std::string;

public:
    Content(const std::string &relativePath = "resources/", const std::string &nullTexturePath = "Templates/16x16Large.png");

    sf::Texture &GetTexture(const std::string &name);
    TextureSheet &GetTextureSheet(const std::string &name);

    void PreloadTexture(const std::string &filePath);
    void PreloadTextureSheet(const std::string &filePath, const sf::Vector2i &spriteSize);

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<TextureSheet>> textureSheets;

    std::string relativePath;

    sf::Texture *nullTexture;
    TextureSheet *nullTextureSheet;
};