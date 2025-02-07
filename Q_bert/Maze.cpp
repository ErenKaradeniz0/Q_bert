#include "Maze.h"

// Definitions for global variables
Disc Discs[2];
SquareBlock SquareBlocks[28];

void PyramidMatrix() {
    //line 1
    SquareBlocks[0] = { 0, 300, 100, 0, -1, 1, 2, -1, 325, 90 }; // id, x, y, colorstate, up, down, right, left, centerX, centerY
    //line 2
    SquareBlocks[1] = { 1, 255, 170, 0, 0, 3, 4, -1, 280, 160 };
    SquareBlocks[2] = { 2, 345, 170, 0, -1, 4, 5, 0, 370, 160 };
    //line 3
    SquareBlocks[3] = { 3, 210, 240, 0, 1, 6, 7, -1, 235, 230 };
    SquareBlocks[4] = { 4, 300, 240, 0, 2, 7, 8, 1, 325, 230 };
    SquareBlocks[5] = { 5, 390, 240, 0, -1, 8, 9, 2, 415, 230 };
    //line 4
    SquareBlocks[6] = { 6, 165, 310, 0, 3, 10, 11, -1, 190, 300 };
    SquareBlocks[7] = { 7, 255, 310, 0, 4, 11, 12, 3, 280, 300 };
    SquareBlocks[8] = { 8, 345, 310, 0, 5, 12, 13, 4, 370, 300 };
    SquareBlocks[9] = { 9, 435, 310, 0, -1, 13, 14, 5, 460, 300 };
    //Line 5
    SquareBlocks[10] = { 10, 120, 380, 0, 6, 15, 16, -1, 145, 370 };
    SquareBlocks[11] = { 11, 210, 380, 0, 7, 16, 17, 6, 235, 370 };
    SquareBlocks[12] = { 12, 300, 380, 0, 8, 17, 18, 7, 325, 370 };
    SquareBlocks[13] = { 13, 390, 380, 0, 9, 18, 19, 8, 415, 370 };
    SquareBlocks[14] = { 14, 480, 380, 0, -1, 19, 20, 9, 505, 370 };
    //Line 6
    SquareBlocks[15] = { 15, 75, 450, 0, 10, 21, 22, -1, 100, 440 };
    SquareBlocks[16] = { 16, 165, 450, 0, 11, 22, 23, 10, 190, 440 };
    SquareBlocks[17] = { 17, 255, 450, 0, 12, 23, 24, 11, 280, 440 };
    SquareBlocks[18] = { 18, 345, 450, 0, 13, 24, 25, 12, 370, 440 };
    SquareBlocks[19] = { 19, 435, 450, 0, 14, 25, 26, 13, 460, 440 };
    SquareBlocks[20] = { 20, 525, 450, 0, -1, 26, 27, 14, 550, 440 };
    //Line 7
    SquareBlocks[21] = { 21, 30, 520, 0, 15, -1, -1, -1, 55, 510 };
    SquareBlocks[22] = { 22, 120, 520, 0, 16, -1, -1, 15, 145, 510 };
    SquareBlocks[23] = { 23, 210, 520, 0, 17, -1, -1, 16, 235, 510 };
    SquareBlocks[24] = { 24, 300, 520, 0, 18, -1, -1, 17, 325, 510 };
    SquareBlocks[25] = { 25, 390, 520, 0, 19, -1, -1, 18, 415, 510 };
    SquareBlocks[26] = { 26, 480, 520, 0, 20, -1, -1, 19, 505, 510 };
    SquareBlocks[27] = { 27, 570, 520, 0, -1, -1, -1, 20, 595, 510 };
}

void CreateDisc() {
    SquareBlocks[10].left = 40; //left
    Discs[0] = { SquareBlocks[10].x - 50,SquareBlocks[10].y - 40,10,true,false }; //x,y,block_id,show_state,move_state
    Discs[0].center_x = Discs[0].x + 8;
    Discs[0].center_y = Discs[0].y + 5;

    SquareBlocks[14].up = 45; //rigth
    Discs[1] = { SquareBlocks[14].x + 100,SquareBlocks[14].y - 40,14,true,false }; //x,y,block_id,show_state,move_state
    Discs[1].center_x = Discs[1].x + 8;
    Discs[1].center_y = Discs[1].y + 5;

}

void DiskAndPlayerMovingAnimation(int id) {
    
}