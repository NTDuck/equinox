#include <SFML/Graphics.hpp>


int main() {
    auto window = sf::RenderWindow({ 640, 480 }, "equinox");

    sf::String s;
    window.setFramerateLimit(165);

    sf::Event event;

    while (window.isOpen()) {
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        window.display();
    }
}