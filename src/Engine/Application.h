#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <optional>
#include <string>
#include "Engine/Engine.h"
#include "Common/Input.h"

class Application 
{
public:
    struct Settings 
    {
        std::string title;
        sf::Vector2u windowSize;
        unsigned int framelimit;
        bool vsync;
    };

public:
    Application();
    virtual ~Application();

    void Run(const Settings& settings);
    
    [[nodiscard]] const sf::Vector2u& GetWindowSize() const;

protected:
    virtual void Start() {}
    virtual void Destroy() {}
    virtual void Update(const float deltaTime) {}
    virtual void Render(sf::RenderWindow& window) {}
    virtual void HandleEvent(const sf::Event& event) {}

    sf::RenderWindow& GetWindow();
    Input& GetInput();

    Engine engine;
private:
    void HandleEvents(sf::RenderWindow& window);
    std::unique_ptr<Input> input;
    std::unique_ptr<sf::RenderWindow> window;

    sf::Vector2u windowSize;

    sf::Clock clock;
};