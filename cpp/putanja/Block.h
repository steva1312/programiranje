#pragma once
#include <SFML/Graphics.hpp>

class Block
{
public:
	int x, y;
	bool wall = false;
	int g, h, f;
	Block* parent;
	sf::RectangleShape shape;

public:
	Block(int i, int j, float width, float wall) : x(i), y(j)
	{
		shape.setSize(sf::Vector2f(width, width));
		shape.setPosition(sf::Vector2f((width + wall) * x, (width + wall) * y));
		SetColor();
	}

	void ChangeState()
	{
		wall = !wall;
		SetColor();
	}

	void F(Block* A, Block* B)
	{
		g = std::abs(A->x - x) + std::abs(A->y - y);
		h = std::abs(B->x - x) + std::abs(B->y - y);
		f = g + h;
	}

private:
	void SetColor()
	{
		if (wall)
		{
			shape.setFillColor(sf::Color(60, 60, 60));
		}
		else
		{
			shape.setFillColor(sf::Color::Blue);
		}
	}
};