#include <random>
#include <array>
#include <SFML/Graphics.hpp>

constexpr int WIDTH = 10;
constexpr int HEIGHT = 20;

std::array<std::array<int, WIDTH>,HEIGHT> gameField = {0};

struct Point{int x, y;};
std::array<Point, 4> current, previous;

std::array<std::array<int, 4>, 7> figures =
{
    1, 3, 5, 7, // I
    2, 4, 5, 7, // Z
    3, 5, 4, 6, // S
    3, 5, 4, 7, // T
    2, 3, 5, 7, // L
    3, 5, 7, 6, // J
    2, 3, 4, 5, // O
};

bool check()
{
    for(int i = 0; i < 4; i++)
    {
        if(current[i].x < 0 || current[i].x >= WIDTH || current[i].y >= HEIGHT) return false;
        else if(gameField[current[i].y][current[i].x]) return false;
    }
    return true;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(320, 480), "Tetris");
    sf::Texture t1, t2, t3;

    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/background.png");
    t3.loadFromFile("images/frame.png");
    sf::Sprite tiles(t1), background(t2), frame(t3);

    std::random_device rd;
    std::mt19937 gen(rd());

    sf::Clock clock;

    std::uniform_int_distribution<int> genTetromino(0, 6);
    std::uniform_int_distribution<int> genColor(1, 7);
    int colorIndex = genColor(gen);
    
    int dx = 0;
    bool rotate = false;
    float timer = 0, delay = 0.3;
    
    while(window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();

        timer += time;

        /* trick */
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) window.close();
            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::Up) rotate = true;
                else if(event.key.code == sf::Keyboard::Left) dx = -1;
                else if(event.key.code == sf::Keyboard::Right) dx = 1;
            }
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay = 0.05;


        /* move */
        for(int i = 0; i < 4; i++)
        {
            previous[i] = current[i];
            current[i].x += dx;
        }
        if(!check())
        {
            for(int i = 0; i < 4; i++)
            {
                current[i] = previous[i];
            }
        }
        

        /* rotate */
        if(rotate)
        {
            Point center = current[1];
            for(int i = 0; i < 4; i++)
            {
                int x = current[i].y - center.y;
                int y = current[i].x - center.x;
                current[i].x = center.x - x;
                current[i].y = center.y + y;
            }
            if(!check())
            {
                for(int i = 0; i < 4; i++)
                {
                    current[i] = previous[i];
                }
            }
        }
        

        /* generate */
        if(timer > delay)
        {
            for(int i = 0; i < 4; i++)
            {
                previous[i] = current[i];
                current[i].y += 1;
            }

            if(!check())
            {
                for(int i = 0; i < 4; i++)
                {
                    gameField[previous[i].y][previous[i].x] = colorIndex;
                }
                colorIndex = genColor(gen);
                int tetrominoIndex = genTetromino(gen);
                for(int i = 0; i < 4; i++)
                {
                    current[i].x = figures[tetrominoIndex][i] % 2 + 4;
                    current[i].y = figures[tetrominoIndex][i] / 2;
                }
            }

            timer = 0;
        }


        /* check lines */
        int preLine = HEIGHT - 1;
        for(int i = HEIGHT - 1; i > 0; i--)
        {
            int count = 0;
            for(int j = 0; j < WIDTH; j++)
            {
                if(gameField[i][j]) count++;
                gameField[preLine][j] = gameField[i][j];
            }
            if(count < WIDTH) preLine--;
        }
        

        dx = 0;
        rotate = false;
        delay = 0.3;


        /* draw */
        window.clear(sf::Color::White);
        window.draw(background);

        for(int i = 0; i < HEIGHT; i++)
        {
            for(int j = 0; j < WIDTH; j++)
            {
                if(gameField[i][j] == 0) continue;
                tiles.setTextureRect(sf::IntRect(gameField[i][j] * 18, 0, 18, 18));
                tiles.setPosition(j * 18, i * 18);
                tiles.move(28, 31);
                window.draw(tiles);
            }
        }
        
        for(int i = 0; i < 4; i++)
        {
            tiles.setTextureRect(sf::IntRect(colorIndex * 18, 0, 18, 18));
            tiles.setPosition(current[i].x * 18, current[i].y * 18);
            tiles.move(28, 31);
            window.draw(tiles);
        }

        
        window.draw(frame);
        window.display();

    }

    return 0;
}