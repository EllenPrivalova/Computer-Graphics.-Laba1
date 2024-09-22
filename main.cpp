#include <iostream>
#include <utility>
#include <vector>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

using namespace sf;
using namespace std;

int main() {
	// Создание окна
	RenderWindow window(VideoMode(800, 600), "Laba1");

	// Цвета
	Color background = Color::Black;
	Color triangleBaseColor = Color(40, 40, 40);

	CircleShape* triangle = new CircleShape(20.f, 3);

	// Переместить треугольник в середину окна
	triangle->setPosition(Vector2f(window.getSize().x / 2, window.getSize().y / 2));
	triangle->setOutlineThickness(1);

	// Пара: начальное направление треугольника и его цвет
	pair<Vector2f, Color> triangleInfo;
	triangleInfo = make_pair(Vector2f(-1.f, 1.f), Color::Blue);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}

		// На текущей точке закрашиваем кадр в цвет фона
		triangle->setFillColor(background);
		triangle->setOutlineColor(background);
		triangle->setOutlineThickness(5);
		window.draw(*triangle);

		Vector2f direction = triangleInfo.first;
		Color col = triangleInfo.second;

		// Проверка на выход за правый край
		if (triangle->getGlobalBounds().left + triangle->getGlobalBounds().width > window.getSize().x) {
			direction.x *= -1;
		}

		//Проверка на выход за левый край
		if (triangle->getGlobalBounds().left < 0) {
			direction.x *= -1;
		}

		// Проверка на выход на нижний край
		if (triangle->getGlobalBounds().top + triangle->getGlobalBounds().height > window.getSize().y) {
			direction.y *= -1;
		}

		// Проверка на выход за верхний край
		if (triangle->getGlobalBounds().top < 0) {
			direction.y *= -1;
		}
		
		// Перемещение при столкновении 
		triangle->setPosition(triangle->getPosition() + direction);
		triangleInfo.first = direction;

		// На новой точке отрисовываем кадр
		triangle->setFillColor(triangleBaseColor);
		triangle->setOutlineColor(col);
		triangle->setOutlineThickness(2);
		window.draw(*triangle);

		// Отрендерить все вызовы draw
		window.display();
	}

	delete triangle;

	return 0;
}
