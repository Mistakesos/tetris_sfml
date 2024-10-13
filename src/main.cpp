#include <array>
#include <vector>
#include <SFML/Graphics.hpp>
#include "global.hpp"
#include "matrix.hpp"
#include "tetriminos.hpp"

int main()
{
    /* Textures */
    sf::RenderWindow window(sf::VideoMode(640, 960), "Tetris");
    sf::Texture t1, t2, t3;
    t1.loadFromFile("../images/tiles.png");
    t2.loadFromFile("../images/background.png");
    t3.loadFromFile("../images/frame.png");

    t1.setSmooth(true);
    t2.setSmooth(true);
    t3.setSmooth(true);

    sf::Sprite tiles(t1), background(t2), frame(t3);
    sf::Sprite ghost_tiles(t1);
    tiles.setScale(2, 2);
    ghost_tiles.setScale(2, 2);
    background.setScale(2, 2);
    frame.setScale(2, 2);

    sf::Color transparency(255, 255, 255, 128);
    ghost_tiles.setColor(transparency);
    /* Textures */

    // Creat random engine
    std::random_device rd;
    std::mt19937 gen(rd());

    // Init timer
    sf::Clock clock;

    // Init game matrix and tetrimino
    Matrix matrix;
    Tetrimino tetrimino;
    matrix.m_matrix = {ROWS, std::vector<int>(COLS, 0)};
    std::array<Point, 4> current, previous;

    // Generate 7-bag
    std::array<Shapes, 7> bag = tetrimino.generate_7bag();
    int bagIndex = 0;

    // Offset of X
    int dx = 0;

    // Variables related to rotation
    bool isRotateRight = false;
    bool isRotateLeft = false;
    bool isLock = false;
    int rotationState = 0;

    // Timer and delay
    float timer = 0, delay = 0.3f;
    float lock_delay = 0;
    int lock_times = 15;
    bool hard_drop = false;


    // Init block, is important. If we don't do this, there will be problems with the first block
    Shapes tetriminoShape = matrix.generate_shapes(matrix, tetrimino, current, bag, bagIndex);
    // Init color
    Colors tetriminoColor = matrix.judge_color(tetriminoShape);

    // Game main loop
    while(window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();

        timer += time;


        /* Input */
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed) window.close();
            if(event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Up: isRotateRight = true; break;
                    case sf::Keyboard::Z:  isRotateLeft = true;  break;
                    case sf::Keyboard::Left:    dx = -1;         break;
                    case sf::Keyboard::Right:   dx = 1;          break;
                    case sf::Keyboard::Space: hard_drop = true;  break;
                }
            }
        }

        // Soft drop
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay = 0.05;

        // Hard dorp
        if(hard_drop)
        {
            current = tetrimino.get_ghost_tetrimino(matrix, current);
            matrix.lock_to_matrix(current, matrix, tetriminoColor);
            matrix.generate_tetrimino(matrix, tetrimino, current, tetriminoShape, tetriminoColor, rotationState, bag, bagIndex);
            isLock = true;
        }


        /* Move */
        tetrimino.move_tetrimino(current, previous, matrix, dx);


        /* Rotate */
        if(isRotateRight || isRotateLeft)
        {
            tetrimino.rotate(matrix, tetriminoShape, current, previous, rotationState, isRotateRight);
            lock_times++;
        }


        /* Drop down tetrimino then generate */
        if(timer > delay)
        {
            // matrix.drop_and_generate(current, previous, matrix, tetrimino, bag, bagIndex, tetriminoShape, tetriminoColor, rotationState, timer);
            if(!matrix.can_drop_down(current, previous, matrix))
            {
                matrix.lock_to_matrix(previous, matrix, tetriminoColor);
                matrix.generate_tetrimino(matrix, tetrimino, current, tetriminoShape, tetriminoColor, rotationState, bag, bagIndex);
            }
            timer = 0;
        }


        /* Check for full rows and clear them */
        matrix.clear_lines(matrix);


        // Reset them, prepare for next loop
        dx = 0;
        isRotateRight = false;
        isRotateLeft = false;
        hard_drop = false;
        delay = 0.3f;


        /* Draw */
        window.clear(sf::Color::White);
        window.draw(background);

        for(int i = 0; i < ROWS; i++)
        {
            for(int j = 0; j < COLS; j++)
            {
                if(matrix.m_matrix[i][j] == 0) continue;
                tiles.setTextureRect(sf::IntRect(matrix.m_matrix[i][j] * 18, 0, 18, 18));
                tiles.setPosition(j * 36, (i - 20) * 36);
                tiles.move(56, 62);
                window.draw(tiles);
            }
        }

        for(int i = 0; i < 4; i++)
        {
            // We need that again...
            int colorIndex = static_cast<int>(tetriminoColor);
            tiles.setTextureRect(sf::IntRect(colorIndex * 18, 0, 18, 18));
            tiles.setPosition(current[i].x * 36, (current[i].y - 20) * 36);
            tiles.move(56, 62);
            window.draw(tiles);
        }

        std::array<Point, 4> ghost = tetrimino.get_ghost_tetrimino(matrix, current);
        for(auto& point : ghost)
        {
            // And again...
            int colorIndex = static_cast<int>(tetriminoColor);
            ghost_tiles.setTextureRect(sf::IntRect(colorIndex * 18, 0, 18, 18));
            ghost_tiles.setPosition(point.x * 36, (point.y - 20) * 36);
            ghost_tiles.move(56, 62);
            window.draw(ghost_tiles);
        }

        window.draw(frame);
        window.display();

    }

    return 0;
}