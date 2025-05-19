#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <optional>
#include <string>

class Application 
{
public:
    struct Settings 
    {
        std::string title;
        sf::Vector2u windowSize;
    };

public:
    Application();

    void Run(const Settings& settings);
    
    [[nodiscard]] const sf::Vector2u& GetWindowSize() const;

protected:
    virtual void Start() {}
    virtual void Destroy() {}
    virtual void Update(const float deltaTime) {}
    virtual void Render(sf::RenderWindow& window) {}
    virtual void HandleEvent(const sf::Event& event) {}

private:
    void HandleEvents(sf::RenderWindow& window);
    // std::string title;
    sf::Vector2u windowSize;

    sf::Clock clock;
};