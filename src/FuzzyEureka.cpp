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
#include "PearlyNoise.hpp"

#include <optional>
#include <set>

FuzzyEureka::FuzzyEureka()
    : cliffWater("resources/Ground/Cliff-Water.png", 16)
    , cliff("resources/Ground/Cliff.png", 16)
    , grass("resources/Ground/Grass.png", 16)
    , shore("resources/Ground/Shore.png", 16)
    , winter("resources/Ground/Winter.png", 16)
{
    
}

FuzzyEureka::~FuzzyEureka()
{

}

void FuzzyEureka::LoadFromPearlyNoise(const int width, const int height) 
{
    // static sf::Color lightblue(0,191,255);
    // static sf::Color blue(65,105,225);
    // static sf::Color green(34,139,34);
    // static sf::Color darkgreen(0,100,0);
    // static sf::Color sandy(210,180,140);
    // static sf::Color beach (238, 214, 175);
    // static sf::Color snow(255, 250, 250);
    // static sf::Color mountain (139, 137, 137);

    constexpr int tileSize = 16;

    float threshold  = 0.0f;
    float scale = 0.01f;

    const siv::PerlinNoise::seed_type seed = 12345u;
    const siv::PerlinNoise perlin{ seed };


    auto isDeepWater = [](float noise) { return noise < 0.455; };
    auto isNormalWater = [](float noise) { return noise < 0.465; };
    auto isShallowWater = [](float noise) { return noise < 0.475; };
    auto isShore = [](float noise) { return noise < 0.480; };
    auto isSand = [](float noise) { return noise < 0.51; };
    auto isShallowGrass = [](float noise) { return noise < 0.6; };
    auto isDeepGrass = [](float noise) { return noise < 0.7; };
    auto isMountain = [](float noise) { return noise >= 0.7f; };

    constexpr float Invalid = -1.0f;

    auto isGrass = [&](float noise) -> bool
    {
        return isShallowGrass(noise) || isDeepGrass(noise);
    };

    auto isEdge = [&](float noise) -> bool {
        return (isGrass(noise) && noise > Invalid); // Anything not mountain
    };

    auto isWater = [&](float noise) -> bool
    {
        return isShallowWater(noise) || isNormalWater(noise) || isDeepWater(noise);
    };

    // Generate
    float** noises = new float*[width];
    for(int_fast32_t  x = 0; x < width; x++)
    {
        noises[x] = new float[height];
        for (int_fast32_t  y = 0; y < height; y++)
        {
            float xpos = x * 0.01f;
            float ypos = y * 0.01f;
            float octaves = 6;
            float persistence = 0.5;
            // float lacunarity = 2.0;

            noises[x][y] = perlin.octave2D_01(xpos, ypos, octaves, persistence);
        }
    }

    // Cleanup

    auto Smooth = [=]()
    {
        for (int x = 0; x < width; ++x) 
        {
            for (int y = 0; y < height; y++)
            {
                // fix mountains  
                if(isMountain(noises[x][y]))
                {
                    auto GetTile = [&](int dx, int dy) -> float {
                        int nx = x + dx;
                        int ny = y + dy;
                        if (nx < 0 || nx >= width || ny < 0 || ny >= height) return Invalid;
                        return noises[nx][ny];
                    };

                    auto up = GetTile(0, -1);
                    auto down = GetTile(0, 1);
                    auto left = GetTile(-1, 0);
                    auto right = GetTile(1, 0);
                    
                    auto upLeft = GetTile(-1, -1);
                    auto upRight = GetTile(1, -1);
                    auto downLeft = GetTile(-1, 1);
                    auto downRight = GetTile(1, 1);

                    if(isEdge(up) && isEdge(down) && isEdge(left) && isEdge(right) && isEdge(upLeft) && isEdge(upRight) && isEdge(downLeft) && isEdge(downRight))  noises[x][y] = 0.6;
                    
                    if(isEdge(up) && isEdge(left) && isEdge(right) && !isEdge(down))  noises[x][y] = 0.6;
                    if(isEdge(up) && isEdge(left) && !isEdge(right) && isEdge(down))  noises[x][y] = 0.6;
                    if(isEdge(up) && !isEdge(left) && isEdge(right) && isEdge(down))  noises[x][y] = 0.6;
                    if(!isEdge(up) && isEdge(left) && isEdge(right) && isEdge(down))  noises[x][y] = 0.6;
                }
            }
        }
    };

    int_fast32_t  amountToSmooth = 3;
    for(int_fast32_t  i = 0; i < amountToSmooth; i++)
        Smooth();

    
    // Finalize
    for (int x = 0; x < width; ++x) 
    {
        for (int y = 0; y < height; y++)
        {
            auto GetTile = [&](int dx, int dy) -> float {
                int nx = x + dx;
                int ny = y + dy;
                if (nx < 0 || nx >= width || ny < 0 || ny >= height) return Invalid;
                return noises[nx][ny];
            };

            auto up = GetTile(0, -1);
            auto down = GetTile(0, 1);
            auto left = GetTile(-1, 0);
            auto right = GetTile(1, 0);
            
            auto upLeft = GetTile(-1, -1);
            auto upRight = GetTile(1, -1);
            auto downLeft = GetTile(-1, 1);
            auto downRight = GetTile(1, 1);

            auto isMountainEdge = [&](float noise)
            {
                auto occupy = [&](float noise)
                {
                    return isGrass(noise) && noise != Invalid;
                };

                return isMountain(noise) && (occupy(up) || occupy(down) || occupy(left) || occupy(right) || occupy(upLeft) || occupy(upRight) || occupy(downRight) || occupy(downLeft));
            };

            auto createEnvironment = [&](float noise) -> sf::Sprite
            {
                if (isDeepWater(noise)) return shore.CreateSprite(4, 0);
                if (isNormalWater(noise)) return shore.CreateSprite(3, 0);
                if (isShallowWater(noise)) return shore.CreateSprite(2, 0);
                if (isShore(noise)) return shore.CreateSprite(1, 0);
                if (isSand(noise)) return shore.CreateSprite(0, 0);
                if (isShallowGrass(noise)) return grass.CreateSprite(1, 0);
                if (isDeepGrass(noise)) return grass.CreateSprite(2, 0);
                else 
                {
                    auto background = grass.CreateSprite(2, 0);

                    // Interface map (direction to sprite index)
                    if (isEdge(up) && !isEdge(down) && !isEdge(left) && !isEdge(right)) return cliff.CreateSpriteWitBackground(1, 0, background); // top
                    if (isEdge(down) && !isEdge(up) && !isEdge(left) && !isEdge(right)) return cliff.CreateSpriteWitBackground(1, 2, background); // bottom
                    if (isEdge(left) && !isEdge(up) && !isEdge(down) && !isEdge(right)) return cliff.CreateSpriteWitBackground(0, 1, background); // left
                    if (isEdge(right) && !isEdge(up) && !isEdge(down) && !isEdge(left)) return cliff.CreateSpriteWitBackground(2, 1, background); // right

                    if (isEdge(up) && isEdge(left))        return cliff.CreateSpriteWitBackground(0, 0, background);
                    if (isEdge(up) && isEdge(right))       return cliff.CreateSpriteWitBackground(2, 0, background);
                    if (isEdge(down) && isEdge(left))      return cliff.CreateSpriteWitBackground(0, 2, background);
                    if (isEdge(down) && isEdge(right))     return cliff.CreateSpriteWitBackground(2, 2, background);

                    if (isEdge(upLeft))               return cliff.CreateSpriteWitBackground(3, 2, background); // top-left edge
                    if (isEdge(upRight))              return cliff.CreateSpriteWitBackground(4, 2, background); // top-right edge
                    if (isEdge(downLeft))             return cliff.CreateSpriteWitBackground(3, 3, background); // bottom-left edge
                    if (isEdge(downRight))            return cliff.CreateSpriteWitBackground(4, 3, background); // bottom-right edge

                    return cliff.CreateSprite(1, 1); // Default mountain
                }
            };

            auto noise = noises[x][y];

            if(isWater(noise) || isMountainEdge(noise))
                grid->Lock(grid->GetNodeAt(x, y));

            auto sprite = createEnvironment(noise);
            sprite.setPosition(sf::Vector2f(x*nodeSize.x, y *nodeSize.y));
            world.emplace_back(std::move(sprite));
        }
    }

    for(int i = 0; i < width; ++i) 
        delete[] noises[i];   
    delete[] noises;
}

