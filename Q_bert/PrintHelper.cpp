#include <map>
#include "icb_gui.h"
#include "PrintHelper.h"
#include <string>
#include "Main.h"
#include "Maze.h"


extern ICBYTES screenMatrix;
extern ICBYTES Sprites3X;
extern ICBYTES IntroCoordinates;
extern int FRM1;
extern int score;
extern Player player;


const std::map<char, int> CHAR_INDICES = {
    {'0', 1}, {'1', 2}, {'2', 3}, {'3', 4}, {'4', 5},
    {'5', 6}, {'6', 7}, {'7', 8}, {'8', 9}, {'9', 10},
    {'A', 11}, {'B', 12}, {'C', 13}, {'D', 14}, {'E', 15},
    {'F', 16}, {'G', 17}, {'H', 18}, {'I', 19}, {'J', 20},
    {'K', 21}, {'L', 22}, {'M', 23}, {'N', 24}, {'O', 25},
    {'P', 26}, {'Q', 27}, {'R', 28}, {'S', 29}, {'T', 30},
    {'U', 31}, {'V', 32}, {'W', 33}, {'X', 34}, {'Y', 35},
    {'Z', 36}, {'=', 37}
};

void RenderChar(ICBYTES& screen, char c, int x, int y) {
    auto it = CHAR_INDICES.find(c);
    if (it == CHAR_INDICES.end()) return;

    ICBYTES letterSprite;
    Copy(Sprites3X,
        IntroCoordinates.I(1, it->second),
        IntroCoordinates.I(2, it->second),
        IntroCoordinates.I(3, it->second),
        IntroCoordinates.I(4, it->second),
        letterSprite);
    PasteNon0(letterSprite, x, y, screen);
}

void RenderString(ICBYTES& screen, const char* text, int x, int y, int spacing) {
    int currentX = x;
    while (*text) {
        if (*text != ' ') {
            RenderChar(screen, *text, currentX, y);
        }
        currentX += spacing;
        text++;
    }
}

void ShowGameOverScreen() {
    gameRunning = false;
    Sleep(30);

    FillRect(screenMatrix, 225, 375, 220, 25, 0x000000);
    RenderString(screenMatrix, "GAME OVER", 225, 375, 25);
    DisplayImage(FRM1, screenMatrix);
}

void DrawScore() {
    char scoreText[20] = "Score:  ";
    int tempScore = score;
    int index = 8; // Start placing digits after "Score: "

    // Place digits in reverse order
    do {
        scoreText[index--] = '0' + (tempScore % 10);
        tempScore /= 10;
    } while (tempScore > 0);

    // Ensure the string is null-terminated
    scoreText[9] = '\0';

    Impress12x20(screenMatrix, 10, 10, scoreText, 0xFFFFFF); // Draw score in top left corner
}

void DrawLives() {
    char livesText[20] = "Lives:  ";
    int tempLives = player.lifes;
    int index = 7; // Start placing digits after "Lives: "

    // Place digits in reverse order
    do {
        livesText[index--] = '0' + (tempLives % 10);
        tempLives /= 10;
    } while (tempLives > 0);

    // Ensure the string is null-terminated
    livesText[8] = '\0';

    Impress12x20(screenMatrix, 10, 40, livesText, 0xFFFFFF); // Draw lives in top left corner
}

void DrawMap() {
    // Blue Tile
    int temp = 0;

    for (int i = 0; i < 28; i++) {
        switch (SquareBlocks[i].blk_clr_state)
        {
        case 0: Copy(Sprites3X, 2, 224 * 3 + 1, 32 * 3, 32 * 3, CurrentTileMatrix); break;
        case 1: Copy(Sprites3X, 2, 192 * 3 + 1, 32 * 3, 32 * 3, CurrentTileMatrix); break;
        default:
            break;
        }

        PasteNon0(CurrentTileMatrix, SquareBlocks[i].x, SquareBlocks[i].y, screenMatrix);
    }
}

void DrawDisc() {
    for (int i = 0; i < 2; i++) {
        if (Discs[i].show_state) {
            FillCircle(screenMatrix, Discs[i].x, Discs[i].y, 25, 0x0FFFFFF);
        }
    }
}

ICBYTES PlayerCoordinates{
    { 3, 6, 45, 45},     // 1 (UP-1) 
    { 51, 1, 45, 48 },   // 2 (UP-2)
    { 97, 6, 45, 42 },   // 3 (LEFT-1)
    { 145, 1, 45, 48 },  // 4 (LEFT-1)
    { 192, 6, 45, 42 },  // 5 (DOWN-1)
    { 240, 1, 45, 48 },  // 6 (DOWN-2)
    { 291, 6, 45, 42 },  // 7 (RIGHT-1)
    { 339, 1, 45, 48 }   // 8 (RIGHT-2)
};

void DrawPlayer() {
    int i = player.direction;
    Copy(Sprites3X, PlayerCoordinates.I(1, i), PlayerCoordinates.I(2, i),
        PlayerCoordinates.I(3, i), PlayerCoordinates.I(4, i),
        PlayerMatrix);

    PasteNon0(PlayerMatrix, player.x, player.y, screenMatrix);
}


void DrawEnemies() {
    //for (int i = 0; i < max_enemies; ++i) {
    //    fillrect(screenmatrix, enemies[i].x, enemies[i].y, 10, 10, 0x00ff00);
    //}
}