#include <iostream>
#include <vector>
#include <random>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Block
{
public:
	bool is_hit = false;

private:
	const int block_width = 50, block_height = 25, margin = 3;
	sf::RectangleShape shape;

public:
	Block(int x, int y)
	{
		shape.setSize(sf::Vector2f((float)block_width, (float)block_height));
		shape.setFillColor(getColor(y));
		
		float pos_x, pos_y;
		pos_x = (float)(block_width * x + margin * (x + 1));
		pos_y = (float)(block_height * y + margin * (y + 1));
		shape.setPosition(sf::Vector2f(pos_x, pos_y));
	}

	void draw(sf::RenderWindow& window)
	{
		if (!is_hit) window.draw(shape);
	}

	float top() { return shape.getPosition().y; }
	float bottom() { return shape.getPosition().y + float(block_height); }
	float left() { return shape.getPosition().x; }
	float right() { return shape.getPosition().x + float(block_width); }

private:
	sf::Color getColor(int c)
	{
		sf::Color color;
		switch (c)
		{
			case 0: color = sf::Color::Red; break;
			case 1: color = sf::Color::Magenta; break;
			case 2: color = sf::Color::Blue; break;
			case 3: color = sf::Color::Cyan; break;
			case 4: color = sf::Color::Green; break;
			case 5: color = sf::Color::Yellow; break;
		}
		return color;
	}
};

// global variables
const int rows = 6, cols = 12;
const int win_width = 639, win_height = 480;
int score = 0;
Block* blocks[rows * cols];
bool gameover = false;
std::vector<sf::Text> texts;
sf::Font font;
sf::SoundBuffer block_hitBuffer;
sf::SoundBuffer player_hitBuffer;
sf::SoundBuffer loseBuffer;
sf::SoundBuffer winBuffer;
sf::Sound block_hitSound;
sf::Sound player_hitSound;
sf::Sound loseSound;
sf::Sound winSound;

class Player
{
public:
	sf::RectangleShape shape;
	const int width = 80, height = 25;
	int dx = 0;

private:
	const float vel = 1.2;

public:
	Player()
	{
		shape.setSize(sf::Vector2f(width, height));
		shape.setFillColor(sf::Color::Red);
	}

	void move(int dirx)
	{
		dx = dirx;
		if (dx == -1 && shape.getPosition().x > 0 || dx == 1 && shape.getPosition().x < win_width - width)
		{
			shape.setPosition(sf::Vector2f(shape.getPosition().x + dx * vel, shape.getPosition().y));
		}
	}

	float top() { return shape.getPosition().y; }
	float left() { return shape.getPosition().x; }
	float right() { return shape.getPosition().x + float(width); }

} player;

class Ball
{
public:
	sf::RectangleShape shape;
	int dx, dy;

private:
	const int width = 20;

public:
	Ball()
	{
		shape.setSize(sf::Vector2f(width, width));
		shape.setOrigin(sf::Vector2f((float)(width/2), (float)(width/2)));
		shape.setFillColor(sf::Color::White);
	}

	void move()
	{

		// check walls
		if (top() == 0)
			dy = 1;
		else if (bottom() == win_height)
		{
			gameover = true;
			loseSound.play();
		}
		if (left() == 0)
			dx = 1;
		else if (right() == win_width)
			dx = -1;

		// check blocks
		for (Block* block : blocks)
		{
			if (!block->is_hit)
			{
				if (top() == block->bottom() && left() >= block->left() - width && right() <= block->right() + width)
				{
					dy = 1;
					block->is_hit = true;
				}
				else if (bottom() == block->top() && left() >= block->left() - width && right() <= block->right() + width)
				{
					dy = -1;
					block->is_hit = true;
				}
				else if (left() == block->right() && top() >= block->top() - width && bottom() <= block->bottom() + width)
				{
					dx = 1;
					block->is_hit = true;
				}
				else if (right() == block->left() && top() >= block->top() - width && bottom() <= block->bottom() + width)
				{
					dx = -1;
					block->is_hit = true;
				}

				if (block->is_hit)
				{
					score++;
					block_hitSound.play();
				}

				if (score == rows * cols)
				{
					gameover = true;
					winSound.play();
				}
			}
		}

		// check player
		if (bottom() == player.top() && left() >= player.left() - width && right() <= player.right() + width)
		{
			dy = -1;
			if (player.dx != ball.dx && player.dx != 0)
			{
				dx *= -1;
			}
			player_hitSound.play();
		}

		shape.setPosition(sf::Vector2f(shape.getPosition().x + dx, shape.getPosition().y + dy));
	}

private:
	float top() { return shape.getPosition().y - float(width / 2); }
	float bottom() { return shape.getPosition().y + float(width / 2); }
	float left() { return shape.getPosition().x - float(width / 2); }
	float right() { return shape.getPosition().x + float(width / 2); }
} ball;

