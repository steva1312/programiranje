#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

struct Item
{
	int i, j;
	bool visited = false;
	sf::RectangleShape shape;

	Item(int row, int col, float width, float height, float x, float y, sf::Color color)
	{
		i = row, j = col;

		shape.setSize(sf::Vector2f(width, height));
		shape.setPosition(sf::Vector2f(x, y));
		shape.setFillColor(color);
	}
};

class Logic
{
private: // setup variables
	const int rows = 15, cols = 15;
	const float cubeWidth = 25.f, wallWidth = 10.f;
	int checkedCount = 0;
	int ci, cj; // last i and j of active cube
	Item* activeCube;
	std::vector<std::vector<Item>> cubes; // [row][col]
	std::vector<std::vector<std::vector<Item>>> walls; // [hor || ver][row][col]
	std::vector<Item*> stack;
	std::vector<sf::RectangleShape> edges;
	sf::RectangleShape activeCubeShape;

public:
	Logic();
	sf::VideoMode windowSize();
	void draw(sf::RenderWindow&);
	void update();

private:
	void initCubes();
	void initWalls();
	void initEdges();
	void edge(float, float, float, float);
	std::vector<Item*> getNeighbours(Item*);
	void getSingleNeighbour(int , int, bool, std::vector<Item*>&);
	void clearWall(int, int, int, int);
};