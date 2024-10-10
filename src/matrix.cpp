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

Shapes Matrix::generateShapes(Matrix &matrix, Tetrimino& tetrimino,  std::array<Point, 4> &current, std::array<Shapes, 7> &bag, int &bagIndex)
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

void Matrix::drop_and_generate(std::array<Point, 4> &current, std::array<Point, 4> &previous, Matrix &matrix, Tetrimino& tetrimino, std::array<Shapes, 7>& bag, int& bagIndex, Shapes &tetriminoShape, Colors &tetriminoColor, int &rotationState, float &timer)
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
        tetriminoShape = matrix.generateShapes(matrix, tetrimino, current, bag, bagIndex);

        // reset Rotation state
        rotationState = 0;

        // colorIndex = genColor(gen);  // Real random color
        tetriminoColor = matrix.judgeColor(tetriminoShape);
    }

    timer = 0;

}

bool Matrix::drop_down(std::array<Point, 4>& current, std::array<Point, 4>& previous, Matrix& matrix)
{
    for(int i = 0; i < 4; i++)
    {
        previous[i] = current[i];
        current[i].y += 1;
    }

    if(!matrix.check(current, matrix))
    {
        // Invalid when drop down, reset to previous then return current
        // timer = 0;
        return false;
    }

    return true;
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
    tetriminoShape = matrix.generateShapes(matrix, tetrimino, current, bag, bagIndex);
    tetriminoColor = matrix.judgeColor(tetriminoShape);
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
