#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

// global variables
int board[9][9];
int block_width = 50, wall1_width = 3, wall2_width = 10, margin = 50;
int win_width = block_width * 9 + wall1_width * 6 + wall2_width * 4 + margin * 2;
int player_pos[2] = { 0, 0 };
std::vector<sf::RectangleShape> walls;
sf::Text message;

// function declarations
int* FindEmpty();
bool Valid(int, int, int);
bool ValidBoard();
bool Solve();
void InitWalls();
void Events(sf::RenderWindow&);
void Draw(sf::RenderWindow&, sf::Font&);

int main()
{
	InitWalls();

	sf::RenderWindow window(sf::VideoMode(win_width, win_width + 100), "Sudoku Solver!");
	window.setFramerateLimit(5);

	sf::Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/ARLRDBD.ttf")) std::cout << "Failed to load the font.";

	while (window.isOpen())
	{
		Events(window);
		Draw(window, font);
	}
}

int* FindEmpty()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] == 0)
			{
				int pos[2] = { i, j };
				return pos;
			}
		}
	}

	return nullptr;
}

bool Valid(int row, int col, int num)
{	
	// check row
	for (int i = 0; i < 9; i++)
	{
		if (i == col) continue;
		if (board[row][i] == num) return false;
	}

	// check column
	for (int i = 0; i < 9; i++)
	{
		if (i == row) continue;
		if (board[i][col] == num) return false;
	}

	// check square
	int s_row = row / 3;
	int s_col = col / 3;
	for (int i = s_row * 3; i < s_row * 3 + 3; i++)
	{
		for (int j = s_col * 3; j < s_col * 3 + 3; j++)
		{
			if (i == row && j == col) continue;
			if (board[i][j] == num) return false;
		}
	}

	return true;
}

bool ValidBoard()
{
	// check for mistakes
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] != 0)
			{
				if (!Valid(i, j, board[i][j]))
				{
					message.setString("Ivalid Board.");
					return false;
				}
			}
		}
	}

	// check if already filled
	if (FindEmpty() == nullptr)
	{
		message.setString("The board is already solved.");
		return false;
	}

	return true;
}

bool Solve()
{
	int* pos = FindEmpty();
	int row, col;

	if (pos == nullptr)
	{
		return true;
	}
	else
	{
		row = pos[0];
		col = pos[1];
	}

	for (int i = 1; i < 10; i++)
	{
		if (Valid(row, col, i))
		{
			board[row][col] = i;

			if (Solve())
			{
				return true;
			}

			board[row][col] = 0;
		}
	}

	return false;
}

void InitWalls()
{
	for (int i = 0; i < 20; i++)
	{
		sf::RectangleShape shape;
		shape.setFillColor(sf::Color::Black);

		float width, height, x  = margin, y = margin;

		if (i < 10) // vertical wall
		{
			if (i % 3 == 0) // big wall
			{
				width = wall2_width;
				height = wall2_width * 4 + wall1_width * 6 + block_width * 9;
				x += i / 3 * (wall1_width * 2 + block_width * 3 + wall2_width);
			}
			else // small wall
			{
				width = wall1_width;
				height = wall1_width * 6 + wall2_width * 2 + block_width * 9;
				x += wall2_width + block_width * i + wall1_width  * (i - 1 - i / 3) + wall2_width * (i / 3);
				y += wall2_width;
			}
		}
		else // horizontal wall
		{
			if (i % 3 == 1) // big wall
			{
				width = wall2_width * 4 + wall1_width * 6 + block_width * 9;
				height = wall2_width;
				y += (i - 10) / 3 * (wall1_width * 2 + block_width * 3 + wall2_width);
			}
			else // small wall
			{
				width = wall1_width * 6 + wall2_width * 2 + block_width * 9;
				height = wall1_width;
				x += wall2_width;
				y += wall2_width + block_width * (i - 10) + wall1_width * (i - 11 - (i - 10) / 3) + wall2_width * ((i - 10) / 3);
			}
		}

		shape.setSize(sf::Vector2f(width, height));
		shape.setPosition(sf::Vector2f(x, y));
		walls.push_back(shape);
	}
}

void Events(sf::RenderWindow& window)
{
	sf::Event e;
	while (window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed) window.close();
		if (e.type == sf::Event::KeyPressed) message.setString("");
		if (e.type == sf::Event::KeyPressed && e.key.code >= 26 && e.key.code <= 35)
			board[player_pos[1]][player_pos[0]] = e.key.code - 26;
	}

	// additional erase key
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
	{
		board[player_pos[1]][player_pos[0]] = 0;
	}

	// player movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player_pos[1] > 0)
	{
		player_pos[1]--;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player_pos[0] < 8)
	{
		player_pos[0]++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player_pos[1] < 8)
	{
		player_pos[1]++;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player_pos[0] > 0)
	{
		player_pos[0]--;
	}

	// solve key
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		if (ValidBoard())
		{
			if (Solve())
			{
				message.setString("Sudoku solved!");
			}
			else
			{
				message.setString("There is no solution.");
			}
		}
	}

	// reset key
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				board[i][j] = 0;
			}
		}
	}
}

void Draw(sf::RenderWindow& window, sf::Font& font)
{
	window.clear(sf::Color::White);

	// draw walls
	for (int i = 0; i < walls.size(); i++)
	{
		window.draw(walls[i]);
	}

	// draw player
	sf::RectangleShape player;
	player.setSize(sf::Vector2f(block_width, block_width));
	player.setFillColor(sf::Color(192, 192, 192));
	int px = player_pos[0], py = player_pos[1], x, y;
	x = block_width * px + wall1_width * (px - px / 3) + wall2_width * (px / 3) + wall2_width + margin;
	y = block_width * py + wall1_width * (py - py / 3) + wall2_width * (py / 3) + wall2_width + margin;
	player.setPosition(sf::Vector2f(x, y));
	window.draw(player);

	// draw numbers
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (board[i][j] != 0)
			{
				sf::Text text;
				text.setFont(font);
				text.setFillColor(sf::Color::Black);
				text.setCharacterSize(block_width / 5 * 3);
				text.setString(std::to_string(board[i][j]));

				float x = margin + wall2_width, y = margin + wall2_width;
				x += block_width * j + wall1_width * (j - j / 3) + wall2_width * (j / 3) + (block_width - text.getLocalBounds().width) / 2;
				y += block_width * i + wall1_width * (i - i / 3) + wall2_width * (i / 3) + (block_width - text.getCharacterSize()) / 2;
				text.setPosition(sf::Vector2f(x, y));

				window.draw(text);
			}
		}
	}

	// draw message
	message.setFont(font);
	message.setCharacterSize(36);
	if (message.getString() == "Sudoku solved!") message.setFillColor(sf::Color::Blue);
	else message.setFillColor(sf::Color::Red);
	float mx = (win_width - message.getLocalBounds().width) / 2;
	float my = win_width - margin + (100 + margin - message.getCharacterSize()) / 2;
	message.setPosition(sf::Vector2f(mx, my));
	window.draw(message);

	window.display();
}