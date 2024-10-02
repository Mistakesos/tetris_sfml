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
    Tetrimino();
    ~Tetrimino();

    
    const std::array<std::array<int, 4>, 7> get_tetrimino();
    const std::array<std::array<Point, 5>, 4> otherOffset =
    {
        // Rotation state 0
        Point{0, 0}, Point{0, 0}, Point{0, 0}, Point{0, 0}, Point{0, 0},
        // Rotation state R
        Point{0, 0}, Point{1, 0}, Point{1, -1}, Point{0, 2}, Point{1, 2},
        // Rotation state 2
        Point{0, 0}, Point{0, 0}, Point{0, 0}, Point{0, 0}, Point{0, 0},
        // Rotation state L
        Point{0, 0}, Point{-1, 0}, Point{-1, -1}, Point{0, 2}, Point{-1, 2}
    };

    const std::array<std::array<Point, 5>, 4> IOffset =
    {
        // Rotation state 0
        Point{0, 0}, Point{-1, 0}, Point{2, 0}, Point{-1, 0}, Point{2, 0},
        // Rotation state R
        Point{-1, 0}, Point{0, 0}, Point{0, 0}, Point{0, 1}, Point{0, -2},
        // Rotation state 2
        Point{-1, 1}, Point{1, 1}, Point{-2, 1}, Point{1, 0}, Point{-2, 0},
        // Rotation state L
        Point{0, 1}, Point{0, 1}, Point{0, 1}, Point{0, -1}, Point{0, 2}
    };

    const std::array<std::array<Point, 5>, 4> OOffset =
    {{
        {Point{0, 0}},    // Rotation state 0
        {Point{0, -1}},   // Rotation state R
        {Point{-1, -1}},  // Rotation state 2
        {Point{-1, 0}}    // Rotation state L
    }};

    std::array<Shapes, 7> gen7Bag();

    void rotate(Matrix& matrix, Shapes tetriminoShape, std::array<Point, 4>& current, std::array<Point, 4>& previous, int& rotationState, bool isRotateRight);
};
