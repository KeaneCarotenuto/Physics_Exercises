#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>

#include "CVector.h"

int FixedUpdate();
void Draw();

void DrawTriangle(Triangle _t);

void TestLagrange();
void TestPlanePoint();
void TestPlaneLine();
void TestPlaneTriangle();

double InputDouble(std::string msg);
Vector3 InputVec3(std::string msg);

bool freezeMouse = false;

bool makeLine = false;

bool makeTri = false;

std::vector<Triangle*> tris = { new Triangle() };
Line line;

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
				
				if (newEvent.key.code == sf::Keyboard::Num4) {
					TestPlaneTriangle();
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

	if (!makeTri && !makeLine && sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		makeLine = true;

		line.a = Vector3::Infinity();
		line.b = Vector3::Infinity();
	}

	if (!makeTri && !makeLine && sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
		makeTri = true;

		tris[0]->a = Vector3::Infinity();
		tris[0]->b = Vector3::Infinity();
		tris[0]->c = Vector3::Infinity();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		if (makeLine) {
			makeLine = false;

			line.a = Vector3::Infinity();
			line.b = Vector3::Infinity();
		}

		if (makeTri) {
			makeTri = false;

			tris[0]->a = Vector3::Infinity();
			tris[0]->b = Vector3::Infinity();
			tris[0]->c = Vector3::Infinity();
		}
	}

	if (!makeTri && !makeLine && line.IsValid() && sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		std::vector<Triangle*> toAdd;

		for (Triangle* _t : tris) {
			if (!_t->IsValid()) continue;

			Vector3 intA = Vector3::LineIntersectsLine(line.a, line.b, _t->a, _t->b);
			Vector3 intB = Vector3::LineIntersectsLine(line.a, line.b, _t->b, _t->c);
			Vector3 intC = Vector3::LineIntersectsLine(line.a, line.b, _t->c, _t->a);

			int count = 0;
			if (!intA.AnyInf()) count++;
			if (!intB.AnyInf()) count++;
			if (!intC.AnyInf()) count++;

			if (count == 0 || count == 1) continue;

			if (intA.AnyInf()) {
				Triangle* t1 = new Triangle();
				Triangle* t2 = new Triangle();
				Triangle* t3 = new Triangle();

				t1->c = _t->c;
				t1->a = intC;
				t1->b = intB;

				t2->c = intC;
				t2->a = _t->a;
				t2->b = _t->b;

				t3->c = intC;
				t3->a = _t->b;
				t3->b = intB;

				toAdd.push_back(t1);
				toAdd.push_back(t2);
				toAdd.push_back(t3);

				_t->c = Vector3::Infinity();
				_t->a = Vector3::Infinity();
				_t->b = Vector3::Infinity();
			}

			if (intB.AnyInf()) {

				Triangle* t1 = new Triangle();
				Triangle* t2 = new Triangle();
				Triangle* t3 = new Triangle();

				t1->a = _t->a;
				t1->b = intA;
				t1->c = intC;

				t2->a = intA;
				t2->b = _t->b;
				t2->c = _t->c;

				t3->a = intA;
				t3->b = _t->c;
				t3->c = intC;

				toAdd.push_back(t1);
				toAdd.push_back(t2);
				toAdd.push_back(t3);

				_t->a = Vector3::Infinity();
				_t->b = Vector3::Infinity();
				_t->c = Vector3::Infinity();
			}

			if (intC.AnyInf()) {
				Triangle* t1 = new Triangle();
				Triangle* t2 = new Triangle();
				Triangle* t3 = new Triangle();

				t1->b = _t->b;
				t1->c = intB;
				t1->a = intA;

				t2->b = intB;
				t2->c = _t->c;
				t2->a = _t->a;

				t3->b = intB;
				t3->c = _t->a;
				t3->a = intA;

				toAdd.push_back(t1);
				toAdd.push_back(t2);
				toAdd.push_back(t3);

				_t->b = Vector3::Infinity();
				_t->c = Vector3::Infinity();
				_t->a = Vector3::Infinity();
			}
		}

		for (Triangle* _t : toAdd) {
			_t->col = sf::Color::Color(rand() % 255, rand() % 255, rand() % 255);
			tris.push_back(_t);
		}

		toAdd.clear();
		

		line.a = Vector3::Infinity();
		line.b = Vector3::Infinity();
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i pos = sf::Mouse::getPosition(*game.wind);

		if (!freezeMouse) {
			freezeMouse = true;

			if (makeLine) {
				if (line.a == Vector3::Infinity()) {
					line.a = (Vector3)pos;
				}
				else if (line.b == Vector3::Infinity()) {
					line.b = (Vector3)pos;
					makeLine = false;
				}
				else {
					makeLine = false;
				}
			} 
			else if (makeTri) {
				if (tris[0]->a == Vector3::Infinity()) {
					tris[0]->a = (Vector3)pos;
				}
				else if (tris[0]->b == Vector3::Infinity()) {
					tris[0]->b = (Vector3)pos;
				}
				else if (tris[0]->c == Vector3::Infinity()) {
					tris[0]->c = (Vector3)pos;
					makeTri = false;
				}
				else {
					makeTri = false;
				}
			}
		}
	}
	else {
		if (freezeMouse) freezeMouse = false;
	}
	

	return 1;
}

