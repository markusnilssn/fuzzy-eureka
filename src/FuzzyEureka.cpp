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
    engine.RegisterComponent<NavigationComponent>();
    engine.RegisterComponent<ObjectComponent>();
    engine.RegisterComponent<AnimatorComponent>();
    
    engine.RegisterSystem<RenderSystem>();

    int width = 16;
    int height = 16;
    sf::Vector2i nodeSize{16, 16};
    grid = std::make_shared<Grid>(width, height, nodeSize);

    engine.RegisterSystem<AStarSystem>(messageQueue, *(grid.get())).get();

    std::vector<Entity> entities;

    {
        building = engine.CreateEntity();
        engine.AddComponent(building, TransformComponent{
            .position = sf::Vector2f(nodeSize.x * 5, nodeSize.y * 5),
            .size = sf::Vector2f(32, 32),
            .angle = sf::degrees(0),
        });
        engine.AddComponent(building, SpriteComponent{
            .texture = sf::Texture("resources/Buildings/Wood/Keep.png", false, sf::IntRect({0,0}, {32, 32})),
            .sortLayer = 1
        });
    }
    {
        animal = engine.CreateEntity();
        engine.AddComponent(animal, TransformComponent{
            .position = sf::Vector2f(0,0),
            .size = sf::Vector2f(nodeSize.x, nodeSize.y),
            .angle = sf::degrees(0),
        });

        SpriteSheet boar;
        boar.LoadTexture("resources/Animals/Boar.png", sf::Vector2i(16, 16));
        boar.Cut(Animation::Walk::Down, sf::Vector2i(0,0), sf::Vector2i(3,0));
        boar.Cut(Animation::Walk::Up, sf::Vector2i(0,1), sf::Vector2i(3,1));
        boar.Cut(Animation::Walk::Right, sf::Vector2i(0,2), sf::Vector2i(3,2));
        boar.Cut(Animation::Walk::Left, sf::Vector2i(0,3), sf::Vector2i(3,3));
        
        engine.AddComponent(animal, AnimatorComponent{
            .animation = Animation::Walk::Down,
            .spriteSheet = boar,
            .loop = true,
            // .speedInSeconds = 0.4f,
        });
        engine.AddComponent(animal, NavigationComponent{});
        engine.AddComponent(animal, ObjectComponent{});
    }

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
       
        messageQueue.Send<MoveEntity>(node, animal);
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