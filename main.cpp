#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

void setPixel(sf::Image& image, int x, int y, const sf::Color& color) {
    if (x < 0 || x >= (int)image.getSize().x)
        return;
    if (y < 0 || y >= (int)image.getSize().y)
        return;
    
    image.setPixel(x, y, color);
}

void drawImage(sf::Image& image) {
    for (int i = 0; i < 200; i++) {
        setPixel(image, 100 + i, 100 + i, sf::Color::Green);
    }
    
    for (int x = 300; x < 310; x++) {
        for (int y = 200; y < 210; y++) {
            setPixel(image, x, y, sf::Color::Blue);
        }
    }
    
    for (int i = 0; i < 100; i++) {
        int x = rand() % WINDOW_WIDTH;
        int y = rand() % WINDOW_HEIGHT;
        setPixel(image, x, y, sf::Color(
            rand() % 256,
            rand() % 256,
            rand() % 256
        ));
    }
}

int main() {
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
        "Ray Tracing iheay"
    );
    
    sf::Image image;
    image.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::Black);

    
    
    sf::Texture texture;
    texture.loadFromImage(image);
    
    sf::Sprite sprite(texture);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        
        texture.update(image);
        
        window.clear();
        
        window.draw(sprite);
        
        window.display();
        
        sf::sleep(sf::milliseconds(100));
    }
    
    return 0;
}
