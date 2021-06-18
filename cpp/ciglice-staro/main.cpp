#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

// global variables
int win_width = 800, win_height = 600;

struct Player
{
	float width = 75, height = 20;
	float x = (win_width - width) / 2, y = 600 - height - 50;
	float vel = 1.25f;
	int dir = 0;
	sf::RectangleShape shape;

	Player()
	{
		shape.setSize(sf::Vector2f(width, height));
		shape.setPosition(sf::Vector2f(x, y));
		shape.setFillColor(sf::Color::Red);
	}

	float left() { return x; }
	float right() { return x + width; }
	float up() { return y; }

	void move()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) dir = -1;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) dir = 1;
		else dir = 0;

		if (x + vel * dir > 0 && x + vel * dir + width < win_width) {
			x += vel * dir;
			shape.setPosition(sf::Vector2f(x, y));
		}
	}
};

struct Ball
{
	float radius = 10.f;
	float x = win_width / 2.f, y = win_height / 3.f * 2.f;
	float vel = 1.f;
	int dirx = 1, diry = -1;
	sf::CircleShape shape;

	Ball()
	{
		shape.setRadius(radius);
		shape.setPosition(sf::Vector2f(x, y));
		shape.setFillColor(sf::Color::White);
	}

	float left() { return x - radius; }
	float right() { return x + radius; }
	float up() { return y - radius; }
	float down() { return y + radius; }

	void move(Player &p)
	{
		// check walls
		if (left() + vel * dirx < 0) dirx = 1;
		else if (right() + vel * dirx > win_width) dirx = -1;
		if (up() + vel * diry < 0) diry = 1;
		else if (down() + vel * diry > win_height) diry = -1;

		// check player
		if (x >= p.left() && x <= p.right() && down() == p.up())
		{
			if (p.dir != 0) dirx = p.dir;
			diry = -1;
		}

		// change position
		x += vel * dirx;
		y += vel * diry;
		shape.setPosition(sf::Vector2f(x, y));
	}
};

struct Brick
{
	float x, y;
	float width, height;
	bool hit = false;
	sf::RectangleShape shape;

	Brick(float bx, float by, float w, float h)
	{
		x = bx;
		y = by;
		width = w;
		height = h;
		shape.setSize(sf::Vector2f(width, height));
		shape.setPosition(sf::Vector2f(x, y));
		shape.setFillColor(sf::Color(255, 165, 0));
	}

	float left() { return x; };
	float right() { return x + width; };
	float up() { return y; };
	float down() { return y + height; };
};

struct Table
{
	int rows = 7, cols = 15;
	float margin = 5.f;
	float brick_width = (win_width - margin * (cols + 1)) / cols, brick_height = 15.f;
	std::vector<Brick> table;

	Table()
	{
		std::cout << brick_width;
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < cols; ++j)
				table.push_back(Brick((j + 1) * margin + brick_width * j, (i + 1) * margin + brick_height * i, brick_width, brick_height));
	}

	void check(Ball &b)
	{
		for (int i = 0; i < rows * cols; ++i)
		{

			Brick brick = table[i];
			if (brick.hit) continue;

			bool is_hit = false;

			if (b.x >= brick.left() && b.x <= brick.right() && b.up() == brick.down()) { b.diry = 1; is_hit = true; }
			else if (b.x >= brick.left() && b.x <= brick.right() && b.down() == brick.up()) { b.diry = -1; is_hit = true; }
			else if (b.y >= brick.up() && b.y <= brick.down() && b.right() == brick.left()) { b.dirx = -1; is_hit = true; }
			else if (b.y >= brick.up() && b.y <= brick.down() && b.left() == brick.right()) { b.dirx = 1; is_hit = true; }

			if (is_hit) table[i].hit = true;
		}
	}
};

int main()
{
	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Ciglice");
	window.setFramerateLimit(360);

	Player player;
	Ball ball;
	Table table;

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window.close();
		}

		// move
		player.move();
		table.check(ball);
		ball.move(player);

		// draw
		window.clear(sf::Color::Black);

		window.draw(player.shape);
		for (Brick brick : table.table)
		{
			if (brick.hit) continue;
			window.draw(brick.shape);
		}
		window.draw(ball.shape);

		window.display();
	}
}