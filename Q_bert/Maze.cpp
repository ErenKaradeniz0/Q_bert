#include "Maze.h"

// Definitions for global variables
Disc Discs[2];
SquareBlock SquareBlocks[28];

void PyramidMatrix() {
    // Line 1
    SquareBlocks[0] = SquareBlock(0, 300, 100, 0, nullptr, &SquareBlocks[1], &SquareBlocks[2], nullptr, 325, 90);
    // Line 2
    SquareBlocks[1] = SquareBlock(1, 255, 170, 0, &SquareBlocks[0], &SquareBlocks[3], &SquareBlocks[4], nullptr, 280, 160);
    SquareBlocks[2] = SquareBlock(2, 345, 170, 0, nullptr, &SquareBlocks[4], &SquareBlocks[5], &SquareBlocks[0], 370, 160);
    // Line 3
    SquareBlocks[3] = SquareBlock(3, 210, 240, 0, &SquareBlocks[1], &SquareBlocks[6], &SquareBlocks[7], nullptr, 235, 230);
    SquareBlocks[4] = SquareBlock(4, 300, 240, 0, &SquareBlocks[2], &SquareBlocks[7], &SquareBlocks[8], &SquareBlocks[1], 325, 230);
    SquareBlocks[5] = SquareBlock(5, 390, 240, 0, nullptr, &SquareBlocks[8], &SquareBlocks[9], &SquareBlocks[2], 415, 230);
    // Line 4
    SquareBlocks[6] = SquareBlock(6, 165, 310, 0, &SquareBlocks[3], &SquareBlocks[10], &SquareBlocks[11], nullptr, 190, 300);
    SquareBlocks[7] = SquareBlock(7, 255, 310, 0, &SquareBlocks[4], &SquareBlocks[11], &SquareBlocks[12], &SquareBlocks[3], 280, 300);
    SquareBlocks[8] = SquareBlock(8, 345, 310, 0, &SquareBlocks[5], &SquareBlocks[12], &SquareBlocks[13], &SquareBlocks[4], 370, 300);
    SquareBlocks[9] = SquareBlock(9, 435, 310, 0, nullptr, &SquareBlocks[13], &SquareBlocks[14], &SquareBlocks[5], 460, 300);
    // Line 5
    SquareBlocks[10] = SquareBlock(10, 120, 380, 0, &SquareBlocks[6], &SquareBlocks[15], &SquareBlocks[16], nullptr, 145, 370);             // disc on left of 14th with 14th id
    SquareBlocks[11] = SquareBlock(11, 210, 380, 0, &SquareBlocks[7], &SquareBlocks[16], &SquareBlocks[17], &SquareBlocks[6], 235, 370);
    SquareBlocks[12] = SquareBlock(12, 300, 380, 0, &SquareBlocks[8], &SquareBlocks[17], &SquareBlocks[18], &SquareBlocks[7], 325, 370);
    SquareBlocks[13] = SquareBlock(13, 390, 380, 0, &SquareBlocks[9], &SquareBlocks[18], &SquareBlocks[19], &SquareBlocks[8], 415, 370);
    SquareBlocks[14] = SquareBlock(14, 480, 380, 0, nullptr, &SquareBlocks[19], &SquareBlocks[20], &SquareBlocks[9], 505, 370);             // disc on up 14th id 14
    // Line 6
    SquareBlocks[15] = SquareBlock(15, 75, 450, 0, &SquareBlocks[10], &SquareBlocks[21], &SquareBlocks[22], nullptr, 100, 440);
    SquareBlocks[16] = SquareBlock(16, 165, 450, 0, &SquareBlocks[11], &SquareBlocks[22], &SquareBlocks[23], &SquareBlocks[10], 190, 440);
    SquareBlocks[17] = SquareBlock(17, 255, 450, 0, &SquareBlocks[12], &SquareBlocks[23], &SquareBlocks[24], &SquareBlocks[11], 280, 440);
    SquareBlocks[18] = SquareBlock(18, 345, 450, 0, &SquareBlocks[13], &SquareBlocks[24], &SquareBlocks[25], &SquareBlocks[12], 370, 440);
    SquareBlocks[19] = SquareBlock(19, 435, 450, 0, &SquareBlocks[14], &SquareBlocks[25], &SquareBlocks[26], &SquareBlocks[13], 460, 440);
    SquareBlocks[20] = SquareBlock(20, 525, 450, 0, nullptr, &SquareBlocks[26], &SquareBlocks[27], &SquareBlocks[14], 550, 440);
    // Line 7
    SquareBlocks[21] = SquareBlock(21, 30, 520, 0, &SquareBlocks[15], nullptr, nullptr, nullptr, 55, 510);
    SquareBlocks[22] = SquareBlock(22, 120, 520, 0, &SquareBlocks[16], nullptr, nullptr, &SquareBlocks[15], 145, 510);
    SquareBlocks[23] = SquareBlock(23, 210, 520, 0, &SquareBlocks[17], nullptr, nullptr, &SquareBlocks[16], 235, 510);
    SquareBlocks[24] = SquareBlock(24, 300, 520, 0, &SquareBlocks[18], nullptr, nullptr, &SquareBlocks[17], 325, 510);
    SquareBlocks[25] = SquareBlock(25, 390, 520, 0, &SquareBlocks[19], nullptr, nullptr, &SquareBlocks[18], 415, 510);
    SquareBlocks[26] = SquareBlock(26, 480, 520, 0, &SquareBlocks[20], nullptr, nullptr, &SquareBlocks[19], 505, 510);
    SquareBlocks[27] = SquareBlock(27, 570, 520, 0, nullptr, nullptr, nullptr, &SquareBlocks[20], 595, 510);
}

void CreateDisc() {
    Discs[0] = { SquareBlocks[10].x - 50, SquareBlocks[10].y - 40, 28, 10, true, false };
    Discs[0].center_x = Discs[0].x + 8;
    Discs[0].center_y = Discs[0].y + 5;

    Discs[1] = { SquareBlocks[14].x + 100, SquareBlocks[14].y - 40, 29, 14, true, false };
    Discs[1].center_x = Discs[1].x + 8;
    Discs[1].center_y = Discs[1].y + 5;
}
