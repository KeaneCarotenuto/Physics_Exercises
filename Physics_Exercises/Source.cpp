#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>

#include "CVector.h"

int FixedUpdate();
void Draw();

void TestLagrange();

void TestPlanePoint();

float GetDirtyFloat(std::string msg);
#define dfx GetDirtyFloat("float x: ")
#define dfy GetDirtyFloat("float y: ")
#define dfz GetDirtyFloat("float z: ")

Vector3 GetDirtyVec3(std::string msg);

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
	float stepTime = 0;
	bool drawn = false;
	sf::Clock clock;

	//FixedUpdate() call rate
	float step = (1.0f / 60.0f);

	sf::RectangleShape* test = new sf::RectangleShape();
	test->setPosition(sf::Vector2f(10, 10));
	test->setFillColor(sf::Color::Red);
	test->setSize(sf::Vector2f(10, 10));

	game.toDraw.push_back(test);

	//TestLagrange();
	TestPlanePoint();

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
	Vector3 a = GetDirtyVec3("Enter vec A");

	Vector3 b = GetDirtyVec3("Enter vec B");

	Vector3 c = GetDirtyVec3("Enter vec C");

	Vector3 LHS = Vector3::Cross(a, Vector3::Cross(b, c));
	Vector3 RHS = b * Vector3::Dot(a, c) - c * Vector3::Dot(a, b);
	float diff = Vector3::Diff(LHS, RHS);

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
	Vector3 k = GetDirtyVec3("Enter Point on Plane");

	Vector3 n = GetDirtyVec3("Enter Plane Normal");

	Vector3 u = GetDirtyVec3("Enter Point to Test");

	float dot = Vector3::Dot(u - k, n);

	std::cout << "Dot: " << dot;

	std::cout << std::endl;

	std::cout << (dot == 0.0f ? "The Point lies on the Plane." : "They Point does NOT lie on the Plane, " + (std::string)(dot > 0.0f ? "it is infront." : "it is hehind.")) << std::endl;

	system("Pause");
}

float GetDirtyFloat(std::string msg) {
	std::cout << msg;

	float _f;
	std::cin >> _f;

	return _f;
}

Vector3 GetDirtyVec3(std::string msg) {
	std::cout << msg << std::endl;
	float x = dfx;
	float y = dfy;
	float z = dfz;
	return Vector3(x, y, z);
}

//float InputFloat(std::string msg, float min, float max) {
//	float i = -INFINITY;
//
//
//}