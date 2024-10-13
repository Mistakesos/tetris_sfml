#include "tetriminos.hpp"

Tetrimino::Tetrimino() = default;

Tetrimino::~Tetrimino() = default;

// Embarrassingly, currently we have only returned an array and have not truly 'obtained' tetrimino
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

// Get ghost tetrimino
const std::array<Point, 4> Tetrimino::get_ghost_tetrimino(Matrix& matrix, std::array<Point, 4>& current)
{
    bool keep_falling = true;
    int total_movement = 0;
    std::array<Point, 4> ghost = current;

    while(keep_falling)
    {
        total_movement++;

        for(auto& point : current)
        {
            if(point.y + total_movement == ROWS)
            {
                keep_falling = false;
                break;
            }

            if(point.y + total_movement < 0)
            {
                continue;
            }
            else if(matrix.m_matrix[point.y + total_movement][point.x] > 0)
            {
                keep_falling = false;
                break;
            }
        }
    }

    for(auto& point : ghost)
    {
        point.y += total_movement - 1;
    }

    return ghost;
}

// Generate 7-bag
std::array<Shapes, 7> Tetrimino::generate_7bag()
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::array<Shapes, 7> bag = {Shapes::I, Shapes::Z, Shapes::S, Shapes::T, Shapes::J, Shapes::L, Shapes::O};
    std::ranges::shuffle(bag, gen);
    return bag;
}

// Get offsets
const std::array<Point, 5> Tetrimino::get_offsets(Shapes shape, int& preRotationState, int& rotationState)
{
    if(shape == Shapes::I)
    {
        switch(preRotationState)
        {
            case 0:
            {
                switch(rotationState)
                {
                    case 1: return {{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}};
                    case 3: return {{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}};
                }
            }
            case 1:
            {
                switch(rotationState)
                {
                    case 2: return {{{0, 0}, {-1, 0}, {2, 0}, {-1, 2}, {2, -1}}};
                    case 0: return {{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}};
                }
            }
            case 2:
            {
                switch(rotationState)
                {
                    case 3: return {{{0, 0}, {2, 0}, {-1, 0}, {2, 1}, {-1, -2}}};
                    case 1: return {{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}};
                }
            }
            case 3:
            {
                switch(rotationState)
                {
                    case 0: return {{{0, 0}, {1, 0}, {-2, 0}, {1, -2}, {-2, 1}}};
                    case 2: return {{{0, 0}, {-2, 0}, {1, 0}, {-2, -1}, {1, 2}}};
                }
            }
        }
    }
    else
    {
        switch(preRotationState)
        {
            case 0:
            {
                switch(rotationState)
                {
                    case 1: return {{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}};
                    case 3: return {{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}};
                }
            }
            case 1:
            {
                switch(rotationState)
                {
                    case 2: return {{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}};
                    case 0: return {{{0, 0}, {1, 0}, {1, -1}, {0, 2}, {1, 2}}};
                }
            }
            case 2:
            {
                switch(rotationState)
                {
                    case 3: return {{{0, 0}, {1, 0}, {1, 1}, {0, -2}, {1, -2}}};
                    case 1: return {{{0, 0}, {-1, 0}, {-1, 1}, {0, -2}, {-1, -2}}};
                }
            }
            case 3:
            {
                switch(rotationState)
                {
                    case 0: return {{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}};
                    case 2: return {{{0, 0}, {-1, 0}, {-1, -1}, {0, 2}, {-1, 2}}};
                }
            }
        }
    }

    return {0};
}

// Move tetrimino <---Left and Right--->
void Tetrimino::move_tetrimino(std::array<Point, 4>& current, std::array<Point, 4>& previous, Matrix& matrix, int dx)
{
        for(int i = 0; i < 4; i++)
        {
            previous[i] = current[i];
            current[i].x += dx;
        }
        if(!matrix.is_valid_move(current, matrix))
        {
            for(int i = 0; i < 4; i++)
            {
                current[i] = previous[i];
            }
        }

}

// Try to rotate the tetrimino!
void Tetrimino::rotate(Matrix& matrix, Shapes tetriminoShape, std::array<Point, 4> &current, std::array<Point, 4> &previous, int& rotationState, bool isRotateRight)
{
    Point center = current[2];

    previous = current;

    // if(tetriminoShape == Shapes::O)
    // {
    //     // O needn't ratate so far
    //     return;
    // }

    if(tetriminoShape == Shapes::I || tetriminoShape == Shapes::O)
    {
        // If it's I or O, we need calculate the center of rotation then rotate
        float center_x = 0;
        float center_y = 0;

        if(tetriminoShape == Shapes::I)
        {
            center_x = (current[1].x + current[2].x) / 2.f;
            center_y = (current[1].y + current[2].y) / 2.f;

            switch(rotationState)
            {
                case 0: center_y += 0.5f; break;
                case 1: center_x -= 0.5f; break;
                case 2: center_y -= 0.5f; break;
                case 3: center_x += 0.5f; break;
            }
        }
        else
        {
            switch(rotationState)
            {
                case 0:
                {
                    center_x = (current[0].x + current[1].x) / 2.f;
                    center_y = (current[1].y + current[2].y) / 2.f;
                }
                    break;
                case 1:
                {
                    center_x = (current[1].x + current[2].x) / 2.f;
                    center_y = (current[2].y + current[3].y) / 2.f;
                }
                    break;
                case 2:
                {
                    center_x = (current[2].x + current[3].x) / 2.f;
                    center_y = (current[3].y + current[0].y) / 2.f;
                }
                    break;
                case 3:
                {
                    center_x = (current[3].x + current[0].x) / 2.f;
                    center_y = (current[0].y + current[1].y) / 2.f;
                }
                    break;
            }
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
    else
    {
        // For other tetriminos, centers is the same one, rotate directly
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

    if(matrix.is_valid_move(current, matrix))
    {
        // Is valid rotation, return then update rotation state
        rotationState = (isRotateRight) ? (rotationState + 1) % 4 : (rotationState + 3) % 4;
        return;
    }

    // Save rotation state to previous state
    int preRotationState = rotationState;

    // Update rotation state
    rotationState = (isRotateRight) ? (rotationState + 1) % 4 : (rotationState + 3) % 4;

    // Judge which offsets are we needed, if I, IOffset, else otherOffset
    const auto offsets = get_offsets(tetriminoShape, preRotationState, rotationState);

    // Try to kick wall
    current = kick_wall(matrix, current, previous, offsets, preRotationState, rotationState);
}

// Kick wall, if valid, return it, invalid, return previous one
std::array<Point, 4> Tetrimino::kick_wall(Matrix& matrix, std::array<Point, 4>& current, std::array<Point, 4>& previous, const std::array<Point, 5>& offsets, int& preRotationState, int& rotationState)
{

    // Try to kick wall --plus the offsets, try 5 times
    for(int i = 0; i < 5; i++)
    {
        int dx = offsets[i].x;
        int dy = offsets[i].y;

        // Every Point needs to plus the offsets, 4 times
        for(int j = 0; j < 4; j++)
        {
            current[j].x += dx;
            current[j].y += dy;
        }

        if(matrix.is_valid_move(current, matrix))
        {
            // If it's valid after kick wall, return current(kicked)
            return current;
        }
        else
        {
            // It's invalid even after kick wall, reset offsets
            for(int j = 0; j < 4; j++)
            {
                current[j].x -= dx;
                current[j].y -= dy;
            }
        }
    }

    // All positions is invalid after kick wall, reset rotation state to previous rotation state, then return previous tetrimino
    current = previous;
    rotationState = preRotationState;

    return current;
}