void Draw() {
	game.wind->clear();

	for (sf::Drawable* item : game.toDraw)
	{
		game.wind->draw((*item));
	}

	if (!makeTri) {
		DrawTriangle(*tris[0]);
	}

	for (Triangle* _t : tris) {
		DrawTriangle(*_t);

		Vector3 intA = Vector3::LineIntersectsLine(line.a, line.b, _t->a, _t->b);
		Vector3 intB = Vector3::LineIntersectsLine(line.a, line.b, _t->b, _t->c);
		Vector3 intC = Vector3::LineIntersectsLine(line.a, line.b, _t->c, _t->a);

		sf::CircleShape c;
		c.setFillColor(sf::Color::Red);
		c.setRadius(5.0f);
		c.setOrigin(5.0f, 5.0f);

		if (!intA.AnyInf()) {
			c.setPosition((sf::Vector2f)intA);
			game.wind->draw(c);
		}
		if (!intB.AnyInf()) {
			c.setPosition((sf::Vector2f)intB);
			game.wind->draw(c);
		}
		if (!intC.AnyInf()) {
			c.setPosition((sf::Vector2f)intC);
			game.wind->draw(c);
		}
	}

	if (makeLine) {
		sf::CircleShape c;
		c.setFillColor(sf::Color::Red);
		c.setRadius(5.0f);
		c.setOrigin(5.0f, 5.0f);
		

		if (line.a != Vector3::Infinity()) {
			c.setPosition((sf::Vector2f)line.a);
			game.wind->draw(c);
		}

		c.setPosition((sf::Vector2f)sf::Mouse::getPosition(*game.wind));
		game.wind->draw(c);
	}
	
	if (makeTri) {
		sf::CircleShape c;
		c.setFillColor(sf::Color::Red);
		c.setRadius(5.0f);
		c.setOrigin(5.0f, 5.0f);


		if (tris[0]->a != Vector3::Infinity()) {
			c.setPosition((sf::Vector2f)tris[0]->a);
			game.wind->draw(c);
		}
		if (tris[0]->b != Vector3::Infinity()) {
			c.setPosition((sf::Vector2f)tris[0]->b);
			game.wind->draw(c);
		}

		c.setPosition((sf::Vector2f)sf::Mouse::getPosition(*game.wind));
		game.wind->draw(c);
	}

	if (!makeLine) {
		sf::VertexArray Llines(sf::LinesStrip, 2);
		Llines[0].position = (sf::Vector2f)line.a;
		Llines[1].position = (sf::Vector2f)line.b;
		Llines[0].color = sf::Color::Red;
		Llines[1].color = sf::Color::Red;
		game.wind->draw(Llines);
	}

	//Update main window
	game.wind->display();
}

void DrawTriangle(Triangle _t)
{
	sf::VertexArray Tlines(sf::Triangles, 3);
	Tlines[0].position = (sf::Vector2f)_t.a;
	Tlines[1].position = (sf::Vector2f)_t.b;
	Tlines[2].position = (sf::Vector2f)_t.c;

	Tlines[0].color = _t.col;
	Tlines[1].color = _t.col;
	Tlines[2].color = _t.col;

	game.wind->draw(Tlines);
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
}

void TestPlaneLine() {
	system("CLS");
	std::cout << "-Line Segment vs Plane Function-" << std::endl << std::endl;

	Vector3 pp = InputVec3("Enter Point on Plane (k)");
	std::cout << std::endl;

	Vector3 pn = InputVec3("Enter Plane Normal (n)");
	std::cout << std::endl;

	Vector3 lpa = InputVec3("Enter Line point (a)");
	std::cout << std::endl;

	Vector3 lpb = InputVec3("Enter Line point (b)");
	std::cout << std::endl;

	bool doesInt = false;
	Vector3 intersectPoint = Vector3::LineIntersectPlanePoint(pp, pn, lpa, lpb, &doesInt);

	std::cout << (doesInt ? "The Line intersects the Plane at point I: " + intersectPoint.ToString() : "They Line does NOT intersect the Plane") << std::endl;
}

void TestPlaneTriangle() {
	system("CLS");
	std::cout << "-Triangle vs Plane Function-" << std::endl << std::endl;

	Vector3 pp = InputVec3("Enter Point on Plane (k)");
	std::cout << std::endl;

	Vector3 pn = InputVec3("Enter Plane Normal (n)");
	std::cout << std::endl;

	Vector3 ta = InputVec3("Enter Triangle point (a)");
	std::cout << std::endl;

	Vector3 tb = InputVec3("Enter Triangle point (b)");
	std::cout << std::endl;

	Vector3 tc = InputVec3("Enter Triangle point (c)");
	std::cout << std::endl;

	bool doesInt = Vector3::TriangleIntersectsPlane(pp, pn, ta, tb, tc);

	std::cout << (doesInt ? "The Triangle intersects the Plane.": "The Triangle does NOT intersect the Plane.") << std::endl;
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