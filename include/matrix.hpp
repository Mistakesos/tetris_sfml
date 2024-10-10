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

    Shapes generateShapes(Matrix& matrix, Tetrimino& tetromino, std::array<Point, 4>& current, std::array<Shapes, 7>& bag, int& bagIndex);

    Colors judgeColor(Shapes tetriminoShape);

    void drop_and_generate(std::array<Point, 4> &current, std::array<Point, 4> &previous, Matrix &matrix, Tetrimino& tetrimino, std::array<Shapes, 7>& bag, int& bagIndex, Shapes &tetriminoShape, Colors &tetriminoColor, int &rotationState, float &timer);

    bool drop_down(std::array<Point, 4>& current, std::array<Point, 4>& previous, Matrix& matrix);

    void lock_to_matrix(std::array<Point, 4>& previous, Matrix& matrix, Colors& tetriminoColor);

    void generate_tetrimino(Matrix& matrix, Tetrimino& tetrimino, std::array<Point, 4>& current, Shapes& tetriminoShape, Colors& tetriminoColor, int& rotationState, std::array<Shapes, 7>& bag, int& bagIndex);
    
    void clear_lines(Matrix& matrix);
};