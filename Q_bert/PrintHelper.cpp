#include "icb_gui.h"
#include <map>
#include "PrintHelper.h"
#include <string>
#include "Main.h"
#include "Maze.h"
#include "Enemy.h"
#include "Game.h"

extern ICBYTES screenMatrix;
extern ICBYTES Sprites3X;
extern ICBYTES IntroCoordinates;
extern int FRM1;
extern int score;
extern Player player;
extern Enemy enemyBall1, enemyBall2, enemySnake;

ICBYTES CurrentTileMatrix, PlayerMatrix, Enemy1Matrix, Enemy2Matrix, DiscMatrix, miniQbert;

std::map<char, int> CHAR_INDICES = {
    {'0', 1}, {'1', 2}, {'2', 3}, {'3', 4}, {'4', 5},
    {'5', 6}, {'6', 7}, {'7', 8}, {'8', 9}, {'9', 10},
    {'A', 11}, {'B', 12}, {'C', 13}, {'D', 14}, {'E', 15},
    {'F', 16}, {'G', 17}, {'H', 18}, {'I', 19}, {'J', 20},
    {'K', 21}, {'L', 22}, {'M', 23}, {'N', 24}, {'O', 25},
    {'P', 26}, {'Q', 27}, {'R', 28}, {'S', 29}, {'T', 30},
    {'U', 31}, {'V', 32}, {'W', 33}, {'X', 34}, {'Y', 35},
    {'Z', 36}, {'=', 37}
};

ICBYTES letterSprite;
void RenderChar(ICBYTES& screen, char c, int x, int y) {
    int index = CHAR_INDICES[c];
    if (index == 0) return;

    Copy(Sprites3X,
        IntroCoordinates.I(1, index),
        IntroCoordinates.I(2, index),
        IntroCoordinates.I(3, index),
        IntroCoordinates.I(4, index),
        letterSprite);
    PasteNon0(letterSprite, x, y, screen);
    Free(letterSprite);
}

void RenderString(ICBYTES& screen, const char* text, int x, int y, int spacing) {
    int currentX = x;
    while (*text != '\0') {
        if (*text != ' ') {
            RenderChar(screen, *text, currentX, y);
        }
        currentX += spacing;
        text++;
    }
}

void DrawScore() {
    RenderString(screenMatrix, "SCORE", 10, 40, 25);

    int currentX = 150;  // Extra space between "SCORE" and number

    // Handle case when score is 0
    if (score == 0) {
        RenderChar(screenMatrix, '0', currentX, 40);
        return;
    }

    // Convert score to digits
    int tempScore = score;
    char digits[10];
    int digitCount = 0;

    while (tempScore > 0) {
        digits[digitCount++] = '0' + (tempScore % 10);
        tempScore /= 10;
    }

    // Draw digits in reverse order (right way around)
    for (int i = digitCount - 1; i >= 0; i--) {
        RenderChar(screenMatrix, digits[i], currentX, 40);
        currentX += 25;
    }
}

void DrawLives() {
    if ((enemyBall1.isAlive && player.currentTile.id == enemyBall1.currentTile.id) ||
        (enemyBall2.isAlive && player.currentTile.id == enemyBall2.currentTile.id) ||
        (enemySnake.isAlive && player.currentTile.id == enemySnake.currentTile.id)) {
        player.lostLife(false);
    }

    Copy(Sprites3X,
        IntroCoordinates.I(1, 52),
        IntroCoordinates.I(2, 52),
        IntroCoordinates.I(3, 52),
        IntroCoordinates.I(4, 52),
        miniQbert);

    const int MINI_QBERT_SPACING = 35;  // Vertical spacing between Q*berts
    const int START_Y = 170;  // Starting Y position (below "LIVES" text)

    for (int i = 0; i < player.lifes; i++) {
        PasteNon0(miniQbert, 25, START_Y + (i * MINI_QBERT_SPACING), screenMatrix);
    }
}

void DrawMap() {
    for (int i = 0; i < 28; i++) {
        switch (SquareBlocks[i].state) {
        case 0: Copy(Sprites3X, 2, 224 * 3 + 1, 32 * 3, 32 * 3, CurrentTileMatrix); break;
        case 1: Copy(Sprites3X, 2, 160 * 3 + 1, 32 * 3, 32 * 3, CurrentTileMatrix); break;
        case 2: Copy(Sprites3X, 2, 192 * 3 + 1, 32 * 3, 32 * 3, CurrentTileMatrix); break;
        default: break;
        }
        PasteNon0(CurrentTileMatrix, SquareBlocks[i].x, SquareBlocks[i].y, screenMatrix);
    }
}

