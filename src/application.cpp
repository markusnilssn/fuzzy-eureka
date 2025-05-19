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

    camera = window.getDefaultView();

    while (window.isOpen())
    {   
        float deltaTime = clock.restart().asSeconds();
        HandleEvents(window);
        Update(deltaTime);
        window.clear(sf::Color::Black);
        window.setView(camera);
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

void Application::Update(const float deltaTime)
{
    
}

enum struct Tile {
    Grass, 
    Water
}

void Application::Render(sf::RenderWindow& window) 
{
    sf::Texture texture("Resources/Ground/TexturedGrass.png");

    // std::map<sf::Color, Tile> mappedColors {
    //     {sf::Color(0, 255, 0), Tile::Grass},
    //     {sf::Color(0, 0, 255), Tile::Water}
    // };

    // sf::Image map("Resources/Map.png");
    // map.get


    int width = 128;
    int height = 128;

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            sf::Sprite sprite(texture);

            int size = 16;
            int column = 0;
            int row = 0;
            
            sf::IntRect clipRect({column * size, row * size}, {size, size});
            sprite.setTextureRect(clipRect);
            sprite.setPosition({x * (float)size, y * (float)size});
            window.draw(sprite);
        }
    }
}