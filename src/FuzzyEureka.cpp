#include "FuzzyEureka.h"
#include "Common/Input.h"
#include "Common/Debug.h"
#include <sstream>
#include <iostream>

FuzzyEureka::FuzzyEureka()
{
    
}

void FuzzyEureka::Start()
{
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
        constexpr float speed = 5.0f;
        move *= speed * deltaTime; // Adjust speed as needed
        camera.move(move);

        std::stringstream ss;
        ss << "Camera position: " << camera.getCenter().x << ", " << camera.getCenter().y;
        Debug::Log(ss.str().c_str());
    }
}

void FuzzyEureka::Render(sf::RenderWindow& window)
{
    window.setView(camera);
    // Render logic here
}

void FuzzyEureka::HandleEvent(const sf::Event& event)
{
 
}