#include "application.h"

Application::Application(const std::string& title, const sf::Vector2u& size) 
    : title(title)
    , size(size)
{

}

void Application::Run() 
{
    sf::VideoMode videoMode(size);
    sf::RenderWindow window(videoMode, title);

    window.setVerticalSyncEnabled(true); // Enable VSync
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        HandleEvents(window);
        Update();
        window.clear();
        Render(window);
        window.display();
    }
}

void Application::HandleEvents(sf::RenderWindow& window)
{
    // Handle events here
    while (const std::optional event = window.pollEvent())
    {
        // Close window: exit
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void Application::Update()
{

}

void Application::Render(sf::RenderWindow& window) 
{
    // render color
    sf::RectangleShape rectangle(sf::Vector2f(100, 100));
    rectangle.setFillColor(sf::Color(255, 0, 0, 255));
    rectangle.setPosition(sf::Vector2f(0,0));

    // draw rectangle
    window.draw(rectangle);
}