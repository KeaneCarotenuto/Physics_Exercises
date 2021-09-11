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
void DrawPoly(RegPolygon _p);

void TestCannon();

bool Within(double _val, double min, double max);
double InputDouble(std::string msg);
Vector3 InputVec3(std::string msg);

sf::Font font;

//In program instructions/controls
sf::Text instructions;
std::string defaultInstruct = "[TAB] TO OPEN AND CLOSE CONTROLS\n";
std::string fullInstruct =	
	"[1] TO TEST ANGLE POINT IN TRIANGLE\n"\
	"[2] TO TEST BARYCENTRIC POINT IN TRIANGLE\n"\
	"[3] TO TEST JAVELIN THROW\n"\
	"[4] TO TEST CANNON ANGLE\n"\
	"[NOTE] PRESS NUMBER KEYS, THEN CHECK CONSOLE FOR READOUTS\n\n"\
	"[T] TO CLEAR/START TRIANGLE\n"\
	"[P] TO CLEAR/START POLYGON\n"\
	"[R] TO CLEAR/RESTART ALL\n\n"\
	"[LEFT CLICK] TO CREATE TRIANGLE/POLYGON POINT\n"\
	"[RIGHT CLICK] TO FINISH POLYGON\n"\
	"[ESC] TO CANCEL SHAPE\n"\
	"[TAB] TO OPEN AND CLOSE CONTROLS\n";

bool freezeMouse = false;

bool makeTri = false;
bool makePoly = false;

bool doBarry = false;

//One triangle
std::vector<Triangle*> tris = {};
std::vector<RegPolygon*> polys = {};

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
				return 0;
			}

			if (newEvent.type == sf::Event::KeyPressed)
			{
				if (newEvent.key.code == sf::Keyboard::Num1) {
					doBarry = false;
				}
				
				if (newEvent.key.code == sf::Keyboard::Num2) {
					doBarry = true;
				}

				if (newEvent.key.code == sf::Keyboard::Key::Num3) {

				}

				if (newEvent.key.code == sf::Keyboard::Key::Num4) {
					TestCannon();
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
		if (makeTri) {
			makeTri = false;

			delete tris.back();
			tris.pop_back();
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		makeTri = false;
		makePoly = false;

		for (Triangle* _t : tris) {
			delete _t;
		}
		tris.clear();

		for (RegPolygon* _p : polys) {
			delete _p;
		}
		polys.clear();
	}

	//Start making triangle
	if (!makeTri && !makePoly && sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
		makeTri = true;

		tris.push_back(new Triangle());

		tris.back()->a = Vector3::Infinity();
		tris.back()->b = Vector3::Infinity();
		tris.back()->c = Vector3::Infinity();
	}

	//Start making poly
	if (!makeTri && !makePoly && sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
		makePoly = true;

		polys.push_back(new RegPolygon());
	}

	//Creating shapes (Line, Triangle, Capsules)
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Vector2i pos = sf::Mouse::getPosition(*game.wind);

		if (!freezeMouse) {
			freezeMouse = true;

			if (makeTri) {
				if (tris.back()->a == Vector3::Infinity()) {
					tris.back()->a = (Vector3)pos;
				}
				else if (tris.back()->b == Vector3::Infinity()) {
					tris.back()->b = (Vector3)pos;
				}
				else if (tris.back()->c == Vector3::Infinity()) {
					tris.back()->c = (Vector3)pos;
					tris.back()->col = sf::Color::White;
					makeTri = false;
				}
				else {
					makeTri = false;
				}
			}
			else if (makePoly) {
				polys.back()->points.push_back((Vector3)pos);
			}
		}
	}
	else {
		if (freezeMouse) freezeMouse = false;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		if (makePoly) {
			makePoly = false;
			polys.back()->col = sf::Color::White;
		}
	}
	

	return 1;
}

