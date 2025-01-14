#pragma once

#include <random>
#include <ranges>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "global.hpp"
#include "matrix.hpp"

class Matrix;

class Tetrimino
{
public:
    // Tetrimino(Shapes shape, Colors color, States state);
    Tetrimino();

    ~Tetrimino();

    Shapes mShape;

    Colors mColor;

    States mState;

    const std::array<std::array<int, 4>, 7> get_tetrimino();

    const std::array<Point, 4> get_ghost_tetrimino(Matrix& matrix, std::array<Point, 4>& current);

    const std::array<Point, 5> get_offsets(Shapes shape, int& preRotationState, int& rotationState);

    void move_tetrimino(std::array<Point, 4>& current, std::array<Point, 4>& previous, Matrix& matrix, int dx, int& lockDelayAction, sf::Clock& lockClock, float& lockTimer);
    
    void rotate(Matrix& matrix, Shapes tetriminoShape, std::array<Point, 4>& current, std::array<Point, 4>& previous, int& rotationState, bool isRotateRight, int& lockDelayAction, sf::Clock& lockClock, float& lockTimer);

    std::array<Point, 4> kick_wall(Matrix& matrix, std::array<Point, 4>& current, std::array<Point, 4>& previous, const std::array<Point, 5>& offsets, int& preRotationState, int& rotationState, int& lockDelayAction, const bool isTouchGround, sf::Clock& lockClock, float& lockTimer);
    
    std::array<Shapes, 7> generate_7bag();

};
