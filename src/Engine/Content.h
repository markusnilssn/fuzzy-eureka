#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <unordered_map>
#include <string>
#include <memory>

#include <map>
#include <sstream>

enum struct Read
{
    LeftToRight,
    RightToLeft,
    UpToDown,
    DownToUp,
};

// struct TileSet
// {
//     sf::Texture texture;
//     int tileSize;

//     std::map<std::string, std::unique_ptr<sf::Texture>> combinedTextures;
//     // std::vector<std::unique_ptr<sf::Texture>> combinedTextures;

//     TileSet(const std::string& filePath, int tileSize)
//         : texture(filePath)
//         , tileSize(tileSize)
//     {

//     }

//     sf::Sprite CreateSprite(int x, int y)
//     {
//         return sf::Sprite(texture, sf::IntRect({x*tileSize, y*tileSize}, {tileSize, tileSize}));
//     }

//     sf::Sprite CreateSpriteWitBackground(int x, int y, const sf::Sprite other)
//     {
//         auto combineTextureKey = [&](int x, int y, const sf::Sprite& sprite) -> std::string
//         {
//             std::stringstream ss;
//             ss << "texture_" << x << "_" << y;

//             const sf::Texture* otherTexture = &sprite.getTexture();
//             if (otherTexture) {
//                 ss << "_bgTexAddr_" << reinterpret_cast<uintptr_t>(otherTexture);
//             } else {
//                 ss << "_bgTexNull"; // Indicate if background has no texture (shouldn't happen with valid sprites)
//             }

//             sf::IntRect otherRect = other.getTextureRect();
//                 ss << "_bgRect_" << otherRect.position.x << "_" << otherRect.position.y << "_"
//                     << otherRect.size.x << "_" << otherRect.size.y;


//             return ss.str();
//         };

//         auto key = combineTextureKey(x, y, other);
//         auto iterator = combinedTextures.find(key);
//         if(iterator != combinedTextures.end())
//         {
//             return sf::Sprite(*(*iterator).second);
//         }

//         sf::RenderTexture renderTexture(sf::Vector2u(tileSize, tileSize));
//         renderTexture.clear(sf::Color::Transparent);
//         renderTexture.draw(other);
//         renderTexture.draw(CreateSprite(x, y));
//         renderTexture.display();

//         auto texture = std::make_unique<sf::Texture>();
//         *texture = renderTexture.getTexture(); 
//         combinedTextures[key] = std::move(texture);
//         return sf::Sprite(*combinedTextures[key]);
//     }
// };

class SpriteSheet final
{
public:
    using Tile = sf::IntRect;

public:
    sf::Texture* texture{};
    sf::Vector2i tileSize{};

    SpriteSheet() = default;
    SpriteSheet(sf::Texture* texture, const sf::Vector2i& tileSize);

    void Cut(const std::string& name, const sf::Vector2i& index);
    [[nodiscard]] const Tile& GetTile(const std::string& name);

private:
    std::unordered_map<size_t, sf::IntRect> tiles;

};

class Animation final
{
public:
    using Frame = sf::IntRect;
    using Sequence = std::vector<Frame>;

public:
    sf::Texture* texture{};
    sf::Vector2i frameSize{};

    Animation() = default;
    Animation(sf::Texture* texture, const sf::Vector2i& frameSize);
    void Cut(const std::string& name, const sf::Vector2i& start, const sf::Vector2i& end);
    [[nodiscard]] const Sequence& GetSequence(const std::string& name);

private:
    std::unordered_map<size_t, std::vector<sf::IntRect>> sequences;
};

class Content final
{   
public:
    Content(const std::filesystem::path& contentJsonPath);

    [[nodiscard]] sf::Texture* GetTexture(const std::string& texturePath);
    [[nodiscard]] sf::Font* GetFont(const std::string& fontPath);

    [[nodiscard]] sf::Texture* CombineTextures(const std::string& name, sf::Texture* front, sf::Texture* back);

    void LoadSpriteSheet(const std::string& filePath, const sf::Vector2i tileSize = sf::Vector2i(16, 16));
    // void LoadTexture(const std::string& filePath, const sf::Vector2i& tileSize = { 16, 16 });

private:
    std::unordered_map<size_t, std::unique_ptr<sf::Texture>> textures;
    std::unordered_map<size_t, std::unique_ptr<sf::Font>> fonts;
    
    std::unordered_map<std::string, std::unique_ptr<Animation>> animations;
    std::unordered_map<std::string, std::unique_ptr<SpriteSheet>> spriteSheets;
    
    std::unordered_map<std::string, std::unique_ptr<sf::Texture>> combinedTextures;

    std::string GetRelativePath(const std::string& filePath);
    std::string relativePath;

    sf::Texture* LoadTexture(const std::string& filePath);
    sf::Font* LoadFont(const std::string& filePath);
};