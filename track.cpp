//#include <cmath>
//#include <ctime>
//#include <cstdlib>
//#include <vector>
//#include <deque>
//#include <utility>
//#include "SFML/Graphics.hpp"
//#include "SFML/Audio.hpp"
//
//#define FRAMERATE 60;
//
//using namespace sf;
//using namespace std;
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
//    deque<ConvexShape> trail; // Очередь для хранения следа
//    int maxTrailSize; // Максимальный размер следа
//
//public:
//    AdvancedTriangle(float side, Vector2f initialPosition, float initialVelocityX, float k = 1.f, float b = 0.f) {
//        // Задаем форму треугольника
//        triangle.setPointCount(3);
//        triangle.setPoint(0, Vector2f(0, 0));
//        triangle.setPoint(1, Vector2f(side, 0));
//        triangle.setPoint(2, Vector2f(side / 2, -side * sqrt(3) / 2));
//        triangle.setPosition(initialPosition);
//
//        velocity.x = initialVelocityX;
//        velocity.y = k * initialVelocityX + b;
//
//        fillColor = Color(40, 40, 40);
//        outlineColor = Color::Blue;
//
//        triangle.setFillColor(fillColor);
//        triangle.setOutlineColor(outlineColor);
//        triangle.setOutlineThickness(1);
//
//        maxTrailSize = 10; // Количество кадров следа
//    }
//
//    void addToTrail() {
//        // Копируем текущий треугольник и добавляем его в очередь следа
//        ConvexShape trailTriangle = triangle;
//        trail.push_front(trailTriangle);
//    }
//
//    void updatePosition(RenderWindow& window) {
//        // Сохраняем текущее положение в след
//        addToTrail();
//
//        // Обновление позиции
//        Vector2f position = triangle.getPosition();
//        position += velocity;
//
//        // Проверка столкновения с границами экрана
//        if (position.x <= 0 || position.x + triangle.getPoint(2).x >= window.getSize().x) {
//            velocity.x = -velocity.x; // Отражение по оси X
//            changeColor(fillColor);
//        }
//        if (position.y + triangle.getPoint(1).y <= 0 || position.y >= window.getSize().y) {
//            velocity.y = -velocity.y; // Отражение по оси Y
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
//    void drawTrail(RenderWindow& window) {
//        if (trail.size() > maxTrailSize) {
//            auto lastTrail = trail.back();
//            trail.pop_back();
//
//            lastTrail.setFillColor(Color::Black);
//            lastTrail.setOutlineColor(Color::Black);
//            lastTrail.setOutlineThickness(10);
//            window.draw(lastTrail);
//        }
//
//        // Проходим по всем элементам следа и отрисовываем их
//        int alphaStep = 255 / maxTrailSize;
//        for (size_t i = 0; i < trail.size(); ++i) {
//            // Уменьшаем прозрачность для каждого последующего треугольника в следе
//            Color trailColor = trail[i].getFillColor();
//            trailColor.a = max(0, 255 - (int)(i)*alphaStep); // Плавное уменьшение прозрачности
//            trail[i].setFillColor(trailColor);
//            trail[i].setOutlineColor(trailColor);
//
//            window.draw(trail[i]);
//        }
//    }
//
//    void draw(RenderWindow& window) {
//        window.draw(triangle);
//    }
//
//    void erase(RenderWindow& window, Color& backgroundColor) {
//        triangle.setOutlineThickness(10);
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
//    AdvancedTriangle advancedTriangle(100, Vector2f(500, 300), -1.f, 1.f, 2.f);
//
//    Color backgroundColor = Color::Black;
//
//    Clock clock;
//    Clock time;
//    float t = 1.f / FRAMERATE;
//
//    while (window.isOpen()) {
//        Event event;
//        while (window.pollEvent(event)) {
//            if (event.type == Event::Closed)
//                window.close();
//        }
//
//        advancedTriangle.erase(window, backgroundColor);
//
//        advancedTriangle.updatePosition(window);
//
//        // Сначала рисуем след
//        advancedTriangle.drawTrail(window);
//
//        // Затем рисуем сам треугольник
//        advancedTriangle.draw(window);
//
//        window.display();
//
//        auto elapsed = clock.restart();
//        sleep(seconds(t - elapsed.asSeconds()));
//    }
//
//    return 0;
//}