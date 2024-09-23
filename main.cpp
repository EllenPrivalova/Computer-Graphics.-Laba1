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

	void updateVelocity(float multiplier) {
		velocity *= multiplier;
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

	AdvancedTriangle advancedTriangle(100, Vector2f(400, 300), Vector2f(-1.f, 1.f));

	Color backgroundColor = Color::Black;

	Clock clock;
	float t = 1.f / FRAMERATE;

	float velocityMultiple = 1;
	int currentGear = 2;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Up) {
				if ((currentGear + 1) < 5) {
					advancedTriangle.updateVelocity(2);
					currentGear++;
				}
			}

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::Down) {
				if (currentGear - 1 > 0) {
					advancedTriangle.updateVelocity(0.5); 
					currentGear--;
				}
			}

			if (event.type == Event::Closed)
				window.close();
		}

		advancedTriangle.erase(window, backgroundColor);

		advancedTriangle.updatePosition(window);

		advancedTriangle.draw(window);

		window.display();

		auto elapsed = clock.restart();
		sleep(seconds(t - elapsed.asSeconds()));
	}

	return 0;
}