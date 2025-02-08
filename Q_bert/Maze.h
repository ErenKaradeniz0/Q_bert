// Maze.h

#ifndef MAZE_H
#define MAZE_H

const int GRID_SIZE = 7; // Size of the Q*bert pyramid

// Class Definitions
class SquareBlock {
public:
    int id;
    int x;
    int y;
    int blk_clr_state;
    int up;
    int down;
    int right;
    int left;
    int centerX;
    int centerY;
    // Default constructor
    SquareBlock()
        : id(-1), x(-1), y(-1), blk_clr_state(-1), up(-1), down(-1), right(-1), left(-1), centerX(-1), centerY(-1) {
    }

    // Parameterized constructor
    SquareBlock(int id, int x, int y, int blk_clr_state, int up, int down, int right, int left, int centerX, int centerY)
        : id(id), x(x), y(y), blk_clr_state(blk_clr_state), up(up), down(down), right(right), left(left), centerX(centerX), centerY(centerY) {
    }
};

class Disc {
public:
    int x;
    int y;
    int block_id;
    bool show_state;
    bool move_state;
    int center_x;
    int center_y;
};

class Sound {
public:
    bool soundControl;
    
};

// External declarations for global variables
extern Disc Discs[2];
extern SquareBlock SquareBlocks[28];

// Function declarations
void PyramidMatrix();
void CreateDisc();

#endif // MAZE_H
