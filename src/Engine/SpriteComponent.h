#pragma once
#include <SFML/Graphics.hpp>    
#include <SFML/System.hpp>

#include <unordered_map>
#include <functional>
#include <vector>
#include <iostream>

enum struct Read
{
    LeftToRight,
    RightToLeft,
    UpToDown,
    DownToUp,
};

namespace Animation
{
    namespace Walk
    {
        inline constexpr const char* Left = "walk-left";
        inline constexpr const char* Right = "walk-left";
        inline constexpr const char* Up = "walk-left";
        inline constexpr const char* Down = "walk-left";
    }

    namespace Action {}
}

// Cannot be RAII 
struct SpriteSheet final
{
    using Sequence = std::vector<sf::IntRect>;

    sf::Texture texture;
    sf::Vector2i clipSize;
    std::unordered_map<size_t, Sequence> sequences;

    void LoadTexture(const std::string& filePath, const sf::Vector2i clipSize)
    {
        bool success = texture.loadFromFile(filePath, false);
        Debug::Assert(success, "Failed to load sprite sheet");
        this->clipSize = clipSize;
    }

    void Cut(const std::string& name, const sf::Vector2i& start, const sf::Vector2i& end) 
    {
        size_t hashCode = std::hash<std::string>{}(name);

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
        
        sequences.insert({hashCode, std::move(sequence)});
    }

    const Sequence& GetSequence(const std::string& name)
    {
        size_t hashCode = std::hash<std::string>{}(name);

        return sequences[hashCode];
    }
};

// struct Time final
// {
//     [[nodiscard]] float AsSeconds() noexcept;
//     [[nodiscard]] float AsMilliseconds() noexcept;
//     [[nodiscard]] float AsMinutes() noexcept;
    
//     float seconds;
// };

struct AnimatorComponent 
{
    SpriteSheet spriteSheet;
    std::string animation;
    bool loop;
    size_t currentFrame = 0;
    float speed = 1.0f;
    float tick = 0;
    // Time time;
    bool animating = false;
};

struct SpriteComponent 
{
    sf::Texture texture;
    uint_fast16_t sortLayer;
    sf::Color color = sf::Color::White;
};
