#include <SFML/Graphics.hpp>

const int M = 20;
const int N = 10;

int field[M][N] = { 0 };

struct Point
{int x, y;} a[4], b[4];

int figures[7][4] =
{
    1,3,5,7, // I
    2,4,5,7, // Z
    3,5,4,6, // S
    3,5,4,7, // T
    2,3,5,7, // L
    3,5,7,6, // J
    2,3,4,5, // O
};

int main()
{
    auto window = sf::RenderWindow{ { 320u, 480u }, "Cuties Tetris" };
    window.setFramerateLimit(144);

    sf::Texture tiles;
    tiles.loadFromFile("assets/graphics/tiles.png");

    sf::Sprite tileSprite(tiles);
    tileSprite.setTextureRect(sf::IntRect(0, 0, 18, 18));

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    int dx = 0;
    bool rotate = 0;
    int colorNum = 1;

    while (window.isOpen())
    {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up) { rotate = true; }
                else if (event.key.code == sf::Keyboard::Left) { dx = -1; }
                else if (event.key.code == sf::Keyboard::Right) { dx = 1; }
            }
        }

        for (int i = 0; i < 4; i++)
        {
            a[i].x += dx;
        }

        if (rotate)
        {
            Point ShapeCenter = a[1];
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - ShapeCenter.y;
                int y = a[i].x - ShapeCenter.x;
                a[i].x = ShapeCenter.x - x;
                a[i].y = ShapeCenter.y - y;
            }
        }

        int n = 3;
        if (a[0].x == 0) {
            for (int i = 0; i < 4; i++)
            {
                a[i].x = figures[n][i] % 2;
                a[i].y = figures[n][i] / 2;
            }
        }

        dx = 0;
        rotate = 0;

        window.clear(sf::Color::White);
        for (int i = 0; i < 4; i++)
        {
            tileSprite.setPosition(a[i].x * 18, a[i].y * 18);
            window.draw(tileSprite);
        }
        window.display();
    }
}
