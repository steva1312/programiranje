#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <cmath>

sf::Vector2f rotate(sf::RectangleShape &shape, float a)
{
    float p, q, y, angle, x1, y1;
    angle = a * M_PI / 180;
    p = shape.getPosition().x + shape.getSize().x / 2;
    q = shape.getPosition().y;
    y = q + shape.getSize().y;

    x1 = p - (y - q) * sin(angle);
    y1 = q + (y - q) * cos(angle);

    return sf::Vector2f(x1, y1);
}

void branch(sf::Vector2f pos, float width, float height, float a, float angle, int amount_left, bool first, sf::RenderWindow &window)
{
    if (amount_left == 0) return;
    sf::RectangleShape shape(sf::Vector2f(width, height));
    shape.setOrigin(sf::Vector2f(width / 2, 0.f));
    shape.setPosition(pos);
    if (!first) shape.rotate(a);
    window.draw(shape);

    sf::Vector2f pos1;

    if (first)
        pos1 = pos;
    else
        pos1 = rotate(shape, a);

    // left
    branch(pos1, width, height * 0.67f, a - angle, angle, amount_left - 1, false, window);

    // right
    branch(pos1, width, height * 0.67f, a + angle, angle, amount_left - 1, false, window);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fractal Trees!");
    float degrees = 45.f;

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color::Black);

        float branch_width = 2.f, branch_height = 200.f;
        branch(sf::Vector2f(window.getSize().x / 2, window.getSize().y - branch_height), branch_width, branch_height, 180.f, degrees, 12, true, window);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) degrees += 1.f;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) degrees -= 1.f;

        window.display();
    }
}