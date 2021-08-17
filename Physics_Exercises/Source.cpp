#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "CVector.h"

void ToggleControls();

int FixedUpdate();
void Draw();

void DrawLine(Line _line, sf::Color _col);

void DrawTriangle(Triangle _t);

void DrawCapsule(Capsule _c);

void TestLagrange();
void TestPlanePoint();
void TestPlaneLine();
void TestPlaneTriangle();

double InputDouble(std::string msg);
Vector3 InputVec3(std::string msg);

sf::Font font;

//In program instructions/controls
sf::Text instructions;
std::string defaultInstruct = "[TAB] TO OPEN AND CLOSE CONTROLS\n";
std::string fullInstruct =	
	"[1] TO TEST LAGRANGE'S FORMULA\n"\
	"[2] TO TEST PLANE VS POINT FUNCTION\n"\
	"[3] TO TEST LINE SEGMENT VS PLANE FUNCTION\n"\
	"[4] TO TEST TRIANGLE VS PLANE FUNCTION\n"\
	"[NOTE] PRESS NUMBER KEYS, THEN CHECK CONSOLE\n\n"\
	"[T] TO CLEAR/START TRIANGLE\n"\
	"[L] TO CLEAR/START LINE\n"\
	"[D] TO DIVIDE TRIANGLE(S) BY LINE\n"\
	"[C] TO CLEAR/START CAPSULES\n\n"\
	"[LEFT CLICK] TO CREATE TRIANGLE/LINE/CAPSULES\n"\
	"[ESC] TO CANCEL SHAPE\n"\
	"[TAB] TO OPEN AND CLOSE CONTROLS\n";

bool freezeMouse = false;

bool makeLine = false;

bool makeTri = false;

bool makeCap = false;

//One triangle
std::vector<Triangle*> tris = { new Triangle() };

//One line
Line line;

//Two capsules
std::vector<Capsule*> caps = { 
	new Capsule(Vector3::Infinity(), Vector3::Infinity(), (double)INFINITY, sf::Color::Color(255,0,0)),
	new Capsule(Vector3::Infinity(), Vector3::Infinity(), (double)INFINITY, sf::Color::Color(0,0,255))
};

//FixedUpdate() call rate
double timeStep = (1.0f / 60.0f);

class CGame {
public:
	sf::Clock time;
	sf::RenderWindow* wind = nullptr;
}game;

int main() {

	//Make lines and such look a bit better
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	//Creating Different Windows
	sf::RenderWindow window(sf::VideoMode(1600, 800), "ProgramName - By Keane Carotenuto", sf::Style::Default, settings);
	game.wind = &window;

	//Load font
	if (!font.loadFromFile("Resources/DMMono-Regular.ttf"))
	{
		std::cout << "ERROR: Failed to load font";
	}

	//Set up controls info
	instructions.setFont(font);
	instructions.setFillColor(sf::Color::White);
	instructions.setOutlineColor(sf::Color::Black);
	instructions.setOutlineThickness(5.0f);
	instructions.setCharacterSize(20);
	instructions.setLineSpacing(1.5f);
	instructions.setString(defaultInstruct);
	instructions.setOrigin(0, instructions.getGlobalBounds().height);
	instructions.setPosition(20, 800);

	//Manages the FixedUpdate() timing
	double stepTime = 0;
	bool drawn = false;
	
	sf::Clock clock;

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

		
		while (window.pollEvent(newEvent))
		{
			//Quit
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

				if (newEvent.key.code == sf::Keyboard::Key::Tab) {
					ToggleControls();
				}
			}

			
		}
	}

	return 0;
}

/// <summary>
/// Toggle the controls help in game
/// </summary>
void ToggleControls()
{
	if (instructions.getString() == defaultInstruct) {
		instructions.setString(fullInstruct);
	}
	else {
		instructions.setString(defaultInstruct);
	}

	instructions.setOrigin(0, instructions.getGlobalBounds().height);
	instructions.setPosition(20, 800);


}

