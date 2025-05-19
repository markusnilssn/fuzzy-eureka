#include "Application.h"
#include "Common/Input.h"
#include "Common/Debug.h"

Application::Application() 
{

}

void Application::Run(const Settings& settings) 
{
    Debug::Log("Hello World!");

    windowSize = settings.windowSize;

    sf::VideoMode videoMode(settings.windowSize);
    sf::RenderWindow window(videoMode, settings.title);

    window.setVerticalSyncEnabled(true); // Enable VSync
    window.setFramerateLimit(144);

    Start();

    while (window.isOpen())
    {   
        float deltaTime = clock.restart().asSeconds();

        HandleEvents(window);
        auto& input = Input::GetInstance();
        input.Update();
        Update(deltaTime);
        // Rendering!
        window.clear(sf::Color::Black);
        Render(window);
        window.display();
    }

    Destroy();
}

const sf::Vector2u &Application::GetWindowSize() const
{
    return windowSize;
}

void Application::HandleEvents(sf::RenderWindow &window)
{
    // Manage core window events here!
    while (const std::optional<sf::Event> event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
        
        // HandleEvent(event); // Error due to std::optional and raw sf::Event
    }
}