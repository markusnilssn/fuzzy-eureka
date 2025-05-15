#include "Application.h"
#include "ResourceManager.h"

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
        window.clear(sf::Color::Black);
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
    // auto resource = ResourceManager::GetInstance().GetTexture("Ground/TexturedGrass.png");
    
    sf::Texture resource("resources/Ground/TexturedGrass.png");
        
    sf::Vector2f size(16, 16);
    sf::Vector2u gridSize(16u, 16u);
    // draw grid
    for (int i = 0; i < gridSize.x; ++i)
    {
        for (int j = 0; j < gridSize.y; ++j)
        {
            sf::Sprite sprite(resource);
            sprite.setPosition(sf::Vector2f(i * size.x, j * size.y));
            window.draw(sprite);
        }
    }

}