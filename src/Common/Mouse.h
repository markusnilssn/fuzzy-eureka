#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <unordered_map>

class Mouse final
{
    friend class Input;
public:
    const bool IsMouseButtonPressed(sf::Mouse::Button button);
    const bool IsMouseButtonDown(sf::Mouse::Button button);
    const bool IsMouseButtonUp(sf::Mouse::Button button);

    const sf::Vector2i GetMousePosition();
    const sf::Vector2f GetMousePosition(const sf::RenderWindow& window);

private:
    Mouse(sf::RenderWindow& window);

    std::unordered_map<sf::Mouse::Button, bool> mouseButtonStates;
    std::unordered_map<sf::Mouse::Button, bool> previousMouseState;

    sf::RenderWindow& window;

    void RegisterMouseButton(sf::Mouse::Button button);

    sf::Vector2i mousePosition;
    void Update();

};
