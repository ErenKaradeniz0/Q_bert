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

void DrawPaused() {
    static ICBYTES letterSprite;
    static bool initialized = false;
    static bool showText = true;
    static int blinkTimer = 0;
    const int BLINK_SPEED = 500;

    // One-time initialization
    if (!initialized) {
        CreateImage(letterSprite, 21, 21, ICB_UINT);
        initialized = true;
    }

    // Helper function to draw a single character
    auto DrawCharacter = [&](char c, int x, int y) {
        int spriteIndex = 0;

        if (c >= 'A' && c <= 'Z') {
            spriteIndex = (c - 'A') + 11;  // Letters start at index 11
        }
        else {
            return;
        }

        Copy(Sprites3X,
            IntroCoordinates.I(1, spriteIndex),
            IntroCoordinates.I(2, spriteIndex),
            IntroCoordinates.I(3, spriteIndex),
            IntroCoordinates.I(4, spriteIndex),
            letterSprite);

        PasteNon0(letterSprite, x, y, screenMatrix);
        };

    FillRect(screenMatrix, 200, 350, 275, 75, 0x000000);

    if (GetTickCount() - blinkTimer > BLINK_SPEED) {
        showText = !showText;
        blinkTimer = GetTickCount();
    }


    if (showText) {
        const char* pausedText = "PAUSED";
        int currentX = 265;
        for (int i = 0; pausedText[i] != '\0'; i++) {
            DrawCharacter(pausedText[i], currentX, 375);
            currentX += 25;
        }
    }

    DisplayImage(FRM1, screenMatrix);
}


void DrawScore() {
    static ICBYTES letterSprite;
    static bool initialized = false;

    // One-time initialization
    if (!initialized) {
        CreateImage(letterSprite, 21, 21, ICB_UINT);
        initialized = true;
    }

    // Helper function to draw a single character
    auto DrawCharacter = [&](char c, int x, int y) {
        int spriteIndex = 0;

        if (c >= '0' && c <= '9') {
            spriteIndex = (c - '0') + 1;  // Numbers start at index 1
        }
        else if (c >= 'A' && c <= 'Z') {
            spriteIndex = (c - 'A') + 11;  // Letters start at index 11
        }
        else {
            return;
        }

        Copy(Sprites3X,
            IntroCoordinates.I(1, spriteIndex),
            IntroCoordinates.I(2, spriteIndex),
            IntroCoordinates.I(3, spriteIndex),
            IntroCoordinates.I(4, spriteIndex),
            letterSprite);

        PasteNon0(letterSprite, x, y, screenMatrix);
        };

    // Draw "SCORE" text
    const char* scoreText = "SCORE";
    int currentX = 10;
    for (int i = 0; scoreText[i] != '\0'; i++) {
        DrawCharacter(scoreText[i], currentX, 10);
        currentX += 25;  // Space between letters
    }

    // Convert score to string and draw each digit
    currentX += 25;  // Extra space between "SCORE" and number

    // Handle case when score is 0
    if (score == 0) {
        DrawCharacter('0', currentX, 10);
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
        DrawCharacter(digits[i], currentX, 10);
        currentX += 25;
    }
}