void FuzzyEureka::Start()
{
    width = 256;
    height = 256;
    nodeSize = sf::Vector2i{16, 16};
    
    engine.RegisterComponent<TransformComponent>(); 
    engine.RegisterComponent<SpriteComponent>();
    engine.RegisterComponent<NavigationComponent>();
    engine.RegisterComponent<ObjectComponent>();
    engine.RegisterComponent<AnimatorComponent>();
    
    engine.RegisterSystem<RenderSystem>(nodeSize);


    grid = std::make_shared<Grid>(width, height, nodeSize);
    LoadFromPearlyNoise(width, height);

    engine.RegisterSystem<AStarSystem>(messageQueue, *(grid.get()), GetConcurrency(), GetInput(), GetWindow());

    std::vector<Entity> entities;
    {
        building = engine.CreateEntity();
        engine.AddComponent(building, TransformComponent{
            .position = sf::Vector2f(nodeSize.x * 53, nodeSize.y * 18),
            .size = sf::Vector2f(32, 32),
            .angle = sf::degrees(0),
        });
        engine.AddComponent(building, SpriteComponent{
            .texture = sf::Texture("resources/Buildings/Wood/Keep.png", false, sf::IntRect({0,0}, {32, 32})),
            .sortLayer = 1
        });
        
        engine.AddComponent(building, NavigationComponent{});
        engine.AddComponent(building, ObjectComponent{});
    }
    {
        animal = engine.CreateEntity();
        engine.AddComponent(animal, TransformComponent{
            .position = sf::Vector2f(48 * nodeSize.x,18 * nodeSize.y),
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
        constexpr float speed = 600.0f;
        move *= speed * deltaTime; // Adjust speed as needed
        camera.move(move);
    }

    // auto& mouse = GetInput().GetMouse();
    // if(mouse.IsMouseButtonPressed(sf::Mouse::Button::Left))
    // {

    // }
    // if(mouse.IsMouseButtonPressed(sf::Mouse::Button::Right))
    // {

    //     const auto& position = mouse.GetMousePosition(GetWindow());

    //     Node* node = grid->NodeFromAbsolutePosition({(float)position.x, (float)position.y});
        
    //     if(node != nullptr)
    //     {
    //         messageQueue.Send<MoveEntity>(node, selectedEntity);
    //     }
    //     // const auto& position = mouse.GetMousePosition(GetWindow());
    //     // Node* node = grid->NodeFromAbsolutePosition({(float)position.x, (float)position.y});
    //     // if(node != nullptr)
    //     // {
    //     //     if(!node->IsLocked())
    //     //     {
    //     //         grid->Lock(node);
    //     //     }
    //     //     else
    //     //     {
    //     //         grid->Unlock(node);
    //     //     }
    //     // }   
    // }
}

void FuzzyEureka::Render(sf::RenderWindow& window)
{
    window.setView(camera);

    const sf::View& view = window.getView();
    sf::Vector2f offset(nodeSize.x, nodeSize.y);
    sf::Vector2f size = view.getSize() + offset;
    sf::Vector2f center = (view.getCenter() - size / 2.0f) - offset;
    sf::FloatRect viewport(center, size);

    int length = width * height;
    for (int index = 0; index < length; ++index) 
    {
        int x = index / height;
        int y = index % height;
        
        if(!viewport.contains(world[index].getPosition()))
        {
            continue;
        }

        window.draw(world[index]);
    }

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

void FuzzyEureka::HandleEvent(const std::optional<sf::Event>& event)
{
    if (auto mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
    {
        float zoomFactor = 1.1f;

        if (mouseWheelScrolled->delta > 0.0f)
            camera.zoom(1.0f / zoomFactor);
        else if (mouseWheelScrolled->delta < 0.0f)
            camera.zoom(zoomFactor);
    }
}