int FixedUpdate() {
	//Esc to reset
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

	//Start making line
	if (!makeTri && !makeLine && sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
		makeLine = true;

		line.a = Vector3::Infinity();
		line.b = Vector3::Infinity();
	}

	//Start making triangle
	if (!makeTri && !makeLine && sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
		makeTri = true;

		tris[0]->a = Vector3::Infinity();
		tris[0]->b = Vector3::Infinity();
		tris[0]->c = Vector3::Infinity();
	}

	//Start making capsules
	if (!makeTri && !makeLine && sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		makeCap = true;

		caps[0]->a = Vector3::Infinity();
		caps[0]->b = Vector3::Infinity();
		caps[0]->radius = (double)INFINITY;

		caps[1]->a = Vector3::Infinity();
		caps[1]->b = Vector3::Infinity();
		caps[1]->radius = (double)INFINITY;
	}

	//Try ivide triangle by line
	if (!makeTri && !makeLine && line.IsValid() && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		std::vector<Triangle*> toAdd;

		for (Triangle* _t : tris) {
			if (!_t->IsValid()) continue;

			//Find all intersections
			Vector3 intA = Vector3::LineIntersectsLine(line.a, line.b, _t->a, _t->b);
			Vector3 intB = Vector3::LineIntersectsLine(line.a, line.b, _t->b, _t->c);
			Vector3 intC = Vector3::LineIntersectsLine(line.a, line.b, _t->c, _t->a);

			//Count intersections
			int count = 0;
			if (!intA.AnyInf()) count++;
			if (!intB.AnyInf()) count++;
			if (!intC.AnyInf()) count++;

			//Only divide if enough intersections
			if (count == 0 || count == 1) continue;

			//Make new triangles
			Triangle* t1 = new Triangle();
			Triangle* t2 = new Triangle();
			Triangle* t3 = new Triangle();

			//Determine how the triangle must be cut
			if (intA.AnyInf()) {

				t1->c = _t->c;
				t1->a = intC;
				t1->b = intB;

				t2->c = intC;
				t2->a = _t->a;
				t2->b = _t->b;

				t3->c = intC;
				t3->a = _t->b;
				t3->b = intB;
			}
			else if (intB.AnyInf()) {

				t1->a = _t->a;
				t1->b = intA;
				t1->c = intC;

				t2->a = intA;
				t2->b = _t->b;
				t2->c = _t->c;

				t3->a = intA;
				t3->b = _t->c;
				t3->c = intC;
			}
			else if (intC.AnyInf()) {
				

				t1->b = _t->b;
				t1->c = intB;
				t1->a = intA;

				t2->b = intB;
				t2->c = _t->c;
				t2->a = _t->a;

				t3->b = intB;
				t3->c = _t->a;
				t3->a = intA;
			}

			//Add new triangles
			toAdd.push_back(t1);
			toAdd.push_back(t2);
			toAdd.push_back(t3);

			//clear old tri
			_t->b = Vector3::Infinity();
			_t->c = Vector3::Infinity();
			_t->a = Vector3::Infinity();
		}

		//Actually add new tris
		for (Triangle* _t : toAdd) {
			_t->col = sf::Color::Color(rand() % 255, rand() % 255, rand() % 255);
			tris.push_back(_t);
		}

		toAdd.clear();
		
		//Clear line
		line.a = Vector3::Infinity();
		line.b = Vector3::Infinity();
	}

	//Creating shapes (Line, Triangle, Capsules)
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
				else if (caps[1]->radius == (double)INFINITY) {
					caps[0]->radius = (caps[0]->a - (Vector3)pos).Mag();
					caps[1]->radius = caps[0]->radius;
				}
				else if (caps[0]->b == Vector3::Infinity()) {
					caps[0]->b = (Vector3)pos;
				}
				else if (caps[1]->a == Vector3::Infinity()) {
					caps[1]->a = (Vector3)pos;
				}
				else if (caps[1]->b == Vector3::Infinity()) {
					caps[1]->b = (Vector3)pos;
					makeCap = false;
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

	//Update rad dynamically after first click when making capsules
	if (makeCap && caps[0]->a != Vector3::Infinity() && caps[1]->radius == (double)INFINITY) {
		caps[0]->radius = (caps[0]->a - (Vector3)sf::Mouse::getPosition(*game.wind)).Mag();
	}
	

	return 1;
}

void Draw() {
	game.wind->clear();

	DrawCapsule(*caps[0]);
	DrawCapsule(*caps[1]);

	//Draw cap shortest line
	DrawLine(Capsule::ShortestDistanceBetween(*caps[0], *caps[1]), sf::Color::White);

	//Draw basic triangle
	if (!makeTri) {
		DrawTriangle(*tris[0]);
	}

	//Draw all other triangles + intersections with line
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
	}

	//Draw dot on cursor to show clicks are needed
	if (makeLine || makeTri || makeCap) {
		sf::CircleShape c;
		c.setFillColor(sf::Color::Red);
		c.setRadius(5.0f);
		c.setOrigin(5.0f, 5.0f);
		c.setPosition((sf::Vector2f)sf::Mouse::getPosition(*game.wind));
		game.wind->draw(c);
	}

	if (!makeLine) {
		DrawLine(line, sf::Color::Red);
	}

	game.wind->draw(instructions);

	//Update main window
	game.wind->display();
}

void DrawLine(Line _line, sf::Color _col)
{
	sf::VertexArray Llines(sf::LinesStrip, 2);
	Llines[0].position = (sf::Vector2f)_line.a;
	Llines[1].position = (sf::Vector2f)_line.b;
	Llines[0].color = _col;
	Llines[1].color = _col;
	game.wind->draw(Llines);
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
	sf::CircleShape circle1((float)_c.radius);
	circle1.setOrigin((float)_c.radius, (float)_c.radius);
	circle1.setPosition(_c.a);
	circle1.setFillColor(_c.col);

	sf::CircleShape circle2((float)_c.radius);
	circle2.setOrigin((float)_c.radius, (float)_c.radius);
	circle2.setPosition(_c.b);
	circle2.setFillColor(_c.col);

	sf::RectangleShape rect(sf::Vector2f((float)(_c.b - _c.a).Mag(), (float)_c.radius*2.0f));
	rect.setOrigin(rect.getSize().x / 2.0f, rect.getSize().y / 2.0f);
	rect.setPosition(_c.a + (_c.b - _c.a) * (0.5f));
	rect.setFillColor(_c.col);
	rect.setRotation((float)(_c.b - _c.a).Angle());

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

/// <summary>
/// Gets a double from the user in the console
/// </summary>
/// <param name="msg"></param>
/// <returns></returns>
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

/// <summary>
/// Gets a vector 3 from the user in the console
/// </summary>
/// <param name="msg"></param>
/// <returns></returns>
Vector3 InputVec3(std::string msg) {
	std::cout << msg << std::endl;
	double x = InputDouble("double x: ");
	double y = InputDouble("double y: ");
	double z = InputDouble("double z: ");
	return Vector3(x, y, z);
}