#include <SFML/Graphics.hpp>

// Функция для рисования одного пикселя
void setPixel(sf::Image& image, int x, int y, const sf::Color& color) {
    // Проверяем, чтобы координаты были в пределах изображения
    if (x >= 0 && x < (int)image.getSize().x && y >= 0 && y < (int)image.getSize().y) {
        image.setPixel(x, y, color);
    }
}

int main() {
    // 1. ИНИЦИАЛИЗАЦИЯ ОКНА
    const int WIDTH = 800;   // ширина окна
    const int HEIGHT = 600;  // высота окна
    
    // Создаем окно с заданным размером и названием
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML Pixel Drawing");
    
    // 2. СОЗДАНИЕ ИЗОБРАЖЕНИЯ ДЛЯ РИСОВАНИЯ ПИКСЕЛЕЙ
    sf::Image image;                     // Объект изображения
    image.create(WIDTH, HEIGHT, sf::Color::Black);  // Создаем черное изображение
    
    sf::Texture texture;                 // Текстура для отображения
    texture.loadFromImage(image);        // Загружаем изображение в текстуру
    
    sf::Sprite sprite(texture);          // Спрайт для рисования текстуры в окне
    
    // 3. ОСНОВНОЙ ЦИКЛ ПРОГРАММЫ
    while (window.isOpen()) {
        // Обработка событий (закрытие окна и т.д.)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();  // Закрыть окно при нажатии на крестик
            }
        }
        
        // 4. ПРИМЕРЫ ИСПОЛЬЗОВАНИЯ setPixel()
        
        // Очищаем экран (закрашиваем черным)
        image.create(WIDTH, HEIGHT, sf::Color::Black);
        
        // Рисуем красный пиксель в центре
        setPixel(image, WIDTH / 2, HEIGHT / 2, sf::Color::Red);
        
        // Рисуем зеленую диагональ
        for (int i = 0; i < 200; i++) {
            setPixel(image, 100 + i, 100 + i, sf::Color::Green);
        }
        
        // Рисуем синий квадрат 10x10
        for (int x = 300; x < 310; x++) {
            for (int y = 200; y < 210; y++) {
                setPixel(image, x, y, sf::Color::Blue);
            }
        }
        
        // Рисуем случайные пиксели
        for (int i = 0; i < 100; i++) {
            int x = rand() % WIDTH;
            int y = rand() % HEIGHT;
            setPixel(image, x, y, sf::Color(
                rand() % 256,  // красный
                rand() % 256,  // зеленый
                rand() % 256   // синий
            ));
        }
        
        // 5. ОБНОВЛЕНИЕ И ОТРИСОВКА
        
        // Обновляем текстуру с новыми пикселями
        texture.update(image);
        
        // Очищаем окно
        window.clear();
        
        // Рисуем спрайт с нашей текстурой
        window.draw(sprite);
        
        // Отображаем все на экране
        window.display();
        
        // Небольшая задержка, чтобы видеть результат
        sf::sleep(sf::milliseconds(100));
    }
    
    return 0;
}
