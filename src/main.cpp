#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "global.hpp"
#include "matrix.hpp"
#include "tetriminos.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 960), "Tetris");
    sf::Texture t1, t2, t3;
    t1.loadFromFile("../images/tiles.png");
    t2.loadFromFile("../images/background.png");
    t3.loadFromFile("../images/frame.png");

    t1.setSmooth(true);
    t2.setSmooth(true);
    t3.setSmooth(true);

    sf::Sprite tiles(t1), background(t2), frame(t3);
    tiles.setScale(2, 2);
    background.setScale(2, 2);
    frame.setScale(2, 2);

    std::random_device rd;
    std::mt19937 gen(rd());

    sf::Clock clock;

    // std::uniform_int_distribution<int> genTetromino(0, 6);   // real random Tetromino generater
    // std::uniform_int_distribution<int> genColor(1, 7);   // real random color generater
    // int colorIndex = genColor(gen);  // Init real random generater

    Matrix matrix;
    Tetrimino tetrimino;
    // matrix.m_matrix = {ROWS, std::vector<int>(COLS, 0)};
    matrix.m_matrix = {40, std::vector<int>(20, 0)};
    std::array<Point, 4> current, previous; 

    std::array<Shapes, 7> bag = tetrimino.gen7Bag();
    int bagIndex = 0;
    int dx = 0;
    bool isRotateRight = false;
    bool isRotateLeft = false;
    float timer = 0, delay = 100.f;
    int rotationState = 0;

    // Init block, is important. If we don't do this, there will be problems with the first block
    Shapes tetriminoShape = matrix.generateTetromino(matrix, tetrimino, current, bag, bagIndex);
    // Init color
    Colors tetriminoColor = matrix.judgeColor(tetriminoShape);


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
                if(event.key.code == sf::Keyboard::Up) isRotateRight = true;
                else if(event.key.code == sf::Keyboard::Z) isRotateLeft = true;
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
        if(!matrix.check(current, matrix))
        {
            for(int i = 0; i < 4; i++)
            {
                current[i] = previous[i];
            }
        }


        /* rotate */
        if(isRotateRight || isRotateLeft)
        {
            tetrimino.rotate(matrix, tetriminoShape, current, previous, rotationState, isRotateRight);
        }


        /* generate */
        if(timer > delay)
        {
            for(int i = 0; i < 4; i++)
            {
                previous[i] = current[i];
                current[i].y += 1;
            }

            if(!matrix.check(current, matrix))
            {
                for(int i = 0; i < 4; i++)
                {
                    // Sadly, we need the enum value again, but I still wanna use enum class, not classic enum|_|
                    int colorIndex = static_cast<int>(tetriminoColor);
                    matrix.m_matrix[previous[i].y][previous[i].x] = colorIndex;
                }

                // int tetrominoIndex = genTetromino(gen);  // Real random Tetromino
                tetriminoShape = matrix.generateTetromino(matrix, tetrimino, current, bag, bagIndex);

                // reset Rotation state
                rotationState = 0;

                // colorIndex = genColor(gen);  // Real random color
                tetriminoColor = matrix.judgeColor(tetriminoShape);
            }

            timer = 0;
        }


        /* check lines */
        int preLine = ROWS - 1;
        for(int i = ROWS - 1; i > 0; i--)
        {
            int count = 0;
            for(int j = 0; j < COLS; j++)
            {
                if(matrix.m_matrix[i][j]) count++;
                matrix.m_matrix[preLine][j] = matrix.m_matrix[i][j];
            }
            if(count < COLS) preLine--;
        }


        dx = 0;
        isRotateRight = false;
        isRotateLeft = false;
        delay = 100.f;


        /* draw */
        window.clear(sf::Color::White);
        window.draw(background);

        for(int i = 0; i < ROWS; i++)
        {
            for(int j = 0; j < COLS; j++)
            {
                if(matrix.m_matrix[i][j] == 0) continue;
                tiles.setTextureRect(sf::IntRect(matrix.m_matrix[i][j] * 18, 0, 18, 18));
                tiles.setPosition(j * 36, i * 36);
                tiles.move(56, 62);
                window.draw(tiles);
            }
        }

        for(int i = 0; i < 4; i++)
        {
            // We need that again...
            int colorIndex = static_cast<int>(tetriminoColor);
            tiles.setTextureRect(sf::IntRect(colorIndex * 18, 0, 18, 18));
            tiles.setPosition(current[i].x * 36, current[i].y * 36);
            tiles.move(56, 62);
            window.draw(tiles);
        }


        window.draw(frame);
        window.display();

    }

    return 0;
}