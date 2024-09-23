#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <utility>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#define FRAMERATE 60;

using namespace sf;
using namespace std;

class AdvancedTriangle {
private:
    ConvexShape triangle;
    Vector2f velocity;
    Color fillColor;
    Color outlineColor;

    // Параметры синусоиды
    float amplitude;
    float period;
    float timeElapsed;

public:
    AdvancedTriangle(float side, Vector2f initialPosition, float initialVelocityX, float amplitude, float period)
        : amplitude(amplitude), period(period), timeElapsed(0.0f) {
        // Задаем форму треугольника
        triangle.setPointCount(3);
        triangle.setPoint(0, Vector2f(0, 0));
        triangle.setPoint(1, Vector2f(side, 0));
        triangle.setPoint(2, Vector2f(side / 2, -side * sqrt(3) / 2));

        triangle.setPosition(initialPosition);
        //velocity = initialVelocity;

        fillColor = Color(40, 40, 40);
        outlineColor = Color::Blue;

        triangle.setFillColor(fillColor);
        triangle.setOutlineColor(outlineColor);
        triangle.setOutlineThickness(1);

        velocity.x = initialVelocityX;
        velocity.y = 0.0f;
    }

    void updatePosition(float deltaTime, RenderWindow& window) {
        // Обновляем время
        timeElapsed += deltaTime;

        // Обновляем позицию по оси X
        Vector2f position = triangle.getPosition();
        position.x += velocity.x;

        // Вычисляем новое положение по оси Y на основе синусоиды
        position.y = (window.getSize().y / 2) + amplitude * sin(2 * 3.14159f * timeElapsed / period);

        // Проверка столкновения с границами экрана по оси X
        if (position.x <= 0 || position.x + triangle.getPoint(1).x >= window.getSize().x) {
            velocity.x = -velocity.x; // Отражение по оси X
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
        triangle.setOutlineThickness(10);
        triangle.setFillColor(backgroundColor);
        triangle.setOutlineColor(backgroundColor);

        draw(window);

        triangle.setOutlineThickness(1);
        triangle.setFillColor(fillColor);
        triangle.setOutlineColor(outlineColor);
    }
};

int main() {
    // Создание окна
    RenderWindow window(VideoMode(800, 600), "Laba1");

    // Параметры синусоиды
    float amplitude = 100.0f; // Амплитуда синусоиды
    float period = 2.0f; // Период синусоиды (в секундах)

    // Создаем треугольник, который движется по синусоиде
    AdvancedTriangle advancedTriangle(100, Vector2f(400, 300), 0.7f, amplitude, period);

    Color backgroundColor = Color::Black;

    Clock clock;
    Clock time;
    float t = 1.f / FRAMERATE;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        advancedTriangle.erase(window, backgroundColor);

        advancedTriangle.updatePosition(time.restart().asSeconds(), window);

        advancedTriangle.draw(window);

        window.display();

        auto elapsed = clock.restart();
        sleep(seconds(t - elapsed.asSeconds()));
    }

    return 0;
}