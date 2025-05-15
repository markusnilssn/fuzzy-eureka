#include "application.h"

int main()
{
    std::string title = "Dennis + Markus";
    sf::Vector2u resolution(1280, 720);

    Application app(title, resolution);
    app.Run();

    return EXIT_SUCCESS;    
}
