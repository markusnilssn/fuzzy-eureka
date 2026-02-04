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

#include "World.h"

#include <optional>
#include <set>

FuzzyEureka::FuzzyEureka()
// : cliffWater("resources/Ground/Cliff-Water.png", 16)
// , cliff("resources/Ground/Cliff.png", 16)
// , grass("resources/Ground/Grass.png", 16)
// , shore("resources/Ground/Shore.png", 16)
// , winter("resources/Ground/Winter.png", 16)
{
    sf::Vector2i x16(16, 16);
    sf::Vector2i x64(64, 64);

    // Ground
    content.PreloadTextureSheet("Ground/Cliff-Water.png", x16);
    content.PreloadTextureSheet("Ground/Cliff.png", x16);
    content.PreloadTextureSheet("Ground/DeadGrass.png", x16);
    content.PreloadTextureSheet("Ground/Grass.png", x16);
    content.PreloadTextureSheet("Ground/Shore.png", x16);
    content.PreloadTextureSheet("Ground/TexturedGrass.png", x16);
    content.PreloadTextureSheet("Ground/Winter.png", x16);

    content.PreloadTextureSheet("Buildings/Wood/Barracks.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/CaveV2.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/Chapels.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/Docks.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/Houses.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/Huts.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/Keep.png", x64);
    content.PreloadTextureSheet("Buildings/Wood/Market.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/Resources.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/Taverns.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/Tower.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/Tower2.png", x16);
    content.PreloadTextureSheet("Buildings/Wood/Workshops.png", x16);

    content.PreloadTextureSheet("Nature/Cactus.png", x16);
    content.PreloadTextureSheet("Nature/CoconutTrees.png", x16);
    content.PreloadTextureSheet("Nature/DeadTrees.png", x16);
    content.PreloadTextureSheet("Nature/PineTrees.png", x16);
    content.PreloadTextureSheet("Nature/Rocks.png", x16);
    content.PreloadTextureSheet("Nature/Trees.png", x16);
    content.PreloadTextureSheet("Nature/TumbleWeed.png", x16);
    content.PreloadTextureSheet("Nature/Wheatfield.png", x16);
    content.PreloadTextureSheet("Nature/WinterDeadTrees.png", x16);
    content.PreloadTextureSheet("Nature/WinterTrees.png", x16);

    content.PreloadTextureSheet("Templates/16x16Large.png", x16);
    content.PreloadTextureSheet("Templates/16x16Small.png", x16);
    content.PreloadTextureSheet("Templates/32x32Small.png", x16);

    content.PreloadTextureSheet("Characters/Workers/FarmerTemplate.png", x16);

    content.PreloadTextureSheet("Characters/Soldiers/Melee/AxemanTemplate.png", x16);
    content.PreloadTextureSheet("Characters/Soldiers/Melee/SpearmanTemplate.png", x16);
    content.PreloadTextureSheet("Characters/Soldiers/Melee/SwordsmanTemplate.png", x16);

    content.PreloadTextureSheet("Characters/Soldiers/Ranged/BowmanTemplate.png", x16);
    content.PreloadTextureSheet("Characters/Soldiers/Ranged/MageTemplate.png", x16);
    content.PreloadTextureSheet("Characters/Soldiers/Ranged/Ballista.png", x16);

    content.PreloadTextureSheet("User Interface/Highlighted-Boxes.png", x16);
    content.PreloadTextureSheet("User Interface/BoxSelector.png", x16);
    content.PreloadTextureSheet("User Interface/UiIcons.png", x16);
}

FuzzyEureka::~FuzzyEureka()
{
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
    // LoadFromPearlyNoise(width, height);

    world = std::make_shared<World>(content, *grid.get(), width, height, nodeSize);

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
                                          .texture = sf::Texture("resources/Buildings/Wood/Keep.png", false, sf::IntRect({0, 0}, {32, 32})),
                                          .sortLayer = 1});

        engine.AddComponent(building, NavigationComponent{});
        engine.AddComponent(building, ObjectComponent{});
    }
    {
        animal = engine.CreateEntity();
        engine.AddComponent(animal, TransformComponent{
                                        .position = sf::Vector2f(48 * nodeSize.x, 18 * nodeSize.y),
                                        .size = sf::Vector2f(nodeSize.x, nodeSize.y),
                                        .angle = sf::degrees(0),
                                    });

        SpriteSheet boar;
        boar.LoadTexture("resources/Animals/Boar.png", sf::Vector2i(16, 16));
        boar.Cut(Animation::Walk::Down, sf::Vector2i(0, 0), sf::Vector2i(3, 0));
        boar.Cut(Animation::Walk::Up, sf::Vector2i(0, 1), sf::Vector2i(3, 1));
        boar.Cut(Animation::Walk::Right, sf::Vector2i(0, 2), sf::Vector2i(3, 2));
        boar.Cut(Animation::Walk::Left, sf::Vector2i(0, 3), sf::Vector2i(3, 3));

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
    auto &keyboard = GetInput().GetKeyboard();

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

void FuzzyEureka::Render(sf::RenderWindow &window)
{
    window.setView(camera);

    world->Render(window);
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

void FuzzyEureka::HandleEvent(const std::optional<sf::Event> &event)
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