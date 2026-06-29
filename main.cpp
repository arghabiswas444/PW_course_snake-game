#include <SFML/Graphics.hpp>
#include <deque>
#include <cstdlib>
#include <ctime>

const int CELL = 20;
const int W = 600;
const int H = 600;

int main()
{
    srand(time(nullptr));

    sf::RenderWindow window(sf::VideoMode({ W, H }), "Snake Game");

    int speed = 10;
    window.setFramerateLimit(speed);

    std::deque<sf::Vector2i> snake = { {5,5}, {4,5}, {3,5} };
    sf::Vector2i dir = { 1, 0 };
    sf::Vector2i food = { 10, 10 };
    bool alive = true;

    int score = 0;
    int bigFood = 0; // IMPORTANT: restore logic

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        if (alive)
        {
            bool dirChanged = false;

            if (!dirChanged && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) && dir.y == 0)
            {
                dir = { 0, -1 };
                dirChanged = true;
            }

            if (!dirChanged && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && dir.y == 0)
            {
                dir = { 0, 1 };
                dirChanged = true;
            }

            if (!dirChanged && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && dir.x == 0)
            {
                dir = { -1, 0 };
                dirChanged = true;
            }

            if (!dirChanged && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && dir.x == 0)
            {
                dir = { 1, 0 };
                dirChanged = true;
            }

            sf::Vector2i newHead = snake.front() + dir;

            if (newHead.x < 0 || newHead.x >= W / CELL ||
                newHead.y < 0 || newHead.y >= H / CELL)
            {
                alive = false;
            }

            for (int i = 0; i < (int)snake.size(); i++)
            {
                if (snake[i] == newHead)
                {
                    alive = false;
                }
            }

            if (alive)
            {
                snake.push_front(newHead);

                // FOOD EATING LOGIC (FIXED)
                if (newHead == food)
                {
                    bigFood++;

                    food = { rand() % (W / CELL), rand() % (H / CELL) };

                    if (bigFood >= 6)
                    {
                        score += 5;
                        bigFood = 0;
                    }
                    else
                    {
                        score += 1;
                    }

                    // speed scaling (from Step 5)
                    if (speed < 30)
                    {
                        speed++;
                        window.setFramerateLimit(speed);
                    }
                }
                else
                {
                    snake.pop_back();
                }
            }
        }

        window.clear(sf::Color(25, 25, 25));

        sf::RectangleShape cell(sf::Vector2f(CELL - 2, CELL - 2));

        // draw snake
        for (int i = 0; i < (int)snake.size(); i++)
        {
            cell.setFillColor(sf::Color::Green);
            cell.setPosition(sf::Vector2f(snake[i].x * CELL, snake[i].y * CELL));
            window.draw(cell);
        }

        // food color logic (NOW WORKS)
        if (bigFood >= 5)
        {
            cell.setFillColor(sf::Color::Yellow);
        }
        else
        {
            cell.setFillColor(sf::Color::Red);
        }

        cell.setPosition(sf::Vector2f(food.x * CELL, food.y * CELL));
        window.draw(cell);

        if (!alive)
        {
            window.clear(sf::Color(100, 0, 0));
        }

        window.display();
    }
}