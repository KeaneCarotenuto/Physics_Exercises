#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iomanip>

#include "CVector.h"

int FixedUpdate();
void Draw();

void TestLagrange();
void TestPlanePoint();
void TestPlaneLine();

double InputDouble(std::string msg);
Vector3 InputVec3(std::string msg);

class CGame {
public:
	sf::RenderWindow* wind;
	std::vector<sf::Drawable*> toDraw;
}game;

int main() {

	//Creating Different Windows
	sf::RenderWindow window(sf::VideoMode(800, 600), "ProgramName - By Keane Carotenuto");
	game.wind = &window;

	//Manages the FixedUpdate() timing
	double stepTime = 0;
	bool drawn = false;
	sf::Clock clock;

	//FixedUpdate() call rate
	double step = (1.0f / 60.0f);

	sf::RectangleShape* test = new sf::RectangleShape();
	test->setPosition(sf::Vector2f(10, 10));
	test->setFillColor(sf::Color::Red);
	test->setSize(sf::Vector2f(10, 10));

	game.toDraw.push_back(test);

	while (window.isOpen() == true)
	{
		stepTime += clock.getElapsedTime().asSeconds();
		clock.restart();

		while (stepTime >= step)
		{
			//Main Loop of Game
			if (FixedUpdate() == 0) return 0;

			stepTime -= step;
			drawn = false;
		}

		//Draws After Updates
		if (drawn)
		{
			//sf::sleep(sf::seconds(0.01f));
		}
		else
		{
			Draw();

			drawn = true;
		}

		sf::Event newEvent;

		//Quit
		while (window.pollEvent(newEvent))
		{
			if (newEvent.type == sf::Event::Closed)
			{
				window.close();
			}

			if (newEvent.type == sf::Event::KeyPressed)
			{
				if (newEvent.key.code == sf::Keyboard::Num1) {
					TestLagrange();
				}

				if (newEvent.key.code == sf::Keyboard::Num2) {
					TestPlanePoint();
				}
				
				if (newEvent.key.code == sf::Keyboard::Num3) {
					TestPlaneLine();
				}
			}

			
		}
	}

	return 0;
}

int FixedUpdate() {
	sf::RectangleShape* theRect = dynamic_cast<sf::RectangleShape*>(game.toDraw[0]);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		theRect->setPosition(sf::Vector2f(theRect->getPosition().x - 1, theRect->getPosition().y));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		theRect->setPosition(sf::Vector2f(theRect->getPosition().x + 1, theRect->getPosition().y));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		theRect->setPosition(sf::Vector2f(theRect->getPosition().x, theRect->getPosition().y - 1));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		theRect->setPosition(sf::Vector2f(theRect->getPosition().x, theRect->getPosition().y + 1));
	}

	return 1;
}

void Draw() {
	game.wind->clear();

	for (sf::Drawable* item : game.toDraw)
	{
		game.wind->draw((*item));
	}

	//Update main window
	game.wind->display();
}

void TestLagrange() {
	system("CLS");
	std::cout << "-Lagrange's Formula-" << std::endl << std::endl;

	Vector3 a = InputVec3("Enter vec A");
	std::cout << std::endl;

	Vector3 b = InputVec3("Enter vec B");
	std::cout << std::endl;

	Vector3 c = InputVec3("Enter vec C");
	std::cout << std::endl;

	Vector3 LHS = Vector3::Cross(a, Vector3::Cross(b, c));
	Vector3 RHS = b * Vector3::Dot(a, c) - c * Vector3::Dot(a, b);
	double diff = Vector3::Diff(LHS, RHS);

	std::cout << "A: " << a.ToString() << std::endl;
	std::cout << "B: " << b.ToString() << std::endl;
	std::cout << "C: " << c.ToString() << std::endl;
	std::cout << std::endl;

	std::cout << "LHS: " << LHS.ToString() << std::endl;
	std::cout << "RHS: " << RHS.ToString() << std::endl;
	std::cout << "Diff: " << diff << std::endl;
	std::cout << (diff <= 0.001 ? "They are Equal." : "They are NOT Equal.") << std::endl;

	system("Pause");
}

void TestPlanePoint() {
	system("CLS");
	std::cout << "-Plane vs Point Function-" << std::endl << std::endl;

	Vector3 k = InputVec3("Enter Point on Plane (k)");
	std::cout << std::endl;

	Vector3 n = InputVec3("Enter Plane Normal (n)");
	std::cout << std::endl;

	Vector3 p = InputVec3("Enter Point to Test (p)");
	std::cout << std::endl;

	double dot = Vector3::Dot(p - k, n);

	std::cout << "Dot: " << dot;

	std::cout << std::endl;

	std::cout << (Vector3::IsPointOnPlane(p, k, n) ? "The Point lies on the Plane." : "They Point does NOT lie on the Plane, " + (std::string)(dot > 0.0f ? "it is infront." : "it is hehind.")) << std::endl;

	system("Pause");
}

void TestPlaneLine() {
	system("CLS");
	std::cout << "-Line Intersect Plane-" << std::endl << std::endl;

	Vector3 pp = InputVec3("Enter Point on Plane (k)");
	std::cout << std::endl;

	Vector3 pn = InputVec3("Enter Plane Normal (n)");
	std::cout << std::endl;

	Vector3 lpa = InputVec3("Enter line point (a)");
	std::cout << std::endl;

	Vector3 lpb = InputVec3("Enter line point (b)");
	std::cout << std::endl;

	bool doesInt = false;
	Vector3 intersectPoint = Vector3::LineIntersectPlanePoint(pp, pn, lpa, lpb, &doesInt);

	std::cout << (doesInt ? "The Line intersects the Plane at point I: " + intersectPoint.ToString() : "They Line does NOT intersect the Plane") << std::endl;

	system("Pause");
}

double InputDouble(std::string msg) {

	std::cout << msg;
	double _d;

	bool notProper = true;
	while (notProper) {
		std::string input;
		std::cin >> input;
			if (input.find_first_not_of("1234567890.-") != std::string::npos) {
				std::cout << "invalid number: " << input << std::endl;
			}
			else {
				_d = stod(input);
				notProper = false;
			}
	};

	return _d;
}

Vector3 InputVec3(std::string msg) {
	std::cout << msg << std::endl;
	double x = InputDouble("double x: ");
	double y = InputDouble("double y: ");
	double z = InputDouble("double z: ");
	return Vector3(x, y, z);
}

//double Inputdouble(std::string msg, double min, double max) {
//	double i = -INFINITY;
//
//
//}