// function declarations
void InitBlocks();
void PrepareGame();
void Draw(sf::RenderWindow&);
void Events(sf::RenderWindow&);
sf::Text MakeText(std::string, int, int);

int main()
{
	InitBlocks();
	PrepareGame();

	if(!font.loadFromFile("C:\\Windows\\Fonts\\ARLRDBD.ttf")) std::cout << "failed to load the font";
	texts.push_back(MakeText("Game over!", 75, win_height / 5 * 2));
	texts.push_back(MakeText("Press enter to play again...", 25, win_height / 5 * 3));

	block_hitBuffer.loadFromFile("C:\\CppProjects\\SFML_PROJECTS\\CIgliceTest\\cigla.wav");
	player_hitBuffer.loadFromFile("C:\\CppProjects\\SFML_PROJECTS\\CIgliceTest\\igrac.wav");
	loseBuffer.loadFromFile("C:\\CppProjects\\SFML_PROJECTS\\CIgliceTest\\gubitak.wav");
	winBuffer.loadFromFile("C:\\CppProjects\\SFML_PROJECTS\\CIgliceTest\\pobeda.wav");
	block_hitSound.setBuffer(block_hitBuffer);
	player_hitSound.setBuffer(player_hitBuffer);
	loseSound.setBuffer(loseBuffer);
	winSound.setBuffer(winBuffer);

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "SFML TEMPLATE!");
	window.setFramerateLimit(300);

	while (window.isOpen())
	{
		Events(window);
		Draw(window);
	}
}

void PrepareGame()
{
	gameover = false;
	score = 0;

	for (Block* block : blocks)
	{
		block->is_hit = false;
	}

	player.shape.setPosition(sf::Vector2f((win_width - player.width) / 2, win_height / 6 * 5));

	ball.shape.setPosition(sf::Vector2f((float)(win_width / 2), (float)(win_height / 4 * 3)));
	srand(time(0));
	ball.dx = rand() % 2 ? 1 : -1;
	ball.dy = -1;
}

void InitBlocks()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			blocks[i * cols + j] = new Block(j, i);
		}
	}
}

void Draw(sf::RenderWindow& window)
{
	window.clear(sf::Color::Black);

	// draw blocks
	for (Block* block : blocks)
	{
		block->draw(window);
	}

	// draw ball
	window.draw(ball.shape);

	// draw player
	window.draw(player.shape);

	// draw game over texts
	if (gameover)
	{
		for (sf::Text text : texts)
		{
			window.draw(text);
		}
	}

	window.display();
}

void Events(sf::RenderWindow& window)
{
	sf::Event ev;
	while (window.pollEvent(ev))
	{
		if (ev.type == sf::Event::Closed) window.close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return) && gameover)
	{
		PrepareGame();
	}

	if (gameover) return;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) player.move(-1);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) player.move(1);
	else player.move(0);

	ball.move();
}

sf::Text MakeText(std::string s, int size, int y)
{
	sf::Text text;
	text.setFont(font);
	text.setString(s);
	text.setCharacterSize(size);
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f((win_width - text.getGlobalBounds().width) / 2, (float)y));

	return text;
}