void SelectEffectDisc(int k) {
    switch (k) {
    case 0: Copy(Sprites3X, 1, 356 * 3 + 1, 16 * 3, 10 * 3, DiscMatrix); break;
    case 1: Copy(Sprites3X, 16 * 3 + 1, 356 * 3 + 1, 16 * 3, 10 * 3, DiscMatrix); break;
    case 2: Copy(Sprites3X, 32 * 3 + 1, 356 * 3 + 1, 16 * 3, 10 * 3, DiscMatrix); break;
    case 3: Copy(Sprites3X, 48 * 3 + 1, 356 * 3 + 1, 16 * 3, 10 * 3, DiscMatrix); break;
    default: break;
    }
}

void DrawDisc() {
    for (int i = 0; i < 2; i++) {
        if (Discs[i].show_state) {
            PasteNon0(DiscMatrix, Discs[i].x, Discs[i].y, screenMatrix);
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
    { 339, 1, 45, 48 },  // 8 (RIGHT-2)
    { 387, 249, 150, 78 }   //  Qbert text - idx 39
};

void DrawPlayer() {
    int i = player.direction;
    Copy(Sprites3X, PlayerCoordinates.I(1, i), PlayerCoordinates.I(2, i),
        PlayerCoordinates.I(3, i), PlayerCoordinates.I(4, i),
        PlayerMatrix);

    PasteNon0(PlayerMatrix, player.x, player.y, screenMatrix);

    if (player.showLostLifeText) {
        Game::Pause(false);
        if (player.lostLifeCounter < 15) {
            player.lostLifeCounter++;
            Copy(Sprites3X, PlayerCoordinates.I(1, 9), PlayerCoordinates.I(2, 9), PlayerCoordinates.I(3, 9), PlayerCoordinates.I(4, 9), PlayerMatrix);
            PasteNon0(PlayerMatrix, player.currentTile.centerX - 30, player.currentTile.centerY - 80, screenMatrix);
        }
        else {
            Game::Resume();
            player.lostLifeCounter = 0; // Reset counter
            player.showLostLifeText = false;
        }
    }
}

ICBYTES EnemyCoordinates{
    { 54, 66, 39, 30},  // Red Ball
    { 6, 72, 39, 24 },  // Red Ball Bounce
    { 243, 57, 48, 39 }, // Snake Egg
    { 192, 66, 48, 30 }, // Snake Egg Bounce
    { 6, 144, 42, 48 },  // Snake up move
    { 51, 99, 45, 93 }, // Snake up jump
    { 96, 144, 42, 48 }, // Snake left move
    { 147, 99, 45, 93 }, // Snake left jump
    { 198, 144, 42, 48 },  // Snake right move
    { 243, 99, 45, 93 }, // Snake right jump
    { 288, 144, 42, 48},  // Snake down move
    { 339, 99, 45, 93 }  // Snake down jump
};

void DrawRedBalls() {
    if (enemyBall1.isAlive) {
        int i = enemyBall1.state;
        Copy(Sprites3X, EnemyCoordinates.I(1, i), EnemyCoordinates.I(2, i),
            EnemyCoordinates.I(3, i), EnemyCoordinates.I(4, i),
            Enemy1Matrix);
        int printx = enemyBall1.x;
        int printy = enemyBall1.y;

        if (i == 1 || i == 2) {
            printy += 20;
        }
        PasteNon0(Enemy1Matrix, printx, printy, screenMatrix);
    }

    if (enemyBall2.isAlive) {
        int i = enemyBall2.state;
        Copy(Sprites3X, EnemyCoordinates.I(1, i), EnemyCoordinates.I(2, i),
            EnemyCoordinates.I(3, i), EnemyCoordinates.I(4, i),
            Enemy2Matrix);
        int printx = enemyBall2.x;
        int printy = enemyBall2.y;

        if (i == 1 || i == 2) {
            printy += 20;
        }
        PasteNon0(Enemy2Matrix, printx, printy, screenMatrix);
    }
}

void DrawSnake() {
    if (enemySnake.isAlive) {
        int i = enemySnake.state;
        Copy(Sprites3X, EnemyCoordinates.I(1, i), EnemyCoordinates.I(2, i),
            EnemyCoordinates.I(3, i), EnemyCoordinates.I(4, i),
            Enemy2Matrix);
        int printx = enemySnake.x;
        int printy = enemySnake.y;

        if (i == 3 || i == 4) {
            printy += 15;
        }
        else if (i == 6 || i == 8 || i == 10 || i == 12) {
            printy -= 45;
        }
        PasteNon0(Enemy2Matrix, printx, printy, screenMatrix);
    }
}
