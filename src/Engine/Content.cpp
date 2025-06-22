#include "Content.h"

/***
 * Content
 */
Content::Content()
    : relativePath("resources/")
{

}

sf::Texture* Content::GetTexture(const std::string &texturePath)
{
    size_t hashCode = std::hash<std::string>{}(texturePath);
    auto iterator = textures.find(hashCode);
    if(iterator != textures.end())
    {
        return (iterator->second).get();
    }

    std::unique_ptr<sf::Texture> texture = std::make_unique<sf::Texture>(GetRelativePath(texturePath));
    sf::Texture* returnValue = texture.get();
    textures[hashCode] = std::move(texture);
    return returnValue;
}

sf::Font* Content::GetFont(const std::string &fontPath)
{
    size_t hashCode = std::hash<std::string>{}(fontPath);
    auto iterator = fonts.find(hashCode);
    if(iterator != fonts.end())
    {
        return (iterator->second).get();
    }

    std::unique_ptr<sf::Font> font = std::make_unique<sf::Font>(GetRelativePath(fontPath));
    sf::Font* returnValue = font.get();
    fonts[hashCode] = std::move(font);
    return returnValue;
}

sf::Texture *Content::CombineTextures(const std::string& name, sf::Texture* front, sf::Texture* back)
{
    return nullptr;
}

std::string Content::GetRelativePath(const std::string &filePath)
{
    return relativePath + filePath;
}

/***
 * Animation
 */
Animation::Animation(sf::Texture *texture, const sf::Vector2i &clipSize)
    : texture(texture)
    , clipSize(clipSize)
{
    
}

void Animation::Cut(const std::string &name, const sf::Vector2i &start, const sf::Vector2i &end)
{
    constexpr int offset = 1;

    int width = (end.x - start.x) + offset;
    int height = (end.y - start.y) + offset;

    int frames = width * height;

    Sequence sequence{};
    for (int i = 0; i < frames; i++)
    {
        int x = i / height;
        int y = i % height;

        sequence.emplace_back(sf::IntRect(sf::Vector2i(x * clipSize.x, y * clipSize.y), clipSize));
    }
    
    size_t hashCode = std::hash<std::string>{}(name);
    sequences[hashCode] = std::move(sequence);
}

const Animation::Sequence &Animation::GetSequence(const std::string &name)
{
    size_t hashCode = std::hash<std::string>{}(name);

    auto iterator = sequences.find(hashCode);
    if(iterator == sequences.end())
    {
        throw std::out_of_range("Failed to find sequence");
    }

    return sequences[hashCode];
}

/***
 * SpriteSheet
 */
SpriteSheet::SpriteSheet(sf::Texture *texture, const sf::Vector2i &tileSize)
    : texture(texture)
    , tileSize(tileSize)
{

}

void SpriteSheet::Cut(const std::string &name, const sf::Vector2i &index)
{
    sf::IntRect rect(sf::Vector2i(index.x * tileSize.x, index.y * tileSize.y), tileSize);
    
    size_t hashCode = std::hash<std::string>{}(name);
    tiles[hashCode] = std::move(rect);
}

const SpriteSheet::Tile &SpriteSheet::GetTile(const std::string &name)
{
    size_t hashCode = std::hash<std::string>{}(name);

    auto iterator = tiles.find(hashCode);
    if(iterator == tiles.end())
    {
        throw std::out_of_range("Failed to find sequence");
    }

    return tiles[hashCode];
}
