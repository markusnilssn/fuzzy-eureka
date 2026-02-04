#include "Content.h"
#include <filesystem>
#include <iostream>
#include <sstream>

TextureSheet::TextureSheet(std::unique_ptr<sf::Texture> texture, const sf::Vector2i &textureSize)
    : texture(std::move(texture)), textureSize(textureSize)
{
}

[[nodiscard]] int TextureSheet::GetRows() const
{
    if (!texture || textureSize.y == 0)
        return 0;

    return texture->getSize().y / textureSize.y;
}

[[nodiscard]] int TextureSheet::GetColumns() const
{
    if (!texture || textureSize.x == 0)
        return 0;

    return texture->getSize().x / textureSize.x;
}

[[nodiscard]] sf::Sprite TextureSheet::Clip(int x, int y)
{
    sf::Sprite sprite(*texture);

    sf::Vector2i point(x * textureSize.x, y * textureSize.y);
    sf::Vector2i size(textureSize.x, textureSize.y);

    sprite.setTextureRect(sf::IntRect(point, size));
    return sprite;
}

sf::Sprite TextureSheet::ClipWithBackground(int x, int y, const sf::Sprite &background)
{
    auto combineTextureKey = [&](int x, int y, const sf::Sprite &sprite) -> std::string
    {
        std::stringstream ss;
        ss << "texture_" << x << "_" << y;

        const sf::Texture *otherTexture = &sprite.getTexture();
        if (otherTexture)
        {
            ss << "_bgTexAddr_" << reinterpret_cast<uintptr_t>(otherTexture);
        }
        else
        {
            ss << "_bgTexNull";
        }

        sf::IntRect otherRect = background.getTextureRect();
        ss << "_bgRect_" << otherRect.position.x << "_" << otherRect.position.y << "_"
           << otherRect.size.x << "_" << otherRect.size.y;

        return ss.str();
    };

    auto key = combineTextureKey(x, y, background);
    auto iterator = combinedTextures.find(key);
    if (iterator != combinedTextures.end())
    {
        return sf::Sprite(*(*iterator).second);
    }

    sf::RenderTexture renderTexture(sf::Vector2u(textureSize.x, textureSize.y));

    renderTexture.clear(sf::Color::Transparent);
    renderTexture.draw(background);
    renderTexture.draw(Clip(x, y));
    renderTexture.display();

    auto texture = std::make_unique<sf::Texture>();
    *texture = renderTexture.getTexture();
    combinedTextures[key] = std::move(texture);
    return sf::Sprite(*combinedTextures[key]);
}

std::string GetFilenameWithoutExtension(const std::string &filePath)
{
    size_t delimit = filePath.find_last_of('/');
    std::string filename = (delimit != std::string::npos) ? filePath.substr(delimit + 1) : filePath;

    size_t extPos = filename.find_last_of('.');
    return (extPos != std::string::npos) ? filename.substr(0, extPos) : filename;
}

Content::Content(const std::string &relativePath, const std::string &nullTexturePath)
    : relativePath(relativePath)
{
    PreloadTexture(nullTexturePath);
    PreloadTextureSheet(nullTexturePath, sf::Vector2i(16, 16));

    std::string textureName = GetFilenameWithoutExtension(nullTexturePath);

    nullTexture = textures.find(textureName)->second.get();
    nullTextureSheet = textureSheets.find(textureName)->second.get();
}

sf::Texture &Content::GetTexture(const std::string &name)
{
    auto iterator = textures.find(name);
    if (iterator == textures.end())
    {
        return *nullTexture;
    }

    return *iterator->second.get();
}

TextureSheet &Content::GetTextureSheet(const std::string &name)
{
    auto iterator = textureSheets.find(name);
    if (iterator == textureSheets.end())
    {
        return *nullTextureSheet;
    }

    return *iterator->second.get();
}

void Content::PreloadTexture(const std::string &filePath)
{
    std::string key = GetFilenameWithoutExtension(filePath);

    if (textures.find(key) != textures.end())
    {
        std::cerr << "Texture already loaded: " << key << std::endl;
        return;
    }

    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(relativePath + filePath))
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return;
    }

    textures.emplace(key, std::move(texture));
}

// Load a texture sheet (spritesheet)
void Content::PreloadTextureSheet(const std::string &filePath, const sf::Vector2i &spriteSize)
{
    std::string key = GetFilenameWithoutExtension(filePath);

    if (textureSheets.find(key) != textureSheets.end())
    {
        std::cerr << "Texture sheet already loaded: " << key << std::endl;
        return;
    }

    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(relativePath + filePath))
    {
        std::cerr << "Failed to load texture sheet: " << filePath << std::endl;
        return;
    }

    auto textureSheet = std::make_unique<TextureSheet>(std::move(texture), spriteSize);
    textureSheets.emplace(key, std::move(textureSheet));
}