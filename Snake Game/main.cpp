#include <SFML/Graphics.hpp>
#include <deque>
#include <cstdlib>

const int CELL = 20;
const int W = 600;
const int H = 600;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ W, H }), "Snake");
    window.setFramerateLimit(10);

    std::deque<sf::Vector2i> snake = { {5,5}, {4,5}, {3,5} };
    sf::Vector2i dir = { 1, 0 };
    sf::Vector2i food = { 10, 10 };
    bool alive = true;

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (alive)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && dir.y == 0) {
                dir = { 0,-1 };
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && dir.y == 0) {
                dir = { 0, 1 };
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && dir.x == 0) {
                dir = { -1, 0 };
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && dir.x == 0) {
                dir = { 1, 0 };
            }

            sf::Vector2i newHead = snake.front() + dir;

            // wall collision
            if (newHead.x < 0 || newHead.x >= W / CELL ||
                newHead.y < 0 || newHead.y >= H / CELL)
                alive = false;

            // self collision
            for (auto& s : snake)
                if (s == newHead) alive = false;

            if (alive)
            {
                snake.push_front(newHead);
                if (newHead == food)
                {
                    food = { rand() % (W / CELL), rand() % (H / CELL) };
                }
                else
                {
                    snake.pop_back();
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        // draw snake
        sf::RectangleShape cell(sf::Vector2f(CELL - 2, CELL - 2));
        for (auto& s : snake)
        {
            cell.setFillColor(sf::Color::Green);
            cell.setPosition(sf::Vector2f(s.x * CELL, s.y * CELL));
            window.draw(cell);
        }

        // draw food
        cell.setFillColor(sf::Color::Red);
        cell.setPosition(sf::Vector2f(food.x * CELL, food.y * CELL));
        window.draw(cell);

        if (!alive)         // flash screen red on death
        {
            window.clear(sf::Color(100, 0, 0));
        }

        window.display();
    }
}