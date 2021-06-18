#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Block.h"

// global variables
int cols = 20, rows = 20, wall = 3, width = 30;
std::vector<std::vector<Block>> blocks;
bool is_holding = false;
Block* last = nullptr;
std::vector<Block*> path;

// function declarations
void InitBlocks();
Block* GetBlock(int, int);
sf::VideoMode WindowSize();
void Events(sf::RenderWindow&);
void Draw(sf::RenderWindow&);
void Generate();

struct Player
{
	int x, y;
	sf::RectangleShape shape;
	sf::Keyboard::Key up, down, left, right;

	Player(int i, int j, sf::Keyboard::Key u, sf::Keyboard::Key d, sf::Keyboard::Key l, sf::Keyboard::Key r)
		: x(i), y(j), up(u), down(d), left(l), right(r)
	{
		shape.setSize(sf::Vector2f((float)width, (float)width));
		shape.setFillColor(sf::Color::Yellow);
		ChangeShapePosition();
	}

	void Move()
	{
		if (sf::Keyboard::isKeyPressed(up))
		{
			if (y > 0) y -= 1;
		}
		if (sf::Keyboard::isKeyPressed(down))
		{
			if (y < rows - 1) y += 1;
		}
		if (sf::Keyboard::isKeyPressed(left))
		{
			if (x > 0) x -= 1;
		}
		if (sf::Keyboard::isKeyPressed(right))
		{
			if (x < cols - 1) x += 1;
		}

		ChangeShapePosition();

		Block* active = GetBlock(x, y);
		if (active->wall) active->ChangeState();
	}

private:
	void ChangeShapePosition()
	{
		shape.setPosition(sf::Vector2f((float)(width + wall) * x, (float)(width + wall) * y));
	}
};

Player p1(0, 0, sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D);
Player p2(cols - 1, rows - 1, sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right);


int main()
{
	InitBlocks();
	Generate();

	sf::RenderWindow window(WindowSize(), "Path Finding!");
	window.setFramerateLimit(10);

	while (window.isOpen())
	{
		Events(window);
		Draw(window);
	}
}

void InitBlocks()
{
	for (int i = 0; i < cols; ++i)
	{
		std::vector<Block> temp_col;
		for (int j = 0; j < rows; ++j)
		{
			temp_col.push_back(Block(i, j, float(width), float(wall)));
		}
		blocks.push_back(temp_col);
	}
}

Block* GetBlock(int x, int y)
{
	return &blocks[x][y];
}

sf::VideoMode WindowSize()
{
	int w = cols * width + (cols - 1) * wall;
	int h = rows * width + (cols - 1) * wall;
	return sf::VideoMode(w, h);
}

void Events(sf::RenderWindow& window)
{
	sf::Event e;
	while (window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed) window.close();

		if (e.type == sf::Event::MouseButtonPressed)
		{
			is_holding = true;
		}
		if (e.type == sf::Event::MouseButtonReleased)
		{
			is_holding = false;
			last = nullptr;
		}
	}

	if (is_holding)
	{
		int pos_x = sf::Mouse::getPosition(window).x;
		int pos_y = sf::Mouse::getPosition(window).y;
		int mx = pos_x % (width + wall);
		int my = pos_y % (width + wall);

		if (mx < width && my < width)
		{
			int col = pos_x / (width + wall);
			int row = pos_y / (width + wall);

			if (col >= 0 && col <= cols && row >= 0 && row <= rows - 1)
			{
				Block* clicked = GetBlock(col, row);

				if (last != clicked && (clicked->x != p1.x || clicked->y != p1.y) && (clicked->x != p2.x || clicked->y != p2.y))
				{
					clicked->ChangeState();
					last = clicked;
				}
			}
		}
	}

	p1.Move();
	p2.Move();

	Generate();
}

void Draw(sf::RenderWindow& window)
{
	window.clear(sf::Color::Black);

	// draw blocks
	for (int i = 0; i < cols; ++i)
	{
		for (int j = 0; j < rows; ++j)
		{
			window.draw(blocks[i][j].shape);
		}
	}

	// draw path
	if (path.size())
	{
		for (int i = 0; i < path.size() - 1; ++i)
		{
			sf::RectangleShape line;

			float w, h;
			Block* current = path[i];
			Block* next = path[i + 1];

			if (std::abs(current->x - next->x)) // horizontal
			{
				w = width + wall + width / 5.f;
				h = width / 5.f;
			}
			else // vertical
			{
				w = width / 5.f;
				h = width + wall + width / 5.f;
			}

			int min_x = std::min(current->x, next->x);
			int min_y = std::min(current->y, next->y);
			float x = min_x * wall + min_x * width + width / 2.f - width / 5.f / 2.f;
			float y = min_y * wall + min_y * width + width / 2.f - width / 5.f / 2.f;

			line.setSize(sf::Vector2f(w, h));
			line.setPosition(sf::Vector2f(x, y));
			line.setFillColor(sf::Color::Yellow);

			window.draw(line);
		}
	}

	// draw players
	window.draw(p1.shape);
	window.draw(p2.shape);

	window.display();
}

// generate functions
Block* Min(std::vector<Block*> &list)
{
	Block* b = list[0];
	for (int i = 1; i < list.size(); ++i)
	{
		if (list[i]->f < b->f) b = list[i];
	}
	return b;
}

void Remove(Block* b, std::vector<Block*> &list)
{
	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i] == b) list.erase(list.begin() + i);
	}
}

bool Exists(Block* b, std::vector<Block*>& list)
{
	for (int i = 0; i < list.size(); ++i)
	{
		if (list[i] == b) return true;
	}
	return false;
}

std::vector<Block*> GetNeighbours(Block* b)
{
	std::vector<Block*> neighbours;
	
	if (b->y > 0) // top
	{
		neighbours.push_back(&blocks[b->x][b->y - 1]);
	}
	if (b->x < cols - 1) // right
	{
		neighbours.push_back(&blocks[b->x + 1][b->y]);
	}
	if (b->y < rows - 1) // bottom
	{
		neighbours.push_back(&blocks[b->x][b->y + 1]);
	}
	if (b->x > 0) // left
	{
		neighbours.push_back(&blocks[b->x - 1][b->y]);
	}

	return neighbours;
}

void Generate()
{
	path.clear();

	std::vector<Block*> open;
	std::vector<Block*> closed;
	open.push_back(GetBlock(p1.x, p1.y));
	Block* current;
	bool path_exsists = false;

	while (1)
	{
		if (open.size() == 0) break;

		current = Min(open);
		Remove(current, open);
		closed.push_back(current);

		if (current == &blocks[p2.x][p2.y])
		{
			path_exsists = true;
			break;
		}

		auto neighbours = GetNeighbours(current);

		for (int i = 0; i < neighbours.size(); ++i)
		{
			Block* neighbour = neighbours[i];

			if (!neighbour->wall && !Exists(neighbour, closed))
			{
				if (!Exists(neighbour, open))
				{
					neighbour->F(&blocks[p1.x][p1.y], &blocks[p2.x][p2.y]);
					neighbour->parent = current;
					open.push_back(neighbour);
				}
			}
		}
	}

	if (path_exsists)
	{
		while (1)
		{
			path.push_back(current);

			if (current == &blocks[p1.x][p1.y]) break;

			current = current->parent;
		}
	}
}