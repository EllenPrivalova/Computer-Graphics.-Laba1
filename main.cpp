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
    FloatRect orthogonalRect;
    Vector2f velocity;
    Color fillColor;
    Color outlineColor;
    float mass;

public:
    AdvancedTriangle(float side, Vector2f initialPosition, Vector2f initialVelocity, float mass) {
        // Задаем форму треугольника
        triangle.setPointCount(3);
        triangle.setPoint(0, Vector2f(0, 0));
        triangle.setPoint(1, Vector2f(side, 0));
        triangle.setPoint(2, Vector2f(side / 2, -side * sqrt(3) / 2));

        triangle.setPosition(initialPosition);
        velocity = initialVelocity;

        this->mass = mass;

        fillColor = Color(40, 40, 40);
        outlineColor = Color::Blue;

        triangle.setFillColor(fillColor);
        triangle.setOutlineColor(outlineColor);
        triangle.setOutlineThickness(1);

        updateOrthogonalRect();
    }

    void updateOrthogonalRect() {
        // Обновление ограничивающего прямоугольника AABB
        sf::Vector2f position = triangle.getPosition();
        float size = triangle.getPoint(1).x; // Длина стороны треугольника
        orthogonalRect = FloatRect(position.x, position.y, size, size * sqrt(3) / 2);
    }

    bool checkCollision(AdvancedTriangle& other) {
        return orthogonalRect.intersects(other.orthogonalRect);
    }

    void resolveCollision(AdvancedTriangle& other) {
        // Закон сохранения импульса
        Vector2f newVelocity1 = velocity;
        Vector2f newVelocity2 = other.velocity;

        float totalMass = mass + other.mass;
        newVelocity1 = (velocity * (mass - other.mass) + other.velocity * (2 * other.mass)) / totalMass;
        newVelocity2 = (other.velocity * (other.mass - mass) + velocity * (2 * mass)) / totalMass;

        // Обновляем скорости после столкновения
        velocity = newVelocity1;
        other.velocity = newVelocity2;
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
        updateOrthogonalRect();
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

int main() {
    // Создание окна
    RenderWindow window(VideoMode(800, 600), "Laba1");

    AdvancedTriangle advancedTriangle1(100, Vector2f(400, 300), Vector2f(1.0f, 1.5f), 1.0f);
    AdvancedTriangle advancedTriangle2(100, Vector2f(500, 400), Vector2f(-1.5f, -1.f), 1.0f);

    Color backgroundColor = Color::Black;

    Clock clock;
    float t = 1.f / FRAMERATE;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        advancedTriangle1.erase(window, backgroundColor);
        advancedTriangle2.erase(window, backgroundColor);

        advancedTriangle1.updatePosition(window);
        advancedTriangle2.updatePosition(window);

        if (advancedTriangle1.checkCollision(advancedTriangle2)) {
            advancedTriangle1.resolveCollision(advancedTriangle2);
        }

        advancedTriangle1.draw(window);
        advancedTriangle2.draw(window);

        window.display();

        auto elapsed = clock.restart();
        sleep(seconds(t - elapsed.asSeconds()));
    }

    return 0;
}