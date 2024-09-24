//#include <cmath>
//#include <ctime>
//#include <cstdlib>
//#include <vector>
//#include <utility>
//#include "SFML/Graphics.hpp"
//#include "SFML/Audio.hpp"
//
//#define FRAMERATE 60;
//
//using namespace sf;
//using namespace std;
//
//// Утилита для проекции точки на ось
//float dotProduct(const Vector2f& a, const Vector2f& b) {
//    return a.x * b.x + a.y * b.y;
//}
//
//// Получаем нормализованную ось для SAT (нормаль к стороне)
//Vector2f getNormal(Vector2f& edge) {
//    return Vector2f(-edge.y, edge.x);
//}
//
//
//class AdvancedTriangle {
//private:
//    ConvexShape triangle;
//    Vector2f velocity;
//    float k;
//    float b;
//
//    Color fillColor;
//    Color outlineColor;
//
//    float mass;
//
//public:
//    AdvancedTriangle(float side, Vector2f initialPosition, float initialVelocityX, float mass, float k = 1.f, float b = 0.f) {
//        // Задаем форму треугольника
//        triangle.setPointCount(3);
//        triangle.setPoint(0, Vector2f(0, 0));
//        triangle.setPoint(1, Vector2f(side, 0));
//        triangle.setPoint(2, Vector2f(side / 2, -side * sqrt(3) / 2));
//        triangle.setPosition(initialPosition);
//
//        velocity.x = initialVelocityX;
//        velocity.y = k * velocity.x + b;
//
//        this->mass = mass;
//
//        fillColor = Color(40, 40, 40);
//        outlineColor = Color::Blue;
//
//        triangle.setFillColor(fillColor);
//        triangle.setOutlineColor(outlineColor);
//        triangle.setOutlineThickness(1);
//    }
//
//    void projectOntoAxis(const Vector2f& axis, float& min, float& max) {
//        min = max = dotProduct(triangle.getTransform().transformPoint(triangle.getPoint(0)), axis);
//        for (int i = 1; i < 3; ++i) {
//            float projection = dotProduct(triangle.getTransform().transformPoint(triangle.getPoint(i)), axis);
//            if (projection < min) {
//                min = projection;
//            }
//            if (projection > max) {
//                max = projection;
//            }
//        }
//    }
//
//    bool checkCollision(AdvancedTriangle& other) {
//        vector<Vector2f> axesToTest;
//
//        // Получаем стороны треугольников для тестирования осей
//        for (int i = 0; i < 3; ++i) {
//            Vector2f edge1 = triangle.getPoint((i + 1) % 3) - triangle.getPoint(i);
//            Vector2f edge2 = other.triangle.getPoint((i + 1) % 3) - other.triangle.getPoint(i);
//            axesToTest.push_back(getNormal(edge1));
//            axesToTest.push_back(getNormal(edge2));
//        }
//
//        for (auto& axis : axesToTest) {
//            float minA, maxA, minB, maxB;
//            projectOntoAxis(axis, minA, maxA);
//            other.projectOntoAxis(axis, minB, maxB);
//
//            // Если на какой-то оси проекции не пересекаются, столкновения нет
//            if (maxA < minB || maxB < minA) {
//                return false;
//            }
//        }
//        return true; // Столкновение произошло
//    }
//
//    void resolveCollision(AdvancedTriangle& other) {
//        Vector2f newVelocity1 = velocity;
//        Vector2f newVelocity2 = other.velocity;
//
//        float totalMass = mass + other.mass;
//        newVelocity1 = (velocity * (mass - other.mass) + other.velocity * (2 * other.mass)) / totalMass;
//        newVelocity2 = (other.velocity * (other.mass - mass) + velocity * (2 * mass)) / totalMass;
//
//        // Обновляем скорости после столкновения
//        velocity = newVelocity1;
//        other.velocity = newVelocity2;
//    }
//
//    void updatePosition(RenderWindow& window) {
//        // Обновление позиции
//        Vector2f position = triangle.getPosition();
//        position += velocity;
//
//        // Проверка столкновения с границами экрана
//        if (position.x <= 0 || position.x + triangle.getPoint(1).x >= window.getSize().x) {
//            velocity.x = -velocity.x;
//            changeColor(fillColor);
//        }
//
//        if (position.y + triangle.getPoint(2).y <= 0 || position.y >= window.getSize().y) {
//            velocity.y = -velocity.y;
//            changeColor(fillColor);
//        }
//
//        triangle.setPosition(position);
//    }
//
//    void changeColor(Color& color) {
//        fillColor = Color(rand() % 256, rand() % 256, rand() % 256);
//        triangle.setFillColor(fillColor);
//    }
//
//    void draw(RenderWindow& window) {
//        window.draw(triangle);
//    }
//
//    void erase(RenderWindow& window, Color& backgroundColor) {
//        triangle.setOutlineThickness(5);
//        triangle.setFillColor(backgroundColor);
//        triangle.setOutlineColor(backgroundColor);
//
//        draw(window);
//
//        triangle.setOutlineThickness(1);
//        triangle.setFillColor(fillColor);
//        triangle.setOutlineColor(outlineColor);
//    }
//};
//
//int main() {
//    // Создание окна
//    RenderWindow window(VideoMode(1000, 600), "Laba1");
//
//    AdvancedTriangle advancedTriangle1(100, Vector2f(400, 300), 1.f, 1.f, 1.f, 0.5f);
//    AdvancedTriangle advancedTriangle2(100, Vector2f(500, 400), -1.5f, 1.f, 1.f, 0.5f);
//
//    Color backgroundColor = Color::Black;
//
//    Clock clock;
//    float t = 1.f / FRAMERATE;
//
//    while (window.isOpen()) {
//        Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == Event::Closed)
//                window.close();
//        }
//
//        advancedTriangle1.erase(window, backgroundColor);
//        advancedTriangle2.erase(window, backgroundColor);
//
//        advancedTriangle1.updatePosition(window);
//        advancedTriangle2.updatePosition(window);
//
//        if (advancedTriangle1.checkCollision(advancedTriangle2)) {
//            advancedTriangle1.resolveCollision(advancedTriangle2);
//        }
//
//        advancedTriangle1.draw(window);
//        advancedTriangle2.draw(window);
//
//        window.display();
//
//        auto elapsed = clock.restart();
//        sleep(seconds(t - elapsed.asSeconds()));
//    }
//
//    return 0;
//}