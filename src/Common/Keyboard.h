#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <unordered_map>
#include <unordered_set>

class Keyboard final
{   
    friend class Input;
public:
    void RegisterKey(sf::Keyboard::Key key);
    void UnregisterKey(sf::Keyboard::Key key);

    const bool IsKeyDown(sf::Keyboard::Key key);
    const bool IsKeyUp(sf::Keyboard::Key key);
    const bool IsKeyPressed(sf::Keyboard::Key key);

    const int HorizontalAxis();
    const int VerticalAxis();

private:
    Keyboard(sf::RenderWindow& window);
    
    std::unordered_set<sf::Keyboard::Key> registeredKeys;

    std::unordered_map<sf::Keyboard::Key, bool> currentState;
    std::unordered_map<sf::Keyboard::Key, bool> previousState;

    sf::RenderWindow& window;

    void Update();

};

