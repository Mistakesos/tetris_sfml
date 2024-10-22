#include "matrix.hpp"
#include "tetriminos.hpp"

Matrix::Matrix() = default;

Matrix::~Matrix() = default;

// Check is't valid move, touched wall or touched another tetrimino
bool Matrix::is_valid_move(std::array<Point, 4>& current, Matrix& matrix) const
{
    for(int i = 0; i < 4; i++)
    {
        if(current[i].x < 0 || current[i].x >= COLS || current[i].y >= ROWS) return false;
        else if(matrix.m_matrix[current[i].y][current[i].x]) return false;
    }
    return true;
}

bool Matrix::is_touch_ground(std::array<Point, 4> &current, Matrix &matrix) const
{
    for(auto& cell : current)
    {
        auto tempCell = cell;
        tempCell.y += 1;

        if(tempCell.y >= ROWS)
        {
            return true;
        }
        else if(matrix.m_matrix[tempCell.y][tempCell.x])
        {
            return true;
        }
    }

    return false;
}

bool Matrix::is_game_over(std::array<Point, 4> current, Matrix &matrix) const
{
    for(auto& cell : current)
    {
        if(matrix.m_matrix[cell.y][cell.x])
        {
            return true;
        }
    }
    return false;
}


Shapes Matrix::generate_shapes(Matrix &matrix, Tetrimino& tetrimino,  std::array<Point, 4> &current, std::array<Shapes, 7> &bag, int &bagIndex)
{
    Shapes tetriminoShapes  = bag[bagIndex++];
    if(bagIndex == 7)
    {
        bagIndex = 0;
        bag = tetrimino.generate_7bag();
    }


    int tetriminoIndex = static_cast<int>(tetriminoShapes); // Bad luck, I need the enum value. And so far, I can't handle it!
    if(tetriminoShapes == Shapes::O || tetriminoShapes == Shapes::I) // Tetromino is I, X need plus 1;
    {
        for(int i = 0; i < 4; i++)
        {
            current[i].x = tetrimino.get_tetrimino()[tetriminoIndex][i] / 2 + (COLS / 2) - 2 ;
            current[i].y = tetrimino.get_tetrimino()[tetriminoIndex][i] % 2 + 18;
        }

    }
    else
    {
        for(int i = 0; i < 4; i++)
        {
            current[i].x = tetrimino.get_tetrimino()[tetriminoIndex][i] / 2 + (COLS / 2) - 3;
            current[i].y = tetrimino.get_tetrimino()[tetriminoIndex][i] % 2 + 18;
        }
    }

    return tetriminoShapes;
}

Colors Matrix::judge_color(Shapes tetriminoShape)
{
    switch(tetriminoShape)
    {
        case Shapes::I: return Colors::CYAN;
        case Shapes::Z: return Colors::RED;
        case Shapes::S: return Colors::GREEN;
        case Shapes::T: return Colors::PURPLE;
        case Shapes::J: return Colors::BLUE;
        case Shapes::L: return Colors::ORANGE;
        case Shapes::O: return Colors::YELLOW;
    }
    return Colors::CYAN;
}

void Matrix::drop_down(std::array<Point, 4>& current, std::array<Point, 4>& previous)
{
    previous = current;

    for(auto& cell : current)
    {
        cell.y += 1;
    }
}

void Matrix::lock_to_matrix(std::array<Point, 4>& previous, Matrix& matrix, Colors& tetriminoColor)
{
    int colorIndex = static_cast<int>(tetriminoColor);
    for(auto& cell : previous)
    {
        matrix.m_matrix[cell.y][cell.x] = colorIndex;
    }
}

void Matrix::generate_tetrimino(Matrix& matrix, Tetrimino& tetrimino, std::array<Point, 4>& current, Shapes& tetriminoShape, Colors& tetriminoColor, int& rotationState, std::array<Shapes, 7>& bag, int& bagIndex)
{
    tetriminoShape = matrix.generate_shapes(matrix, tetrimino, current, bag, bagIndex);
    tetriminoColor = matrix.judge_color(tetriminoShape);
    rotationState = 0;
}

void Matrix::clear_lines(Matrix& matrix)
{
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
}