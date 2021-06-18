#include "Logic.h"

Logic::Logic()
{
	initCubes();
	initWalls();
	initEdges();

	activeCubeShape.setSize(sf::Vector2f(cubeWidth, cubeWidth));
	activeCubeShape.setFillColor(sf::Color::Green);
	activeCubeShape.setPosition(sf::Vector2f(wallWidth, wallWidth));

	srand(time(0));
}

void Logic::initCubes()
{
	for (int i = 0; i < rows; ++i)
	{
		std::vector<Item> row;
		for (int j = 0; j < cols; ++j)
		{
			float x = (cubeWidth + wallWidth) * j + wallWidth;
			float y = (cubeWidth + wallWidth) * i + wallWidth; 
			row.push_back(Item(i, j, cubeWidth, cubeWidth, x, y, sf::Color::Blue));
		}
		cubes.push_back(row);
	}
	activeCube = &cubes[0][0];
}

void Logic::initWalls()
{
	for (int n = 0; n < 2; ++n)
	{
		std::vector<std::vector<Item>> wall_rows;
		for (int i = 0; i < rows - 1; ++i)
		{
			std::vector<Item> row;
			for (int j = 0; j < cols; ++j)
			{
				float width, height, x, y;
				if (!n) // vertical
				{
					width = wallWidth;
					height = cubeWidth + wallWidth * 2;
					x = cubeWidth * (i + 1) + wallWidth * i + wallWidth;
					y = (cubeWidth + wallWidth) * j - wallWidth + wallWidth;
				}
				else // horizontal
				{
					width = cubeWidth + wallWidth * 2;
					height = wallWidth;
					x = (cubeWidth + wallWidth) * j - wallWidth + wallWidth;
					y = cubeWidth * (i + 1) + wallWidth * i + wallWidth;
				}
				row.push_back(Item(i, j, width, height, x, y, sf::Color::Black));
			}
			wall_rows.push_back(row);
		}
		walls.push_back(wall_rows);
	}
}

void Logic::initEdges()
{
	sf::VideoMode windowValues = windowSize();
	float w = windowValues.width;
	float h = windowValues.height;
	
	edge(w, wallWidth, 0.f, 0.f); // top
	edge(w, wallWidth, 0.f, h - wallWidth); // bottom
	edge(wallWidth, h, 0.f, 0.f); // left
	edge(wallWidth, h, w - wallWidth, 0.f); // right
}

void Logic::edge(float width, float height, float x, float y)
{
	sf::RectangleShape shape(sf::Vector2f(width, height));
	shape.setPosition(sf::Vector2f(x, y));
	shape.setFillColor(sf::Color::Black);
	edges.push_back(shape);
}

sf::VideoMode Logic::windowSize()
{
	int width = cubeWidth * cols + wallWidth * (cols - 1) + wallWidth * 2;
	int height = cubeWidth * rows + wallWidth * (rows - 1) + wallWidth * 2;
	return sf::VideoMode(width, height);
}

void Logic::draw(sf::RenderWindow& window)
{
	// draw edges
	for (int i = 0; i < edges.size(); ++i)
	{
		window.draw(edges[i]);
	}

	// draw cubes
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (!cubes[i][j].visited) window.draw(cubes[i][j].shape);
		}
	}
	
	// draw walls
	for (int n = 0; n < 2; ++n)
	{
		for (int i = 0; i < rows - 1; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				if (!walls[n][i][j].visited) window.draw(walls[n][i][j].shape);
			}
		}
	}

	// draw active cube
	if (checkedCount != rows * cols)
	{
		window.draw(activeCubeShape);
	}
}

void Logic::update()
{
	if (checkedCount == rows * cols) return;

	if (!activeCube->visited)
	{
		if (checkedCount > 0)
		{
			clearWall(ci, cj, activeCube->i, activeCube->j);
		}
		activeCube->visited = true;
		++checkedCount;
	}

	activeCubeShape.setPosition(activeCube->shape.getPosition());

	std::vector<Item*> neighbours = getNeighbours(activeCube);
	if (neighbours.size())
	{
		ci = activeCube->i;
		cj = activeCube->j;

		stack.push_back(activeCube);
		activeCube = neighbours[rand() % neighbours.size()];
	}
	else
	{
		activeCube = stack[stack.size() - 1];
		stack.erase(stack.begin() + stack.size() - 1);
	}
}

std::vector<Item*> Logic::getNeighbours(Item* cube)
{
	std::vector<Item*> neighbours;
	
	getSingleNeighbour(activeCube->i - 1, activeCube->j, activeCube->i > 0, neighbours); // top
	getSingleNeighbour(activeCube->i, activeCube->j + 1, activeCube->j < cols - 1, neighbours); // right
	getSingleNeighbour(activeCube->i + 1, activeCube->j, activeCube->i < rows - 1, neighbours); // bottom
	getSingleNeighbour(activeCube->i, activeCube->j - 1, activeCube->j > 0, neighbours); // left

	return neighbours;
}

void Logic::getSingleNeighbour(int i, int j, bool condition, std::vector<Item*>& neighbours)
{
	if (condition && !cubes[i][j].visited)
	{
		neighbours.push_back(&cubes[i][j]);
	}
}

void Logic::clearWall(int ci, int cj, int ni, int nj)
{
	int n, i, j;

	n = std::abs(ci - ni);

	if (!n)
	{
		i = std::min(cj, nj);
		j = ci;
	}
	else
	{
		i = std::min(ci, ni);
		j = cj;
	}

	walls[n][i][j].visited = true;
}