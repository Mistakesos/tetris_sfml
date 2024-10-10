#pragma once

constexpr int ROWS = 40;    // Width of matrix
constexpr int COLS = 10;    // Height of matrix

struct Point{int x, y;};    // Positions of Tetrimino

enum class Shapes{I,Z,S,T,J,L,O};

enum class Colors
{
    CYAN = 5,
    RED = 2,
    GREEN = 3,
    PURPLE = 1,
    BLUE = 7,
    ORANGE = 6,
    YELLOW = 4
};

enum class States {ZERO, RIGHT, TWO, LEFT};