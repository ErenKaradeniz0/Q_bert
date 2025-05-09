// Maze.h
#pragma once

class SquareBlock {
public:
    int id;
    int x;
    int y;
    int state;
    SquareBlock* up;
    SquareBlock* down;
    SquareBlock* right;
    SquareBlock* left;
    int centerX;
    int centerY;

    // Default constructor
    SquareBlock()
        : id(-1), x(-1), y(-1), state(-1), up(nullptr), down(nullptr), right(nullptr), left(nullptr), centerX(-1), centerY(-1) {
    }

    // Parameterized constructor
    SquareBlock(int id, int x, int y, int blk_clr_state, SquareBlock* up, SquareBlock* down, SquareBlock* right, SquareBlock* left, int centerX, int centerY)
        : id(id), x(x), y(y), state(blk_clr_state), up(up), down(down), right(right), left(left), centerX(centerX), centerY(centerY) {
    }
};

class Disc {
public:
    int x;
    int y;
    int disc_id;
    int block_id;
    bool show_state;
    bool move_state;
    int center_x;
    int center_y;
};

// Global variables
extern Disc Discs[2];
extern SquareBlock SquareBlocks[28];

// Function declarations
void PyramidMatrix();
void CreateDisc();

