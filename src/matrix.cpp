#include "matrix.hpp"
#include "tetriminos.hpp"

Matrix::Matrix() = default;

Matrix::~Matrix() = default;

// Check is't valid move, touched wall or touched another tetrimino
bool Matrix::check(std::array<Point, 4>& current, Matrix& matrix)
{
    for(int i = 0; i < 4; i++)
    {
        if(current[i].x < 0 || current[i].x >= COLS || current[i].y >= ROWS) return false;
        else if(matrix.m_matrix[current[i].y][current[i].x]) return false;
    }
    return true;
}

Shapes Matrix::generateTetromino(Matrix &matrix, Tetrimino& tetrimino,  std::array<Point, 4> &current, std::array<Shapes, 7> &bag, int &bagIndex)
{
    Shapes tetriminoShapes  = bag[bagIndex++];
    if(bagIndex == 7)
    {
        bagIndex = 0;
        bag = tetrimino.gen7Bag();
    }


    int tetriminoIndex = static_cast<int>(tetriminoShapes); // Bad luck, I need the enum value. And so far, I can't handle it!
    if(tetriminoShapes == Shapes::O || tetriminoShapes == Shapes::I) // Tetromino is I, X need plus 1;
    {
        for(int i = 0; i < 4; i++)
        {
            current[i].x = tetrimino.get_tetrimino()[tetriminoIndex][i] / 2 + (COLS / 2) - 2 ;
            current[i].y = tetrimino.get_tetrimino()[tetriminoIndex][i] % 2;
        }

    }
    else
    {
        for(int i = 0; i < 4; i++)
        {
            current[i].x = tetrimino.get_tetrimino()[tetriminoIndex][i] / 2 + (COLS / 2) - 3;
            current[i].y = tetrimino.get_tetrimino()[tetriminoIndex][i] % 2;
        }
    }

    return tetriminoShapes;
}

Colors Matrix::judgeColor(Shapes tetriminoShape)
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
