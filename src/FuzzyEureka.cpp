#include "FuzzyEureka.h"
#include "Common/Input.h"
#include "Common/Debug.h"
#include <sstream>
#include <iostream>

#include "Engine/Components.h"
#include "Engine/RenderSystem.h"
#include "Engine/Engine.h"

#include "AStarSystem.h"

#include "Node.h"
#include "GameMessages.h"

FuzzyEureka::FuzzyEureka()
{
    
}

void LoadFromPearlyNoise(const int width, const int height, const sf::Vector2i& nodeSize,  Engine& engine) 
{
    // const siv::PerlinNoise::seed_type seed = 12345u;
    // const siv::PerlinNoise perlin{ seed };

    // int octaves = 8;
    // double frequency = 8.0f;

    // for(int x = 0; x < width; ++x)
    // {
    //     for(int y = 0; y < height; ++y)
    //     {
    //         const double fx = (frequency / image.width());
	// 	    const double fy = (frequency / image.height());

    //         const sf::Color color(perlin.octave2D_01((x * fx), (y * fy), octaves));



    //         Entity entity = engine.CreateEntity();
    //         engine.AddComponent(entity, TransformComponent{
    //             .position = sf::Vector2f(x * nodeSize.x, y * nodeSize.y),
    //         });
    //         engine.AddComponent(entity, SpriteComponent{
    //             .texture = sf::Texture("resources/Ground/Grass.png"),
    //             .color = color
    //         })
    //     }
    // }
}

void FuzzyEureka::Start()
{
    engine.RegisterComponent<TransformComponent>(); 
    engine.RegisterComponent<SpriteComponent>();
    engine.RegisterComponent<AStarComponent>();
    
    engine.RegisterSystem<RenderSystem>();

    int width = 16;
    int height = 16;
    sf::Vector2i nodeSize{16, 16};
    grid = std::make_shared<Grid>(width, height, nodeSize);


    pathfinding = engine.RegisterSystem<AStarSystem>(messageQueue, *(grid.get())).get();

    std::vector<Entity> entities;

    monster = engine.CreateEntity();
    engine.AddComponent(monster, TransformComponent{
        .position = sf::Vector2f(0,0),
        .size = sf::Vector2f(32, 32),
        .angle = sf::degrees(0),
    });
    // "resources/Characters/Monsters/Slimes/KingSlimeBlue.png",  false, sf::IntRect({0,0}, {16, 16}
    engine.AddComponent(monster, SpriteComponent{
        .texture = sf::Texture("resources/Buildings/Wood/Keep.png", false, sf::IntRect({0,0}, {32, 32})),
        .sortLayer = 1
    });
    engine.AddComponent(monster, AStarComponent{});

    auto windowSize = GetWindowSize();

    camera.setSize(sf::Vector2f(windowSize.x, windowSize.y));
    camera.setCenter(sf::Vector2f(windowSize.x / 2.0f, windowSize.y / 2.0f));
    camera.setViewport(sf::FloatRect({0.f, 0.f}, {1.f, 1.f}));
}

void FuzzyEureka::Destroy()
{

}

void FuzzyEureka::Update(const float deltaTime)
{
    auto& keyboard = GetInput().GetKeyboard();
    
    int horizontal = keyboard.HorizontalAxis();
    int vertical = keyboard.VerticalAxis();

    if (horizontal != 0 || vertical != 0)
    {
        sf::Vector2f move(horizontal, vertical);
        constexpr float speed = 100.0f;
        move *= speed * deltaTime; // Adjust speed as needed
        camera.move(move);
    }

    auto& mouse = GetInput().GetMouse();
    if(mouse.IsMouseButtonPressed(sf::Mouse::Button::Left))
    {
        const auto& position = mouse.GetMousePosition(GetWindow());

        Node* node = grid->NodeFromWorldPosition({(float)position.x, (float)position.y});
       
        messageQueue.Send<MoveEntity>(node, monster);
    }
    if(mouse.IsMouseButtonPressed(sf::Mouse::Button::Right))
    {
        const auto& position = mouse.GetMousePosition(GetWindow());
        Node* node = grid->NodeFromWorldPosition({(float)position.x, (float)position.y});
        if(!node->IsLocked())
            grid->Lock(node);
        else
            grid->Unlock(node);
        
    }
}

void FuzzyEureka::Render(sf::RenderWindow& window)
{
    window.setView(camera);
    grid->Render(window);


    // currentTime = clock.getElapsedTime();
    // float fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds()); // the asSeconds returns a float
    // sf::Text text(font);
    // text.setFillColor(sf::Color::White);
    // text.setCharacterSize(128);
    // text.setString(std::to_string(fps));
    // window.draw(text);

    // grid.Render(window);
}

void FuzzyEureka::HandleEvent(const sf::Event& event)
{
 
}