void DrawLives() {
    static ICBYTES miniQbert;
    static bool initialized = false;

    // One-time initialization of mini Q*bert sprite
    if (!initialized) {
        CreateImage(miniQbert, IntroCoordinates.I(3, 52), IntroCoordinates.I(4, 52), ICB_UINT);
        initialized = true;
    }

    // First draw "LIVES" text using sprite letters
    static ICBYTES letterSprite;
    if (!initialized) {
        CreateImage(letterSprite, 21, 21, ICB_UINT);
    }

    // Draw "LIVES" text
    const char* livesText = "LIVES";
    int currentX = 10;
    for (int i = 0; livesText[i] != '\0'; i++) {
        int spriteIndex = (livesText[i] - 'A') + 11;  // Convert letter to sprite index

        Copy(Sprites3X,
            IntroCoordinates.I(1, spriteIndex),
            IntroCoordinates.I(2, spriteIndex),
            IntroCoordinates.I(3, spriteIndex),
            IntroCoordinates.I(4, spriteIndex),
            letterSprite);

        PasteNon0(letterSprite, currentX, 40, screenMatrix);
        currentX += 25;
    }

    // Then draw mini Q*berts for each life
    Copy(Sprites3X,
        IntroCoordinates.I(1, 52),
        IntroCoordinates.I(2, 52),
        IntroCoordinates.I(3, 52),
        IntroCoordinates.I(4, 52),
        miniQbert);

    // Draw mini Q*berts vertically, starting under the "LIVES" text
    const int MINI_QBERT_SPACING = 35;  // Vertical spacing between Q*berts
    const int START_Y = 70;  // Starting Y position (below "LIVES" text)

    for (int i = 0; i < player.lifes; i++) {
        PasteNon0(miniQbert, 20, START_Y + (i * MINI_QBERT_SPACING), screenMatrix);
    }
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


void SelectEffectDisc(int k) {
    switch (k)
    {
    case 0: { Copy(Sprites3X, 1, 356 * 3 + 1, 16 * 3, 10 * 3, DiscMatrix); break; }
    case 1: { Copy(Sprites3X, 16 * 3 + 1, 356 * 3 + 1, 16 * 3, 10 * 3, DiscMatrix); break; }
    case 2: { Copy(Sprites3X, 32 * 3 + 1, 356 * 3 + 1, 16 * 3, 10 * 3, DiscMatrix); break; }
    case 3: { Copy(Sprites3X, 48 * 3 + 1, 356 * 3 + 1, 16 * 3, 10 * 3, DiscMatrix); break; }
    default:
        break;
    }
};

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
    { 51, 99, 45, 93 }, // Snake up jump
    { 96, 144, 42, 48 }, // Snake left move
    { 147, 99, 45, 93 }, // Snake left jump
    { 198, 144, 42, 48 },  // Snake right move
    { 243, 99, 45, 93 }, // Snake right jump
    { 288, 144, 42, 48},  // Snake down move
    { 339, 99, 45, 93 }  // Snake down jump
};

void DrawEnemies() {
    if (enemyBall1.isAlive == true){
        int i = enemyBall1.state;

        Copy(Sprites3X, EnemyCoordinates.I(1, i), EnemyCoordinates.I(2, i),
            EnemyCoordinates.I(3, i), EnemyCoordinates.I(4, i),
            Enemy1Matrix);
		int printx = enemyBall1.x;
		int printy = enemyBall1.y;

        switch (i)
        {
		case 1:

		case 2:
            printy += 20;
        default:
            break;
        }
        PasteNon0(Enemy1Matrix, printx, printy, screenMatrix);
    }

    if(enemyBall2.isAlive == true){
        int i = enemyBall2.state;

        Copy(Sprites3X, EnemyCoordinates.I(1, i), EnemyCoordinates.I(2, i),
            EnemyCoordinates.I(3, i), EnemyCoordinates.I(4, i),
            Enemy2Matrix);

        int printx = enemyBall2.x;
        int printy = enemyBall2.y;

        switch (i)
        {
        case 1:

        case 2:
            printy += 20;
        default:
            break;

        }
        PasteNon0(Enemy2Matrix, printx, printy, screenMatrix);

    }
    if (enemySnake.isAlive == true) {
        int i = enemySnake.state;

        Copy(Sprites3X, EnemyCoordinates.I(1, i), EnemyCoordinates.I(2, i),
            EnemyCoordinates.I(3, i), EnemyCoordinates.I(4, i),
            Enemy2Matrix);

        int printx = enemySnake.x;
        int printy = enemySnake.y;

        switch (i) {
        case 3:
        case 4:
            printy += 15;
            break;
        case 6:
        case 8:
        case 10:
        case 12:
            printy -= 45;
            break;
        default:
            break;
        }

        PasteNon0(Enemy2Matrix, printx, printy, screenMatrix);

    }

}