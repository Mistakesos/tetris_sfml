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

    bool is_valid_move(std::array<Point, 4>& current, Matrix& matrix) const;

    bool is_touch_ground(std::array<Point, 4>& current, Matrix& matrix) const;

    bool is_game_over(std::array<Point, 4> current, Matrix& matrix) const;

    Shapes generate_shapes(Matrix& matrix, Tetrimino& tetromino, std::array<Point, 4>& current, std::array<Shapes, 7>& bag, int& bagIndex);

    Colors judge_color(Shapes tetriminoShape);

    void drop_down(std::array<Point, 4>& current, std::array<Point, 4>& previous);

    void lock_to_matrix(std::array<Point, 4>& previous, Matrix& matrix, Colors& tetriminoColor);

    void generate_tetrimino(Matrix& matrix, Tetrimino& tetrimino, std::array<Point, 4>& current, Shapes& tetriminoShape, Colors& tetriminoColor, int& rotationState, std::array<Shapes, 7>& bag, int& bagIndex);
    
    void clear_lines(Matrix& matrix);
};