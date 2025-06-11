#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <optional>
#include <string>
#include "Engine/Engine.h"
#include "Common/Input.h"

#include "MessageQueue.h"
#include "Scene.h"
#include "Concurrency.h"

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
    virtual void HandleEvent(const std::optional<sf::Event>& event) {}

    sf::RenderWindow& GetWindow();
    Input& GetInput();
    Concurrency& GetConcurrency();

    Engine engine;
    MessageQueue messageQueue;
private:
    void HandleEvents(sf::RenderWindow& window);
    std::unique_ptr<Input> input;
    std::unique_ptr<sf::RenderWindow> window;
    std::unique_ptr<Concurrency> concurrency;

    std::unordered_map<size_t, std::unique_ptr<Scene>> scenes; // Manage scenes! 
    // Scene& activeScene;

    template<typename T>
    void Push()
    {
        // static_assert(std::is_base_of<Scene, T>::value, "T is not derived from Scene");

        // size_t hashCode = typeid(T).hash_code();

        // auto iterator = scenes.find(hashCode);
        // if(iterator != scenes.end())
        // {
        //     activeScene = *(*iterator->second);
        // }
    }

    sf::Vector2u windowSize;

    sf::Clock clock;
};