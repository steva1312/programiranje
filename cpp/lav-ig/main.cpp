#include <iostream>
#include <vector>
#include <cstdint>
#include <ctime>
#include <chrono>
#include <SFML/Graphics.hpp>

struct Block;
struct Wall;

// global variables
int cols, rows;
const float block_width = 25.f, wall_width = 5.f;
const int margin = 50;
int screen_width, screen_height;
bool is_playing = true;

std::vector<sf::RectangleShape> edges;
std::vector<std::vector<Block>> blocks;
std::vector<std::vector<std::vector<Wall>>> walls;

std::chrono::steady_clock::time_point begin;
std::chrono::steady_clock::time_point end;
float finish_time;


struct Block
{
	int x, y;
	bool visited = false;
	std::vector<Block*> neighbours;

	Block(int i, int j) : x(i), y(j) {}

	bool Neighbours()
	{
		neighbours.clear();

		GetNeigbour(x, y - 1, y); // top
		GetNeigbour(x + 1, y, x < cols - 1); // right
		GetNeigbour(x, y + 1, y < rows - 1); // bottom
		GetNeigbour(x - 1, y, x); // left

		return neighbours.size();
	}

private:
	void GetNeigbour(int i, int j, bool condition)
	{
		if (condition)
		{
			Block* neighbour = &blocks[i][j];
			if (!neighbour->visited) neighbours.push_back(neighbour);
		}
	}
};

struct Wall
{
	bool visible = true;
	sf::RectangleShape shape;

	Wall(float w, float h, float x, float y)
	{
		shape.setSize(sf::Vector2f(w, h));
		shape.setPosition(sf::Vector2f(x + margin, y + margin));
		shape.setFillColor(sf::Color::Black);
	}
};

// function declarations
void Input();
void WindowSize();
void InitVectors();
Wall* GetWall(int ax, int ay, int nx, int ny);
void Generate();
void Events(sf::RenderWindow&);
void Draw(sf::RenderWindow&, sf::Text);

struct Player
{
	int x = 0, y = 0, moves = 0;
	sf::RectangleShape shape; 

	Player()
	{
		shape.setSize(sf::Vector2f(block_width, block_width));
		shape.setPosition(sf::Vector2f(wall_width + margin, wall_width + margin));
		shape.setFillColor(sf::Color::Red);
	}
	
	void move(int mx, int my)
	{
		if (moves == 0)
		{
			begin = std::chrono::steady_clock::now();
		}

		Wall* w = GetWall(x, y, x + mx, y + my);

		if (!w->visible)
		{
			x += mx;
			y += my;
			++moves;

			shape.setPosition(sf::Vector2f(wall_width + (block_width + wall_width) * x + margin, wall_width + (block_width + wall_width) * y + margin));

			if (x == cols - 1 && y == rows - 1)
			{
				is_playing = false;

				end = std::chrono::steady_clock::now();
				finish_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() / 10 / 100.f;

				std::cout << "Finished in " << finish_time << " seconds!" << std::endl;
				std::cout << "Moves needed: " << moves << std::endl;
			}
		}
	}
}
p;

int main()
{
	srand(time(0));

	Input();
	WindowSize();
	InitVectors();
	Generate();

	sf::RenderWindow window(sf::VideoMode(screen_width + margin * 2, screen_height + margin * 2), "Maze Game!");
	window.setFramerateLimit(10);

	// font and text
	sf::Font font;
	if (!font.loadFromFile("C:\\Windows\\Fonts\\ARLRDBD.ttf")) std::cout << "No font loaded!";

	sf::Text text;
	text.setFont(font);
	text.setString("You won!");
	text.setCharacterSize(screen_height / 5);
	text.setFillColor(sf::Color::Red);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(5.f);

	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	text.setPosition(sf::Vector2f(screen_width / 2.f + margin, screen_height / 2.f + margin));


	while (window.isOpen())
	{
		Events(window);
		Draw(window, text);
	}
}


void Input()
{
	std::cout << "Number of columns: ";
	std::cin >> cols;
	std::cout << "Number of rows: ";
	std::cin >> rows;
}

void WindowSize()
{
	screen_width = cols * block_width + (cols + 1) * wall_width;
	screen_height = rows * block_width + (rows + 1) * wall_width;
}

