#include <array>
#include <vector>
#include <ranges>
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
    sf::Sprite holdTiles(t1);
    tiles.setScale(2, 2);
    ghost_tiles.setScale(2, 2);
    holdTiles.setScale(2, 2);
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
    sf::Clock lockClock;

    // Init game matrix and tetrimino
    Matrix matrix;
    Tetrimino tetrimino;
    matrix.m_matrix = {ROWS, std::vector<int>(COLS, 0)};
    std::array<Point, 4> current, previous, hold = {0};

    // Generate 7-bag
    std::array<Shapes, 7> bag = tetrimino.generate_7bag();
    int bagIndex = 0;

    // Offset of X
    int dx = 0;

    // Variables related to rotation
    bool isRotateRight = false;
    bool isRotateLeft = false;
    int rotationState = 0;

    // Harrrd drop
    bool hardDrop = false;

    // Hold
    bool isHold = false;
    bool hasHold = false;
    static Shapes holdShape;
    static Colors holdColor;


    // Timer, delay and actions
    float timer = 0, delay = 0.3f;
    float lockTimer = 0, lockDelay = 0.5f;

    int lockDelayAction = 0;


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
                    case sf::Keyboard::Up: isRotateRight = true; if(isRotateRight || isRotateLeft){tetrimino.rotate(matrix, tetriminoShape, current, previous, rotationState, isRotateRight, lockDelayAction, lockClock, lockTimer);} break;
                    case sf::Keyboard::Z:  isRotateLeft = true;  if(isRotateRight || isRotateLeft){tetrimino.rotate(matrix, tetriminoShape, current, previous, rotationState, isRotateRight, lockDelayAction, lockClock, lockTimer);} break;
                    case sf::Keyboard::Left:    dx = -1;         tetrimino.move_tetrimino(current, previous, matrix, dx, lockDelayAction, lockClock, lockTimer); break;
                    case sf::Keyboard::Right:   dx = 1;          tetrimino.move_tetrimino(current, previous, matrix, dx, lockDelayAction, lockClock, lockTimer); break;
                    case sf::Keyboard::C:   isHold = true;       break;
                    case sf::Keyboard::Space: hardDrop = true;   break;
                }
            }
        }

        // Soft drop
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay = 0.05;

        // Hard dorp
        if(hardDrop)
        {
            current = tetrimino.get_ghost_tetrimino(matrix, current);
            matrix.lock_to_matrix(current, matrix, tetriminoColor);
            hasHold = false;
            matrix.generate_tetrimino(matrix, tetrimino, current, tetriminoShape, tetriminoColor, rotationState, bag, bagIndex);
            if(matrix.is_game_over(current, matrix))
            {
                return 0;
            }
        }
        

        /* Lock to matrix */ 
        if(matrix.is_touch_ground(current, matrix))
        {
            if(lockTimer == 0) {lockClock.restart();}

            lockTimer += lockClock.getElapsedTime().asSeconds();
            lockClock.restart();
            
            if(lockDelayAction >= MAX_LOCK_DELAY_ACTION || lockTimer > lockDelay)
            {
                lockTimer = 0;
                lockDelayAction = 0;
                matrix.lock_to_matrix(current, matrix, tetriminoColor);
                hasHold = false;
                holdShape = tetriminoShape;
                holdColor = tetriminoColor;
                matrix.generate_tetrimino(matrix, tetrimino, current, tetriminoShape, tetriminoColor, rotationState, bag, bagIndex);
                if(matrix.is_game_over(current, matrix)) {return 0;}
            }
        }


        /* Hooooooooold */
        if(!hasHold)
        {
            if(isHold)
            {
                if(std::ranges::all_of(hold, [](Point i){return i.x == 0 && i.y == 0;}))
                {
                    // iF hold is empty(All of array is zero)
                    hold = current;
                    holdShape = tetriminoShape;
                    holdColor = tetriminoColor;
                    matrix.generate_tetrimino(matrix, tetrimino, current, tetriminoShape, tetriminoColor, rotationState, bag, bagIndex);
                    hasHold = true;
                }
                else
                {
                    hold = {0};
                    tetriminoColor = holdColor;
                    hasHold = true;
                    hold = matrix.generate_hold_tetrimino(matrix, tetrimino, current, holdShape, rotationState);
                }
            }

        }
        

        /* Drop down tetrimino */
        if(timer > delay)
        {
            matrix.drop_down(current, previous);
            if(!matrix.is_valid_move(current, matrix)) {current = previous; lockTimer += lockClock.getElapsedTime().asSeconds();}
            else{lockClock.restart(); lockTimer = 0;}
            timer = 0;
        }


        /* Check for full rows and clear them */
        matrix.clear_lines(matrix);


        // Reset them, prepare for next loop
        dx = 0;
        isRotateRight = false;
        isRotateLeft = false;
        hardDrop = false;
        isHold = false;
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