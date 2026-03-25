#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    std::cout << "Starting game..." << std::endl;

    sf::RenderWindow window(sf::VideoMode({800, 600}), "Galaxy Defenders");
    std::cout << "Window created." << std::endl;

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                std::cout << "Window closed." << std::endl;
            }
        }

        window.clear(sf::Color::Black);
        window.display();
    }

    std::cout << "Game exited." << std::endl;
    return 0;
}

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    return main();
}
#endif