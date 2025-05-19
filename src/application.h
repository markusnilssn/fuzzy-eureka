#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <optional>
#include <string>

class Application {
public:
    Application(const std::string& title, const sf::Vector2u& size);

    void Run();

private:
    void HandleEvents(sf::RenderWindow& window);
    void Update(const float deltaTime);
    void Render(sf::RenderWindow& window);

    std::string title;
    sf::Vector2u size;

    sf::View camera;
    sf::Clock clock;

};