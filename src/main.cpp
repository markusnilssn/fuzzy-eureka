#include "FuzzyEureka.h"

int main()
{
    FuzzyEureka::Settings settings;
    settings.title = "Application";
    settings.windowSize = sf::Vector2u(1280, 720);
    settings.framelimit = 60u;
    settings.vsync = true;

    FuzzyEureka app{};
    app.Run(settings);

    return EXIT_SUCCESS;    
}
