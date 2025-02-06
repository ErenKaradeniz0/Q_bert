#include "Maze.h"

// Definitions for global variables
Disc Discs[2];
SquareBlock SquareBlocks[28];

void PyramidMatrix() {
    //line 1
    SquareBlocks[0] = { 300,100,0,-1,1,2,-1 }; // x, y, colorstate, up, down, right left 
    //line 2
    SquareBlocks[1] = { 255,170,0,0,3,4,-1 };
    SquareBlocks[2] = { 345,170,0,-1,4,5,0 };
    //line 3
    SquareBlocks[3] = { 210,240,0,1,6,7,-1 };
    SquareBlocks[4] = { 300,240,0,2,7,8,1 };
    SquareBlocks[5] = { 390,240,0,-1,8,9,2 };
    //line 4
    SquareBlocks[6] = { 165,310,0,3,10,11,-1 };
    SquareBlocks[7] = { 255,310,0,4,11,12,3 };
    SquareBlocks[8] = { 345,310,0,5,12,13,4 };
    SquareBlocks[9] = { 435,310,0,-1,13,14,5 };
    //Line 5
    SquareBlocks[10] = { 120,380,0,6,15,16,-1 };
    SquareBlocks[11] = { 210,380,0,7,16,17,6 };
    SquareBlocks[12] = { 300,380,0,8,17,18,7 };
    SquareBlocks[13] = { 390,380,0,9,18,19,8 };
    SquareBlocks[14] = { 480,380,0,-1,19,20,9 };
    //Line 6
    SquareBlocks[15] = { 75,450,0,10,21,22,-1 };
    SquareBlocks[16] = { 165,450,0,11,22,23,10 };
    SquareBlocks[17] = { 255,450,0,12,23,24,11 };
    SquareBlocks[18] = { 345,450,0,13,24,25,12 };
    SquareBlocks[19] = { 435,450,0,14,25,26,13 };
    SquareBlocks[20] = { 525,450,0,-1,26,27,14 };
    //Line 7
    SquareBlocks[21] = { 30,520,0,15,-1,-1,-1 };
    SquareBlocks[22] = { 120,520,0,16,-1,-1,15 };
    SquareBlocks[23] = { 210,520,0,17,-1,-1,16 };
    SquareBlocks[24] = { 300,520,0,18,-1,-1,17 };
    SquareBlocks[25] = { 390,520,0,19,-1,-1,18 };
    SquareBlocks[26] = { 480,520,0,20,-1,-1,19 };
    SquareBlocks[27] = { 570,520,0,-1,-1,-1,20 };

}

void CreateDisc() {
    SquareBlocks[10].left = 40; //left
    Discs[0] = { SquareBlocks[10].x - 15,SquareBlocks[10].y - 40,10,true,false }; //x,y,block_id,state
    SquareBlocks[14].up = 45; //right
    Discs[1] = { SquareBlocks[14].x + 110,SquareBlocks[14].y - 40,14,true,false }; //x,y,block_id,show_state,move_state
}
