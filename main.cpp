#include <iostream>
#include <utility>
#include <vector>
#include <stack>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

using namespace sf;
using namespace std;

struct Triangle {
	Vector2f left;
	Vector2f right;
	Vector2f top;
};

void changeColor(Color& color) {
	color.r = rand() % 256;
	color.g = rand() % 256;
	color.b = rand() % 256;
}

/*
	Алгоритм Брезенхе́ма — это алгоритм, определяющий, какие точки n-мерного растра нужно закрасить, 
	чтобы получить близкое приближение прямой линии между двумя заданными точками.
*/
void drawLine(int x1, int y1, int x2, int y2, Image& image, Color color) {
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;

	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		image.setPixel(x, y, color);

		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
			{
				px = px + 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					y = y + 1;
				}
				else
				{
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}
			image.setPixel(x, y, color);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		image.setPixel(x, y, color);

		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					x = x + 1;
				}
				else
				{
					x = x - 1;
				}
				py = py + 2 * (dx1 - dy1);
			}
			image.setPixel(x, y, color);
		}
	}
}

void fillTriangle(Vector2f center, Image& image, Color fillColor, Color lineColor) {
	stack<Vector2f> pixels;

	pixels.push(center);
	while (!pixels.empty()) {
		auto pixel = pixels.top();
		pixels.pop();

		if (pixel.x >= 0 && pixel.x < image.getSize().x && pixel.y >= 0 && pixel.y < image.getSize().y)
		{
			Color color = image.getPixel(pixel.x, pixel.y);

			if (color != lineColor && color != fillColor) {
				image.setPixel(pixel.x, pixel.y, fillColor);

				pixels.push(Vector2f(pixel.x, pixel.y + 1));
				pixels.push(Vector2f(pixel.x + 1, pixel.y));
				pixels.push(Vector2f(pixel.x, pixel.y - 1));
				pixels.push(Vector2f(pixel.x - 1, pixel.y));
			}
		}
	}
}

Triangle drawTriangle(Vector2f center, float radius, Image& image, Color fillColor, Color lineColor) {
	Vector2f leftPoint = center + Vector2f(-sqrt(3) / 2 * radius, 0.5 * radius);
	Vector2f rightPoint = center + Vector2f(sqrt(3) / 2 * radius, 0.5 * radius);
	Vector2f topPoint = center + Vector2f(0, -radius);

	drawLine(leftPoint.x, leftPoint.y, rightPoint.x, rightPoint.y, image, lineColor);
	drawLine(topPoint.x, topPoint.y, rightPoint.x, rightPoint.y, image, lineColor);
	drawLine(leftPoint.x, leftPoint.y, topPoint.x, topPoint.y, image, lineColor);

	fillTriangle(center, image, fillColor, lineColor);

	return {
		leftPoint,
		rightPoint,
		topPoint
	};
}

int main() {
	// Создание окна
	RenderWindow window(VideoMode(800, 600), "Laba1");

	// Цвета
	Color fillColor = Color(40, 40, 40);
	Color lineColor = Color::Blue;
	Color backgroundColor = Color::Black;

	Image image;
	image.create(800, 600, backgroundColor);

	float radius = 20.f;
	Vector2f center = Vector2f(window.getSize().x / 2, window.getSize().y / 2);

	// Пара: начальное направление треугольника и его цвет
	Vector2f direction = Vector2f(-1.f, 1.f);

	Triangle triangle;

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		

		triangle = drawTriangle(center, radius, image, backgroundColor, backgroundColor);

		// Проверка на выход за правый край
		if (triangle.right.x + 1 >= image.getSize().x) {
			direction.x *= -1;
			changeColor(fillColor);
		}

		//Проверка на выход за левый край
		if (triangle.left.x - 1 < 0) {
			direction.x *= -1;
			changeColor(fillColor);
		}

		// Проверка на выход на нижний край
		if (triangle.left.y + 1 >= image.getSize().y) {
			direction.y *= -1;
			changeColor(fillColor);
		}

		// Проверка на выход за верхний край
		if (triangle.top.y - 1 < 0) {
			direction.y *= -1;
			changeColor(fillColor);
		}
		
		// Перемещение при столкновении
		center = center + direction;

		// На новой точке отрисовываем кадр
		drawTriangle(center, radius, image, fillColor, lineColor);
		
		Texture texture;
		texture.loadFromImage(image);

		Sprite sprite(texture);

		window.draw(sprite);

		// Отрендерить все вызовы draw
		window.display();
	}

	return 0;
}
