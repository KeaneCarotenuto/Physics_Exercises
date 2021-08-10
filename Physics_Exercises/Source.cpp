#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <box2d/box2d.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "CVector.h"

int FixedUpdate();
void Draw();

void DrawTriangle(Triangle _t);

void DrawCapsule(Capsule _c);

void TestLagrange();
void TestPlanePoint();
void TestPlaneLine();
void TestPlaneTriangle();

double InputDouble(std::string msg);
Vector3 InputVec3(std::string msg);

bool freezeMouse = false;

bool makeLine = false;

bool makeTri = false;

bool makeCap = false;

std::vector<Triangle*> tris = { new Triangle() };
Line line;

std::vector<Capsule*> caps = { new Capsule(), new Capsule() };

b2Vec2 gravity(0.0f, -10.0f);
b2World world(gravity);

b2BodyDef groundBodyDef;
b2Body* groundBody;
b2PolygonShape groundBox;

b2BodyDef bodyDef;
b2Body* body;
b2PolygonShape dynamicBox;
b2FixtureDef fixtureDef;

int32 velocityIterations = 6;
int32 positionIterations = 2;

//FixedUpdate() call rate
double timeStep = (1.0f / 60.0f);

class CGame {
public:
	sf::Clock time;
	sf::RenderWindow* wind;
	std::vector<sf::Drawable*> toDraw;
}game;

int main() {

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	//Creating Different Windows
	sf::RenderWindow window(sf::VideoMode(800, 600), "ProgramName - By Keane Carotenuto", sf::Style::Default, settings);
	game.wind = &window;

	/*sf::View view = window.getDefaultView();
	view.setSize(800, -600);
	window.setView(view);*/

	//Manages the FixedUpdate() timing
	double stepTime = 0;
	bool drawn = false;
	
	sf::Clock clock;

	
	//Make floor
	groundBodyDef.position.Set(0.0f, -10.0f);
	groundBody = world.CreateBody(&groundBodyDef);
	groundBox.SetAsBox(800.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Make box
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(200.0f / 30.0f, 200.0f / 30.0f);
	body = world.CreateBody(&bodyDef);
	dynamicBox.SetAsBox(20.0f / 30.0f / 2.0f, 20.0f / 30.0f / 2.0f);
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.75f;
	body->CreateFixture(&fixtureDef);
	body->SetTransform(body->GetPosition(), 41 * (double)(M_PI / 180.0));

	//make rect for box
	sf::RectangleShape* test = new sf::RectangleShape();
	test->setPosition(sf::Vector2f(0, 200));
	test->setFillColor(sf::Color::Red);
	test->setSize(sf::Vector2f(20, 20));
	test->setOrigin(10, 10);

	game.toDraw.push_back(test);

	while (window.isOpen() == true)
	{
		stepTime += clock.getElapsedTime().asSeconds();
		clock.restart();

		while (stepTime >= timeStep)
		{
			//Main Loop of Game
			if (FixedUpdate() == 0) return 0;

			stepTime -= timeStep;
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
	//step b2D system
	world.Step(timeStep, velocityIterations, positionIterations);

	//update visual
	b2Vec2 position = body->GetPosition();
	float angle = body->GetAngle();
	sf::RectangleShape* theRect = dynamic_cast<sf::RectangleShape*>(game.toDraw[0]);
	theRect->setPosition(position.x * 30.0, game.wind->getSize().y - position.y * 30.0);
	theRect->setRotation(-angle / (M_PI/ 180.0));

	//move box
	float force = 10;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		body->ApplyForceToCenter({-force,0}, true);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		body->ApplyForceToCenter({ force,0 }, true);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		body->ApplyForceToCenter({ 0,force }, true);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		body->ApplyForceToCenter({ 0,-force }, true);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		body->ApplyTorque(force / 5, true);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		body->ApplyTorque(-force / 5, true);
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

		if (makeCap) {
			makeCap = false;

			caps[0]->a = Vector3::Infinity();
			caps[0]->b = Vector3::Infinity();
			caps[0]->radius = (double)INFINITY;
			
			caps[1]->a = Vector3::Infinity();
			caps[1]->b = Vector3::Infinity();
			caps[1]->radius = (double)INFINITY;
		}
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

	if (!makeTri && !makeLine && line.IsValid() && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
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
					tris[0]->col = sf::Color::White;
					makeTri = false;
				}
				else {
					makeTri = false;
				}
			}
			else if (makeCap) {
				if (caps[0]->a == Vector3::Infinity()) {
					caps[0]->a = (Vector3)pos;
				}
				else if (caps[0]->b == Vector3::Infinity()) {
					caps[0]->b = (Vector3)pos;
				}
				else if (caps[1]->a == Vector3::Infinity()) {
					caps[1]->a = (Vector3)pos;
				}
				else if (caps[1]->b == Vector3::Infinity()) {
					caps[1]->b = (Vector3)pos;
				}
				else {
					makeCap = false;
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

	DrawCapsule(*caps[0]);

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

void DrawCapsule(Capsule _c) 
{
	_c.a = { 100,100,0 };
	_c.b = { 
		sin(game.time.getElapsedTime().asSeconds()) * 50.0 + 100.0 ,
		cos(game.time.getElapsedTime().asSeconds()) * 50.0 + 100.0,
		0 };
	_c.radius = 30;
	_c.col = sf::Color::White;

	sf::CircleShape circle1(_c.radius);
	circle1.setOrigin(_c.radius, _c.radius);
	circle1.setPosition(_c.a);
	circle1.setFillColor(_c.col);

	sf::CircleShape circle2(_c.radius);
	circle2.setOrigin(_c.radius, _c.radius);
	circle2.setPosition(_c.b);
	circle2.setFillColor(_c.col);

	sf::RectangleShape rect(sf::Vector2f((_c.b - _c.a).Mag(), _c.radius*2.0f));
	rect.setOrigin(rect.getSize().x / 2.0f, rect.getSize().y / 2.0f);
	rect.setPosition(_c.a + (_c.b - _c.a) * (0.5f));
	rect.setFillColor(_c.col);
	rect.setRotation((_c.b - _c.a).Angle());

	game.wind->draw(circle1);
	game.wind->draw(circle2);
	game.wind->draw(rect);
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