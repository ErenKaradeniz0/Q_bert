#include "icb_gui.h"
#include <map>
#include "PrintHelper.h"
#include <string>
#include "Main.h"
#include "Maze.h"
#include "Enemy.h"

extern ICBYTES screenMatrix;
extern ICBYTES Sprites3X;
extern ICBYTES IntroCoordinates;
extern int FRM1;
extern int score;
extern Player player;
extern Enemy enemy1;
extern Enemy enemy2;
ICBYTES CurrentTileMatrix, PlayerMatrix, Enemy1Matrix, Enemy2Matrix, DiscMatrix;

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

ICBYTES EnemyCoordinates{
    { 54, 66, 39, 30},  // Red Ball
    { 6, 72, 39, 24 },  // Red Ball Bounce
    { 243, 57, 48, 39 }, // Snake Egg
    { 192, 66, 48, 30 }, // Snake Egg Bounce
    { 6, 144, 42, 48 },  // Snake up move
    { 48, 96, 48, 96 }, // Snake up jump
    { 96, 144, 42, 48 }, // Snake left move
    { 144, 96, 48, 96 }, // Snake left jump
    { 198, 144, 42, 48 },  // Snake right move
    { 240, 96, 48, 96 }, // Snake right jump
    { 288, 144, 42, 48},  // Snake down move
    { 336, 96, 48, 96 }  // Snake down jump
};

void DrawEnemies() {
    if (enemy1.isAlive == true){
        int i = enemy1.state;

        Copy(Sprites3X, EnemyCoordinates.I(1, i), EnemyCoordinates.I(2, i),
            EnemyCoordinates.I(3, i), EnemyCoordinates.I(4, i),
            Enemy1Matrix);
		int printx = enemy1.x;
		int printy = enemy1.y;

        switch (i)
        {
		case 1:

		case 2:
			printy += 7;
        default:
            break;
        }
        PasteNon0(Enemy1Matrix, printx, printy, screenMatrix);
    }

    //if(enemy2.isAlive == true){
    //    int i = enemy2.state;

    //    Copy(Sprites3X, EnemyCoordinates.I(1, i), EnemyCoordinates.I(2, i),
    //        EnemyCoordinates.I(3, i), EnemyCoordinates.I(4, i),
    //        Enemy2Matrix);

    //    PasteNon0(Enemy2Matrix, enemy2.x, enemy2.y, screenMatrix);

    //}

}