// INIT FUNCTIONS
void InitBlocks()
{
	for (int i = 0; i < cols; ++i)
	{
		std::vector<Block> temp_col;

		for (int j = 0; j < rows; ++j)
		{
			temp_col.push_back(Block(i, j));
		}

		blocks.push_back(temp_col);
	}
}

void InitWalls()
{
	for (int n = 0; n < 2; ++n)
	{
		std::vector<std::vector<Wall>> temp_hr; // horizontal or vertical vector

		int n1 = n ? cols - 1 : rows - 1;
		for (int i = 0; i < n1; ++i)
		{
			std::vector<Wall> temp_cr; // column or row vector

			int n2 = n ? rows : cols;
			for (int j = 0; j < n2; ++j)
			{
				float w, h, x, y;

				if (n) // vertical wall
				{
					w = wall_width;
					h = block_width + wall_width * 2;
					x = block_width * (i + 1) + wall_width * i + wall_width;
					y = (block_width + wall_width) * j;
				}
				else // horizontal wall
				{
					w = block_width + wall_width * 2;
					h = wall_width;
					x = (block_width + wall_width) * j;
					y = block_width * (i + 1) + wall_width * i + wall_width;
				}

				temp_cr.push_back(Wall(w, h, x, y));
			}

			temp_hr.push_back(temp_cr);
		}

		walls.push_back(temp_hr);
	}
}

void Edge(float w, float h, float x, float y)
{
	sf::RectangleShape edge;

	edge.setSize(sf::Vector2f(w, h));
	edge.setPosition(sf::Vector2f(x + margin, y + margin));
	edge.setFillColor(sf::Color::Black);

	edges.push_back(edge);
}

void InitEdges()
{
	Edge(screen_width - block_width - wall_width, wall_width, block_width + wall_width, 0.f); // top
	Edge(wall_width, screen_height, screen_width - wall_width, 0.f); // right
	Edge(screen_width - block_width - wall_width, wall_width, 0.f, screen_height - wall_width); // bottom
	Edge(wall_width, screen_height, 0.f, 0.f); // left
}

void InitVectors()
{
	InitBlocks();
	InitWalls();
	InitEdges();
}


// GENERATE MAZE FUNCTIONS
Wall* GetWall(int ax, int ay, int nx, int ny)
{
	int n = std::abs(ax - nx);
	int i = n ? std::min(ax, nx) : std::min(ay, ny);
	int j = n ? ay : ax;

	return &walls[n][i][j];
}

void Generate()
{
	std::vector<Block*> stack;
	Block* active = &blocks[0][0];
	active->visited = true;
	int visited_count = 0;

	while (visited_count < cols * rows - 1)
	{
		if (active->Neighbours())
		{
			Block* next = active->neighbours[rand() % active->neighbours.size()];
			next->visited = true;

			GetWall(active->x, active->y, next->x, next->y)->visible = false;

			stack.push_back(active);
			active = next;

			++visited_count;
		}
		else
		{
			while (1)
			{
				active = stack[stack.size() - 1];

				if (active->Neighbours())
				{
					break;
				}
				else
				{
					stack.erase(stack.begin() + stack.size() - 1);
				}
			}
		}
	}
}


// EVENT FUNCTIONS
void Events(sf::RenderWindow& window)
{
	sf::Event e;
	while (window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			window.close();
	}

	// keypress events for movement
	if (is_playing)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && p.y > 0) p.move(0, -1);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && p.x < cols - 1) p.move(1, 0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && p.y < rows - 1) p.move(0, 1);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && p.x > 0) p.move(-1, 0);
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) window.close();
	}
}


// DRAW FUNCTIONS
void DrawEdges(sf::RenderWindow& window)
{
	for (int i = 0; i < edges.size(); ++i)
	{
		window.draw(edges[i]);
	}
}

void DrawWalls(sf::RenderWindow& window)
{
	for (int n = 0; n < 2; ++n)
	{
		int n1 = n ? cols - 1 : rows - 1;
		for (int i = 0; i < n1; ++i)
		{
			int n2 = n ? rows : cols;
			for (int j = 0; j < n2; ++j)
			{
				Wall* w = &walls[n][i][j];
				if (w->visible) window.draw(w->shape);
			}
		}
	}
}

void Draw(sf::RenderWindow& window, sf::Text text)
{
	window.clear(sf::Color::White);

	DrawEdges(window);
	DrawWalls(window);
	window.draw(p.shape);
	if(!is_playing) window.draw(text);
	
	window.display();
}