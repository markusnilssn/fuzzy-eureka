#include "Application.h"
#include "Common/Input.h"
#include "Common/Debug.h"

// #include "imgui.h"
// #include "imgui-SFML.h"

Application::Application() 
{

}

Application::~Application() 
{
    engine.Destroy();
}

void Application::Run(const Settings& settings) 
{
    Debug::Log("Hello World!");

    windowSize = settings.windowSize;

    sf::VideoMode videoMode(settings.windowSize);
    window = std::make_unique<sf::RenderWindow>(videoMode, settings.title);
    input = std::make_unique<Input>(*(window.get()));

    window->setVerticalSyncEnabled(true); // Enable VSync
    window->setFramerateLimit(144);

    engine.Start();
    Start();

    while (window->isOpen())
    {   
        float deltaTime = clock.restart().asSeconds();

        HandleEvents(*window);  
        input->Update();
        engine.Update(deltaTime);
        Update(deltaTime);

        // Rendering!
        window->clear(sf::Color::Black);
        Render(*window);
        engine.Render(*window);
        window->display();
    }

    engine.Destroy();
    Destroy();
}

const sf::Vector2u &Application::GetWindowSize() const
{
    return windowSize;
}

sf::RenderWindow &Application::GetWindow()
{
    return *(window.get());
}

Input &Application::GetInput()
{
    return *(input.get());
}

void Application::HandleEvents(sf::RenderWindow &window)
{
    // Manage core window events here!
    while (const std::optional<sf::Event> event = window.pollEvent())
    {
        // ImGui::SFML::ProcessEvent(window, *event);

        if (event->is<sf::Event::Closed>())
            window.close();
        if(event->is<sf::Event::FocusGained>()) {}
        if(event->is<sf::Event::FocusLost>()) {}
        
        // HandleEvent(event); // Error due to std::optional and raw sf::Event
    }
}