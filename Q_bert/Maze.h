// Maze.h

#ifndef MAZE_H
#define MAZE_H

const int GRID_SIZE = 7; // Size of the Q*bert pyramid

// Class Definitions
class SquareBlock {
public:
    int x;
    int y;
    int blk_clr_state;
    int up;
    int down;
    int right;
    int left;
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

// External declarations for global variables
extern Disc Discs[2];
extern SquareBlock SquareBlocks[28];

// Function declarations
void PyramidMatrix();
void CreateDisc();

#endif // MAZE_H
