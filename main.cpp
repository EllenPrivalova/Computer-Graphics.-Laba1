#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

#define FRAMERATE 60;

class AdvancedTriangle {
private:
    ConvexShape triangle;
    Vector2f velocity;
    Color fillColor;
    Color outlineColor;
    float speed;
public:
    AdvancedTriangle(float side, Vector2f initialPosition, Vector2f initialVelocity) {
        // Задаем форму треугольника
        triangle.setPointCount(3);
        triangle.setPoint(0, Vector2f(0, 0));
        triangle.setPoint(1, Vector2f(side, 0));
        triangle.setPoint(2, Vector2f(side / 2, -side * sqrt(3) / 2));
        triangle.setPosition(initialPosition);
        velocity = initialVelocity;
        fillColor = Color(40, 40, 40);
        outlineColor = Color::Blue;
        triangle.setFillColor(fillColor);
        triangle.setOutlineColor(outlineColor);
        triangle.setOutlineThickness(1);
    }
    void updatePosition(RenderWindow& window) {
        // Обновление позиции
        Vector2f position = triangle.getPosition();
        position += velocity;
        // Проверка столкновения с границами экрана
        if (position.x <= 0 || position.x + triangle.getPoint(1).x >= window.getSize().x) {
            velocity.x = -velocity.x;
            changeColor(fillColor);
        }
        if (position.y + triangle.getPoint(2).y <= 0 || position.y >= window.getSize().y) {
            velocity.y = -velocity.y;
            changeColor(fillColor);
        }
        triangle.setPosition(position);
    }
    void changeColor(Color& color) {
        fillColor = Color(rand() % 256, rand() % 256, rand() % 256);
        triangle.setFillColor(fillColor);
    }
    void draw(RenderWindow& window) {
        window.draw(triangle);
    }
    void erase(RenderWindow& window, Color& backgroundColor) {
        triangle.setOutlineThickness(3);
        triangle.setFillColor(backgroundColor);
        triangle.setOutlineColor(backgroundColor);
        draw(window);
        triangle.setOutlineThickness(1);
        triangle.setFillColor(fillColor);
        triangle.setOutlineColor(outlineColor);
    }
};

class Menu {
private:
    sf::RectangleShape background;
    sf::RectangleShape continueButton;
    sf::RectangleShape restartButton;
    sf::RectangleShape exitButton;

    sf::Text continueText;
    sf::Text restartText;
    sf::Text exitText;

    sf::Font font;

    bool isVisible;

public:
    Menu(float width, float height) {
        isVisible = false;

        // Загрузка шрифта
        if (!font.loadFromFile("Arial.ttf")) {
            std::cerr << "Ошибка загрузки шрифта через SFML\n";
        }

        // Настройка фона меню
        background.setSize(sf::Vector2f(width / 2, height / 2));
        background.setFillColor(sf::Color(50, 50, 50, 200));
        background.setPosition(width / 4, height / 4);

        // Настройка кнопок
        float buttonWidth = background.getSize().x - 40;
        float buttonHeight = 50;
        float x = background.getPosition().x + 20;
        float y = background.getPosition().y + 30;

        continueButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        continueButton.setFillColor(sf::Color(100, 100, 100));
        continueButton.setPosition(x, y);

        restartButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        restartButton.setFillColor(sf::Color(100, 100, 100));
        restartButton.setPosition(x, y + 70);

        exitButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        exitButton.setFillColor(sf::Color(100, 100, 100));
        exitButton.setPosition(x, y + 140);

        // Настройка текста кнопок
        continueText.setFont(font);
        continueText.setString("Continue");
        continueText.setCharacterSize(24);
        continueText.setFillColor(sf::Color::White);
        centerText(continueText, continueButton);

        restartText.setFont(font);
        restartText.setString("Restart");
        restartText.setCharacterSize(24);
        restartText.setFillColor(sf::Color::White);
        centerText(restartText, restartButton);

        exitText.setFont(font);
        exitText.setString("Exit");
        exitText.setCharacterSize(24);
        exitText.setFillColor(sf::Color::White);
        centerText(exitText, exitButton);
    }

    void show() {
        isVisible = true;
    }

    void hide() {
        isVisible = false;
    }

    bool visible() const {
        return isVisible;
    }

    void draw(sf::RenderWindow& window) {
        if (!isVisible)
            return;

        window.draw(background);
        window.draw(continueButton);
        window.draw(restartButton);
        window.draw(exitButton);

        window.draw(continueText);
        window.draw(restartText);
        window.draw(exitText);
    }

    int handleEvent(sf::Event& event, sf::RenderWindow& window) {
        if (!isVisible)
            return 0; // Нет действия

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (continueButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                // Продолжить
                return 1;
            }
            else if (restartButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                // Перезапустить
                return 2;
            }
            else if (exitButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                // Выход
                return 3;
            }
        }

        return 0; // Нет действия
    }

private:
    void centerText(sf::Text& text, sf::RectangleShape& button) {
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        text.setPosition(button.getPosition().x + button.getSize().x / 2.0f,
            button.getPosition().y + button.getSize().y / 2.0f);
    }
};

int main() {
    RenderWindow window(VideoMode(1000, 600), "Laba1", Style::Fullscreen);

    AdvancedTriangle advancedTriangle(100, Vector2f(400, 300), Vector2f(-1.f, 1.f));
    Color backgroundColor = Color::Black;
    Clock clock;
    float t = 1.f / FRAMERATE;

    // Создаем меню
    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            // Обработка закрытия окна
            if (event.type == sf::Event::Closed)
                window.close();

            // Обработка нажатия клавиш
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (menu.visible()) {
                        menu.hide();
                    }
                    else {
                        menu.show();
                    }
                }
            }

            // Обработка событий меню
            if (menu.visible()) {
                int action = menu.handleEvent(event, window);
                if (action == 1) {
                    // Продолжить
                    menu.hide();
                }
                else if (action == 2) {
                    // Перезапустить
                    advancedTriangle = AdvancedTriangle(100, sf::Vector2f(400, 300), sf::Vector2f(0.7f, 0.5f));
                    menu.hide();
                }
                else if (action == 3) {
                    // Выход
                    window.close();
                }
            }
        }

        // Обновляем позицию треугольника, если меню не отображается
        if (!menu.visible()) {
            advancedTriangle.updatePosition(window);
        }

        // Отрисовываем кадр
        window.clear(backgroundColor);

        advancedTriangle.draw(window);

        // Отображаем меню, если оно активно
        if (menu.visible()) {
            menu.draw(window);
        }

        window.display();
        auto elapsed = clock.restart();
        sleep(seconds(t - elapsed.asSeconds()));
    }

    return 0;
}
