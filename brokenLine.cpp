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
//class AdvancedTriangle {
//private:
//	ConvexShape triangle;
//	Vector2f velocity;
//	float k;
//	float b;
//
//	Color fillColor;
//	Color outlineColor;
//
//	vector<Vector2f> trajectoryPoints; // ������� ����������
//	size_t currentPointIndex; // ������ ������� ����� �� ����������
//
//public:
//	AdvancedTriangle(float side, Vector2f initialPosition, float initialVelocityX, vector<Vector2f>& points, float k = 1.f, float b = 0.f) {
//		// ������ ����� ������������
//		triangle.setPointCount(3);
//		triangle.setPoint(0, Vector2f(0, 0));
//		triangle.setPoint(1, Vector2f(side, 0));
//		triangle.setPoint(2, Vector2f(side / 2, -side * sqrt(3) / 2));
//		triangle.setPosition(initialPosition);
//
//		velocity.x = initialVelocityX;
//		velocity.y = k * initialVelocityX + b;
//
//		fillColor = Color(40, 40, 40);
//		outlineColor = Color::Blue;
//
//		triangle.setFillColor(fillColor);
//		triangle.setOutlineColor(outlineColor);
//		triangle.setOutlineThickness(1);
//
//		trajectoryPoints = points;
//		currentPointIndex = 0;
//
//		// ��������� �������� ���������� � ������ ����� ����������
//		updateVelocityTowardsNextPoint();
//	}
//
//	void updateVelocityTowardsNextPoint() {
//		if (currentPointIndex < trajectoryPoints.size()) {
//			Vector2f targetPoint = trajectoryPoints[currentPointIndex];
//			Vector2f direction = targetPoint - triangle.getPosition();
//
//			// ������������ ������� ����������� ��� ��������� ���������� �������
//			float length = sqrt(direction.x * direction.x + direction.y * direction.y);
//			if (length != 0) {
//				velocity = direction / length;
//			}
//		}
//	}
//
//	void updatePosition() {
//		// ���������� �������
//		Vector2f position = triangle.getPosition();
//		position += velocity;
//
//		// �������� ���������� ������� ����� ����������
//		Vector2f targetPoint = trajectoryPoints[currentPointIndex];
//		if (abs(position.x - targetPoint.x) < 5.0f && abs(position.y - targetPoint.y) < 5.0f) {
//			// ������������ � ��������� �����
//			currentPointIndex = (currentPointIndex + 1) % trajectoryPoints.size();
//			updateVelocityTowardsNextPoint();
//		}
//
//		triangle.setPosition(position);
//	}
//
//	void changeColor(Color& color) {
//		fillColor = Color(rand() % 256, rand() % 256, rand() % 256);
//		triangle.setFillColor(fillColor);
//	}
//
//	void draw(RenderWindow& window) {
//		window.draw(triangle);
//	}
//
//	void erase(RenderWindow& window, Color& backgroundColor) {
//		triangle.setOutlineThickness(5);
//		triangle.setFillColor(backgroundColor);
//		triangle.setOutlineColor(backgroundColor);
//
//		draw(window);
//
//		triangle.setOutlineThickness(1);
//		triangle.setFillColor(fillColor);
//		triangle.setOutlineColor(outlineColor);
//	}
//};
//
//int main() {
//	// �������� ����
//	RenderWindow window(VideoMode(1000, 600), "Laba1");
//
//	// ������� ����������
//	vector<Vector2f> trajectoryPoints = {
//		Vector2f(100, 100),
//		Vector2f(700, 100),
//		Vector2f(400, 500),
//		Vector2f(100, 500)
//	};
//
//	// ������� �����������
//	AdvancedTriangle advancedTriangle(100, Vector2f(500, 300), -1.f, trajectoryPoints, 1.f, 2.f);
//
//	Color backgroundColor = Color::Black;
//
//	Clock clock;
//	float t = 1.f / FRAMERATE;
//
//	while (window.isOpen()) {
//		Event event;
//		while (window.pollEvent(event)) {
//			if (event.type == Event::Closed)
//				window.close();
//		}
//
//		advancedTriangle.erase(window, backgroundColor);
//
//		advancedTriangle.updatePosition();
//
//		advancedTriangle.draw(window);
//
//		window.display();
//
//		auto elapsed = clock.restart();
//		sleep(seconds(t - elapsed.asSeconds()));
//	}
//
//	return 0;
//}