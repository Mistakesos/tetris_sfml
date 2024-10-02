#pragma once

#include <vector>
#include <array>
#include "global.hpp"
#include "tetriminos.hpp"

class Tetrimino;

class Matrix
{
public:
    Matrix();
    ~Matrix();
    std::vector<std::vector<int>> m_matrix;
    bool check(std::array<Point, 4>& current, Matrix& matrix);
    Shapes generateTetromino(Matrix& matrix, Tetrimino& tetromino, std::array<Point, 4>& current, std::array<Shapes, 7>& bag, int& bagIndex);
    Colors judgeColor(Shapes tetriminoShape);
};