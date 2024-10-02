#include "tetriminos.hpp"

Tetrimino::Tetrimino() = default;

Tetrimino::~Tetrimino() = default;

const std::array<std::array<int, 4>, 7> Tetrimino::get_tetrimino()
{

    std::array<std::array<int, 4>, 7> shapes =
    {
        1, 3, 5, 7, // I
        2, 4, 5, 7, // Z
        6, 4, 5, 3, // S
        4, 3, 5, 7, // T
        2, 3, 5, 7, // J
        6, 7, 5, 3, // L
        4, 2, 3, 5, // O
    };

    return shapes;
}

// 
std::array<Shapes, 7> Tetrimino::gen7Bag()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::array<Shapes, 7> bag = {Shapes::I, Shapes::Z, Shapes::S, Shapes::T, Shapes::J, Shapes::L, Shapes::O};
    std::ranges::shuffle(bag, gen);
    return bag;
}

// Try to rotate the tetrimino!
void Tetrimino::rotate(Matrix& matrix, Shapes tetriminoShape, std::array<Point, 4> &current, std::array<Point, 4> &previous, int& rotationState, bool isRotateRight)
{
    Point center = current[2];

    previous = current;

    // O needn't ratate so far
    if(tetriminoShape == Shapes::O)
    {
        return;
    }

    // If it's I, we need calculate the center of rotation
    if(tetriminoShape == Shapes::I)
    {
        float center_x = (current[1].x + current[2].x) / 2.f;
        float center_y = (current[1].y + current[2].y) / 2.f;

        switch(rotationState)
        {
            case 0: center_y += 0.5f; break;
            case 1: center_x -= 0.5f; break;
            case 2: center_y -= 0.5f; break;
            case 3: center_x += 0.5f; break;
        }

        for(int i = 0; i < 4; i++)
        {
            float x  = current[i].y - center_y;
            float y = current[i].x - center_x;

            if(isRotateRight)
            {
                current[i].x = static_cast<int>(center_x - x);
                current[i].y = static_cast<int>(center_y + y);
            }
            else
            {
                current[i].x = static_cast<int>(center_x + x);
                current[i].y = static_cast<int>(center_y - y);
            }
        }
    }
    else    // Other tetriminos, the offsets is the same
    {

        for(int i = 0; i < 4; i++)
        {
            int x = current[i].y - center.y;
            int y = current[i].x - center.x;

            if(isRotateRight)
            {
                current[i].x = center.x - x;
                current[i].y = center.y + y;
            }
            else
            {
                current[i].x = center.x + x;
                current[i].y = center.y - y;
            }
        }
    }

    // Is valid rotation, return then update rotation state
    if(matrix.check(current, matrix))
    {
        rotationState = (isRotateRight) ? (rotationState + 1) % 4 : (rotationState + 3) % 4;
        return;
    }

    // Judge which offsets are we needed, if I, IOffset, if O, OOffset, else otherOffset
    const auto& offsets = (tetriminoShape == Shapes::I) ? IOffset : (tetriminoShape == Shapes::O) ? OOffset : otherOffset;

    int preRotationState = rotationState;
    rotationState = (isRotateRight) ? (rotationState + 1) % 4 : (rotationState + 3) % 4;

    for(int i = 0; i < 5; i++)
    {
        int dx = offsets[preRotationState][i].x - offsets[rotationState][i].x;
        int dy = offsets[preRotationState][i].y - offsets[rotationState][i].y;


        for(int j = 0; j < 4; j++)
        {
            current[j].x += dx;
            current[j].y += dy;
        }

        // Is valid， return, needn't unset the offsets
        if(matrix.check(current, matrix)) return;   

        // Action is not valid, unset offsets then reset tetrimino status and rotationState
        for(int j = 0; j < 4; j++)
        {
            current[j].x -= dx;
            current[j].y -= dy;
        }
    }
    current = previous;                 // Reset tetrimino status
    rotationState = preRotationState;   // Reset tetrimino rotation state
}
