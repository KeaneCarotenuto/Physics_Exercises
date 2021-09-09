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

bool makeTri = false;
bool makePoly = false;

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
	}

	//Start making triangle
	if (!makeTri && !makePoly && sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
		makeTri = true;

		tris.push_back(new Triangle());

		tris.back()->a = Vector3::Infinity();
		tris.back()->b = Vector3::Infinity();
		tris.back()->c = Vector3::Infinity();
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
		}
	}
	else {
		if (freezeMouse) freezeMouse = false;
	}
	

	return 1;
}

void Draw() {
	game.wind->clear();

	//Draw all other triangles + intersections with line
	for (Triangle* _t : tris) {
		if (!_t->IsValid()) continue;

		sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(*game.wind);
		float angleA = Vector3::Angle(_t->a - Vector3(mousePos), _t->b - Vector3(mousePos));
		float angleB = Vector3::Angle(_t->b - Vector3(mousePos), _t->c - Vector3(mousePos));
		float angleC = Vector3::Angle(_t->c - Vector3(mousePos), _t->a - Vector3(mousePos));

		if (360.0 - abs((float)(angleA + angleB + angleC)) <= 0.01) {
			_t->col = sf::Color::Red;
		}
		else {
			_t->col = sf::Color::White;
		}

		DrawTriangle(*_t);
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