void Draw() {
	game.wind->clear();

	//Draw all triangles
	for (Triangle* _t : tris) {
		if (!_t->IsValid()) continue;

		Vector3 mousePos = Vector3((sf::Vector2f)sf::Mouse::getPosition(*game.wind));

		_t->col = sf::Color::White;

		//Bary + Angle checks
		if (doBarry && _t->BaryPoint(mousePos, true)) _t->col = sf::Color::Red;
		else if (_t->AnglePoint(mousePos, true)) _t->col = sf::Color::Red;

		DrawTriangle(*_t);
	}

	if (!makePoly && polys.size() == 2) {
		std::vector<Vector3>* points = new std::vector<Vector3>();
		points->clear();

		bool outcome = !RegPolygon::intersect(polys[0], polys[1], points);

		sf::CircleShape c;
		c.setFillColor(sf::Color::Red);
		c.setRadius(5.0f);
		c.setOrigin(5.0f, 5.0f);

		for (const Vector3& _p : *points) {
			c.setPosition((sf::Vector2f)_p);
			game.wind->draw(c);
		}

		cprint::Print({ 5,10 }, L"INT: " + (std::wstring)(outcome ? L"YES" : L"NO"));
	}

	//Draw all polygons
	for (RegPolygon* _p : polys) {
		if (!_p->IsValid()) continue;

		DrawPoly(*_p);
	}
	
	if (makeTri) {
		sf::CircleShape c;
		c.setFillColor(sf::Color::Red);
		c.setRadius(5.0f);
		c.setOrigin(5.0f, 5.0f);


		if (tris.back()->a != Vector3::Infinity()) {
			c.setPosition((sf::Vector2f)tris.back()->a);
			game.wind->draw(c);
		}
		if (tris.back()->b != Vector3::Infinity()) {
			c.setPosition((sf::Vector2f)tris.back()->b);
			game.wind->draw(c);
		}
	}

	if (makePoly) {
		sf::CircleShape c;
		c.setFillColor(sf::Color::Red);
		c.setRadius(5.0f);
		c.setOrigin(5.0f, 5.0f);


		for (Vector3 _point : polys.back()->points)
		{
			c.setPosition((sf::Vector2f)_point);
			game.wind->draw(c);
		}
	}

	//Draw dot on cursor to show clicks are needed
	if (makeTri || makePoly) {
		sf::CircleShape c;
		c.setFillColor(sf::Color::Red);
		c.setRadius(5.0f);
		c.setOrigin(5.0f, 5.0f);
		c.setPosition((sf::Vector2f)sf::Mouse::getPosition(*game.wind));
		game.wind->draw(c);
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

void DrawPoly(RegPolygon _p)
{
	sf::VertexArray Plines(sf::LineStrip, _p.points.size() + 1);

	for (int i = 0; i < _p.points.size() + 0; ++i) {
		Plines[i].position = (sf::Vector2f)_p.points[i];
		Plines[i].color = _p.col;
	}
	Plines[_p.points.size()].position = (sf::Vector2f)_p.points[0];
	Plines[_p.points.size()].color = _p.col;

	game.wind->draw(Plines);
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

	sf::RectangleShape rect(sf::Vector2f((float)(_c.b - _c.a).Mag(), (float)_c.radius * 2.0f));
	rect.setOrigin(rect.getSize().x / 2.0f, rect.getSize().y / 2.0f);
	rect.setPosition(_c.a + (_c.b - _c.a) * (0.5f));
	rect.setFillColor(_c.col);
	rect.setRotation((float)(_c.b - _c.a).Angle());

	game.wind->draw(circle1);
	game.wind->draw(circle2);
	game.wind->draw(rect);
}

void TestCannon() {
	system("CLS");

	double l = InputDouble("Enter Length: ");
	double v = InputDouble("Enter Velocity Mag: ");
	Vector3 P = InputVec3("Enter Desired Point to hit (z value is ignored): ");
	double dp = InputDouble("Enter desired DP: ");
	bool* passed = new bool(true);

	if (P.Mag() <= l) {
		std::cout << "\nWanring, Length of barrel equals/exceeds point to hit. May encounter strange results.\n";
	}

	std::cout << std::fixed << std::setprecision((int)dp) << "Angle: " << fmod(Vector3::FindCannonAngle(l, v, P, dp, 1000, passed) / (M_PI) * 180, 360) << " degrees\n";
	if (*passed == true) {
		std::cout << "Completed.\n";
	}
	else
	{
		std::cout << "Cannot Reach.\n";
	}

	system("pause");

	system("CLS");
}

bool Within(double _val, double min, double max) {
	if (_val >= min && _val <= max) return true;
	else return false;
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