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
	
	vector<Vector2f> trajectoryPoints; // Ломаная траектория
	size_t currentPointIndex; // Индекс текущей точки на траектории

public:
	AdvancedTriangle(float side, Vector2f initialPosition, Vector2f initialVelocity, vector<Vector2f>& points) {
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

		trajectoryPoints = points;
		currentPointIndex = 0;

		// Начальная скорость направлена к первой точке траектории
		updateVelocityTowardsNextPoint();
	}

	void updateVelocityTowardsNextPoint() {
		if (currentPointIndex < trajectoryPoints.size()) {
			Vector2f targetPoint = trajectoryPoints[currentPointIndex];
			Vector2f direction = targetPoint - triangle.getPosition();

			// Нормализация вектора направления для получения единичного вектора
			float length = sqrt(direction.x * direction.x + direction.y * direction.y);
			if (length != 0) {
				velocity = direction / length;
			}
		}
	}

	void updatePosition() {
		// Обновление позиции
		Vector2f position = triangle.getPosition();
		position += velocity;

		// Проверка достижения текущей точки траектории
		Vector2f targetPoint = trajectoryPoints[currentPointIndex];
		if (abs(position.x - targetPoint.x) < 5.0f && abs(position.y - targetPoint.y) < 5.0f) {
			// Перемещаемся к следующей точке
			currentPointIndex = (currentPointIndex + 1) % trajectoryPoints.size();
			updateVelocityTowardsNextPoint();
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

int main() {
	// Создание окна
	RenderWindow window(VideoMode(800, 600), "Laba1");

	// Ломаная траектория
	vector<Vector2f> trajectoryPoints = {
		Vector2f(100, 100),
		Vector2f(700, 100),
		Vector2f(400, 500),
		Vector2f(100, 500)
	};

	// Создаем треугольник
	AdvancedTriangle advancedTriangle(100, Vector2f(400, 300), Vector2f(-1.f, 1.f), trajectoryPoints);
	
	Color backgroundColor = Color::Black;

	Clock clock;
	float t = 1.f / FRAMERATE;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}

		advancedTriangle.erase(window, backgroundColor);

		advancedTriangle.updatePosition();

		advancedTriangle.draw(window);

		window.display();

		auto elapsed = clock.restart();
		sleep(seconds(t - elapsed.asSeconds()));
	}

	return 0;
}