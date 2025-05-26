#include "FuzzyEureka.h"
#include "Common/Input.h"
#include "Common/Debug.h"
#include <sstream>
#include <iostream>

#include "Engine/Components.h"
#include "Engine/RenderSystem.h"
#include "Engine/Engine.h"

#include "Node.h"

FuzzyEureka::FuzzyEureka()
    : font()
{
    
}


void LoadFromPearlyNoise(const siv::PerlinNoise::seed_type seed, int width, int height, Engine& engine, const sf::Vector2i& nodeSize)
{
        const siv::PerlinNoise   perlin{ seed };
        const siv::PerlinNoise   lakeMaskNoise{ seed + 1 };
        const double freq        = 0.12, lac = 2.5, gain = 0.5;
        const size_t octs        = 5;
        const double elevThresh  = -0.2;
        const double lakeFreq    = 0.4;
        const double lakeMaskMin = 0.2;
std::stringstream ss;
        for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            double nx = double(x)/width, ny = double(y)/height;
            // fBm height
            double amp=1, f=freq, sum=0, aSum=0;
            for (size_t o=0;o<octs;++o){
            sum  += perlin.noise2D(f*nx,f*ny)*amp;
            aSum += amp;
            amp  *= gain; f *= lac;
            }
            double elevation = sum/aSum;
            // lake mask
            double lm = lakeMaskNoise.noise2D(lakeFreq*nx, lakeFreq*ny);

            bool isLake = (elevation < elevThresh) && (lm > lakeMaskMin);
            int col = isLake ? 0 : 2;

            int column = (isLake) ? 2 : 0;
            int row = 0;

            Entity entity = engine.CreateEntity();
            sf::Vector2f position(x * nodeSize.x, y * nodeSize.y);
            engine.AddComponent(entity, TransformComponent{
                .position = position,
            });
            std::string filePath = "resources/Ground/Grass.png";
            sf::IntRect source({nodeSize.x * column, nodeSize.y * row}, nodeSize);

            engine.AddComponent(entity, SpriteComponent{
                .texture = sf::Texture(filePath, false, source),
                .sortLayer = 0,
            });

            ss << std::to_string(elevation);
            ss << " ";
        }

        ss << "\n";
    }
}

void FuzzyEureka::Start()
{
    engine.RegisterComponent<TransformComponent>(); 
    engine.RegisterComponent<SpriteComponent>();
    
    engine.RegisterSystem<RenderSystem>();

    int width = 64;
    int height = 64;
    sf::Vector2i nodeSize{16, 16};
    // Grid grid(width, height, nodeSize);
    LoadFromPearlyNoise(123456u, width, height, engine, nodeSize);

    Entity entity = engine.CreateEntity();
    engine.AddComponent(entity, TransformComponent{
        .position = sf::Vector2f(0,0)
    });
    engine.AddComponent(entity, SpriteComponent{
        .texture = sf::Texture("resources/Buildings/Wood/Barracks.png", false, sf::IntRect({0,0}, {16, 16})),
        .sortLayer = 1
    });

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
    auto& keyboard = Input::GetInstance().GetKeyboard();

    int horizontal = keyboard.HorizontalAxis();
    int vertical = keyboard.VerticalAxis();

    if (horizontal != 0 || vertical != 0)
    {
        sf::Vector2f move(horizontal, vertical);
        constexpr float speed = 100.0f;
        move *= speed * deltaTime; // Adjust speed as needed
        camera.move(move);

        // std::string toString = "" + std::to_string(camera.getCenter().x)+ " " + std::to_string(camera.getCenter().y);
        // Debug::Log(toString);
    }
}

void FuzzyEureka::Render(sf::RenderWindow& window)
{
    window.setView(camera);

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