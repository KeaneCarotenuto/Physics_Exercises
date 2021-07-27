#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>

int FixedUpdate();
void Draw();

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