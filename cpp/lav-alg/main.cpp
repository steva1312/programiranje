#include <SFML/Graphics.hpp>
#include "Logic.h"

int main()
{
	Logic logic;

	sf::RenderWindow window(logic.windowSize(), "Maze Generator!");
	window.setFramerateLimit(10);
	
	while (window.isOpen())
	{
		// events
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// update
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			logic.update();
		}

		// draw
		window.clear(sf::Color::White);
		logic.draw(window);
		window.display();
	}
}