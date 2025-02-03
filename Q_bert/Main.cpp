#include "icb_gui.h"
#include <map>
#include <string>

// Globals
const int GRID_SIZE = 7; // Size of the Q*bert pyramid
bool gameRunning = false;
HANDLE renderMutex;
ICBYTES screenMatrix, Sprites, Sprites3X;
ICBYTES CurrentTileMatrix, PlayerMatrix, EnemyMatrix, DiscMatrix;
int FRM1;
int keypressed;
int score = 0; // Global score variable

// Class Definitions
class SquareBlock {
public:
    int x;
    int y;
    int blk_clr_state;
    int up;
    int down;
    int rigth;
    int left;
};

class Disc {
public:
    int x;
    int y;
    int block_id;
    bool show_state;
    bool move_state;
};

Disc Discs[2];
SquareBlock SquareBlocks[28];

ICBYTES IntroCoordinates{
    { 387, 99, 282, 45},     // QBERT
    { 816, 121, 27, 24},    //  Pink c
    { 435, 6, 21, 21},      // 2
    { 387, 6, 21, 21},      //  0
    { 507, 6, 21, 21},    // 5
    { 483, 27, 21, 21},     // E
    { 819, 27, 21, 21},    //  S
    { 459, 27, 21, 21},      // D
    { 843, 27, 21, 21},      //  T
    { 867, 27, 21, 21},    // U
    { 579, 27, 15, 21},      // I
    { 627, 3, 21, 21},      //  O
    { 387, 27, 21, 21},    //  A
    { 651, 27, 21, 21},      // L
    { 795, 27, 21, 21},      //  R
    { 531, 27, 21, 21},    // G
    { 555, 27, 21, 21},      // H
    { 891, 27, 21, 21},      //  V
    { 414, 6, 21, 21},    //  1
    { 435, 27, 21, 21},      // C
    { 699, 27, 21, 21},      //  N
    { 747, 27, 21, 21},    // P
    { 963, 27, 21, 21},      // Y
    { 846, 129, 15, 15},      //  =
    { 1149, 240, 84, 96},      //  Flying Qbert

};

struct CharData {
    int spriteIndex;
    const char* text;
};

const std::map<char, int> CHAR_INDICES = {
    {'E', 6}, {'S', 7}, {'D', 8}, {'T', 9},
    {'U', 10}, {'I', 11}, {'O', 12}, {'A', 13},
    {'L', 14}, {'R', 15}, {'G', 16}, {'H', 17},
    {'V', 18}, {'C', 20}, {'N', 21}, {'P', 22},
    {'Y', 23}, {'=', 24}, {'2', 3}, {'0', 4},
    {'5', 5}, {'1', 19}
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

void RenderString(ICBYTES& screen, const char* text, int x, int y, int spacing = 25) {
    int currentX = x;
    while (*text) {
        if (*text != ' ') {
            RenderChar(screen, *text, currentX, y);
        }
        currentX += spacing;
        text++;
    }
}

void DrawStartupAnimation(bool* gameRunningPtr) {
    ICBYTES startScreen;
    CreateImage(startScreen, 700, 700, ICB_UINT);
    MSG msg;

    for (int frame = 0; frame < 180 && *gameRunningPtr; frame++) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                *gameRunningPtr = false;
                return;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        startScreen = 0x1A0F5F;

        ICG_SetFont(24, 0, "Terminal");
        Impress12x20(startScreen, 500, 20, "CREDITS", 0x00FF00);

        if (frame > 30) {
            char creditNum[2] = { '0' + min(9, (frame - 30) / 15), '\0' };
            Impress12x20(startScreen, 600, 20, creditNum, 0xFF6600);
        }

        if (frame > 60) {
            ICBYTES QbertTitle;
            Copy(Sprites3X,
                IntroCoordinates.I(1, 1),
                IntroCoordinates.I(2, 1),
                IntroCoordinates.I(3, 1),
                IntroCoordinates.I(4, 1),
                QbertTitle);
            int yOffset = (frame % 16 > 8) ? 2 : -2;
            PasteNon0(QbertTitle, 220, 200 + yOffset, startScreen);

            ICG_SetFont(20, 0, "Arial");
            Impress12x20(startScreen, 520, 190 + yOffset, "TM", 0xFFD700);
        }

        if (frame > 90) {
            ICBYTES PinkC;
            Copy(Sprites3X,
                IntroCoordinates.I(1, 2),
                IntroCoordinates.I(2, 2),
                IntroCoordinates.I(3, 2),
                IntroCoordinates.I(4, 2),
                PinkC);
            PasteNon0(PinkC, 100, 280, startScreen);

            RenderString(startScreen, "2025", 130, 283);
            RenderString(startScreen, "ESD STUDIOS", 280, 280);
            RenderString(startScreen, "ALL RIGHTS RESERVED", 130, 320);
        }

        if (frame > 120) {
            RenderString(startScreen, "1 COIN = 1 PLAY", 150, 500);

            ICBYTES QB;
            Copy(Sprites3X,
                IntroCoordinates.I(1, 25),
                IntroCoordinates.I(2, 25),
                IntroCoordinates.I(3, 25),
                IntroCoordinates.I(4, 25),
                QB);
            PasteNon0(QB, 350, 600, startScreen);
        }

        DisplayImage(FRM1, startScreen);
        Sleep(33);
    }
}


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

    Copy(Sprites3X, 2, 224 * 3 + 1, 32 * 3, 32 * 3, CurrentTileMatrix);
}

void CreateDisc() {
    SquareBlocks[10].left = 40; //left
    Discs[0] = { SquareBlocks[10].x - 15,SquareBlocks[10].y - 40,10,true,false }; //x,y,block_id,state
    SquareBlocks[14].up = 45; //rigth
    Discs[1] = { SquareBlocks[14].x+110,SquareBlocks[14].y-40,14,true,false }; //x,y,block_id,show_state,move_state
}


class Player {
public:
    int x, y, location; // Position on the pyramid
    int direction = 7;
    bool jumpStatus = false;
    bool willFall = false;
    bool mazeOrder = false; // Flag to indicate if the player is falling
    Player() : x(0), y(0) {}

    void BlockMoveAnimation(char key, int goal_x, int goal_y) {
        int br_x = 0, br_y = 0;

        Sleep(50);

        direction++;
        y -= 40;

        Sleep(50);

        x < goal_x ? br_x = 5 : br_x = -5;
        y < goal_y ? br_y = 5 : br_y = -5;
        while (x != goal_x || y != goal_y) {
            if (x != goal_x) {
                x += br_x;
            }
            if (y != goal_y) {
                y += br_y;
            }
            Sleep(15);
        }

        direction--;
        x = goal_x;
        y = goal_y;
        Sleep(50);

        if (SquareBlocks[location].blk_clr_state == 0 && jumpStatus) {
            SquareBlocks[location].blk_clr_state = 1;
            jumpStatus = !jumpStatus;
            score += 25; // Update score when tile color is changed
        }
    }

    void FallOffEdge(char key) {
        willFall = false;
        // Calculate the total change in x
        int x_change = key == 'l' ? -5 : key == 'r' ? 5 : key == 'u' ? 5 : key == 'd' ? -5 : 0;
        int y_change = key == 'l' ? -5 : key == 'r' ? -5 : key == 'u' ? -5 : key == 'd' ? -5 : 0;
        // Falling animation
        for (int i = 0; i < 40; ++i) {
            if (i < 10) {
                y += y_change;
                x += x_change;
            }
            else {
                if(key == 'l' || key == 'u')
                    mazeOrder = true; // Set the falling flag
                y += 5;
            }
            Sleep(15);
        }
        // Reset player position to the top of the pyramid
        x = 320;
        y = 90;
        location = 0;
        direction = 7;
        mazeOrder = false; // Reset the falling flag
    }

    void move(char key) {
        int block_id = 0;
        jumpStatus = true;
        switch (key)
        {
        case 'l':
            direction = 3;
            if (SquareBlocks[location].left >= 0) {
                //Also checks disk is available to jump
                SquareBlocks[location].left == 40 ? block_id = location : location = SquareBlocks[location].left;
            }
            else {
                jumpStatus = false;
                willFall = true;
            }
            break;
        case 'r':
            direction = 5;
            if (SquareBlocks[location].rigth >= 0) {
                location = SquareBlocks[location].rigth;
            }
            else {
                jumpStatus = false;
                willFall = true;
            }
            break;
        case 'd':
            direction = 7;
            if (SquareBlocks[location].down >= 0) {
                location = SquareBlocks[location].down;
            }
            else {
                jumpStatus = false;
                willFall = true;
            }
            break;
        case 'u':
            direction = 1;
            if (SquareBlocks[location].up >= 0) {
                //Also checks disk is available to jump
                SquareBlocks[location].up == 45 ? block_id = location : location = SquareBlocks[location].up;
            }
            else {
                jumpStatus = false;
                willFall = true;
            }
            break;
        default:
            break;
        }
        keypressed = 0;
        if (willFall) {
            FallOffEdge(key);
            return;
        }

        if (jumpStatus) {
            if (block_id != 0) {
                for (int i = 0; i < 2; i++) {
                    if (Discs[i].block_id == block_id) {
                        BlockMoveAnimation(key, Discs[i].x - 20, Discs[i].y - 20);
                    }
                }

            }
            else {
                BlockMoveAnimation(key, SquareBlocks[location].x + 20, SquareBlocks[location].y - 10);
            }
        }
        else {
            BlockMoveAnimation(key, x + (key == 'l' ? -20 : key == 'r' ? 20 : key == 'w' ? -20 : key == 's' ? 20 : 0), y + (key == 'u' ? -10 : key == 'd' ? 10 : 0));
        }

    }
};

Player player; // Global player

//struct SquareBlock {
//    int x;
//    int y;
//    int blk_clr_state;
//    int up;
//    int down;
//    int rigth;
//    int left;
//    
//
//};




// Max number of enemies
//const int MAX_ENEMIES = 5;

//class Enemy {
//public:
//    int x, y; // Position of the enemy
//    Enemy(int startX = 0, int startY = 0) : x(startX), y(startY) {}
//    void moveTowards(Player& player) {
//        if (player.x > x) x++;
//        else if (player.x < x) x--;
//        if (player.y > y) y++;
//        else if (player.y < y) y--;
//    }
//};
//Enemy enemies[MAX_ENEMIES] = { {60, 10}, {50, 10}, {40, 10}, {30, 10}, {20, 10} }; // Global enemies


// Create window
void ICGUI_Create() {
    ICG_MWTitle("Q_Bert");
    ICG_MWSize(740, 800);
}

void DrawMap() {
    // Blue Tile
    int temp = 0;

    // Center Coordinates
   /* int startX = 300;
    int startY = 100;
    int offsetX = 47;
    int offsetY = 70;
    int x;*/

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

    // Draw pyramid
    //for (int Innerlayer = 0; Innerlayer < 4; Innerlayer++) {
    //    int baseY = startY + Innerlayer * 2 * offsetY;
    //    for (int Outerlayer = 0; Innerlayer * 2 + Outerlayer < GRID_SIZE; Outerlayer++) { // 7 Outerlayer
    //        int y = baseY + Outerlayer * offsetY;
    //       
    //        // Left side
    //        x = startX - Outerlayer * offsetX;
    //        PasteNon0(CurrentTileMatrix, x, y, screenMatrix);

    //        //// Right side
    //        x = startX + Outerlayer * offsetX;
    //        PasteNon0(CurrentTileMatrix, x, y, screenMatrix);
    //    }
    //}

}

void DrawDisc() {
    for (int i = 0; i < 2; i++) {
        if (Discs[i].show_state) {
            FillCircle(screenMatrix, Discs[i].x, Discs[i].y, 25, 0x0FFFFFF);
        }
    }
}

ICBYTES Coordinates{
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
    Copy(Sprites3X, Coordinates.I(1, i), Coordinates.I(2, i),
        Coordinates.I(3, i), Coordinates.I(4, i),
        PlayerMatrix);

    PasteNon0(PlayerMatrix, player.x, player.y, screenMatrix);
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

void DrawEnemies() {
    //for (int i = 0; i < max_enemies; ++i) {
    //    fillrect(screenmatrix, enemies[i].x, enemies[i].y, 10, 10, 0x00ff00);
    //}
}
// Function Definitions
void renderGrid() {
    screenMatrix = 0;

    if (player.mazeOrder) {
        // Draw player first if falling
        DrawPlayer();
    }

    // Draw map
    DrawMap();

    // Draw Disc()
    DrawDisc();

    // Draw score
    DrawScore();

    // Draw enemies
    DrawEnemies();

    if (!player.mazeOrder) {
        // Draw player after map if not falling
        DrawPlayer();
    }

    DisplayImage(FRM1, screenMatrix);
    Sleep(30);
}

VOID* renderThread() {
    while (gameRunning) {
        renderGrid();
    }
    return NULL;
}

VOID* gameLogicThread() {
    while (gameRunning) {

        if (keypressed == 37) player.move('l');
        else if (keypressed == 39) player.move('r');
        else if (keypressed == 38) player.move('u');
        else if (keypressed == 40) player.move('d');
        else if (keypressed == 'p') gameRunning = false; // Pause game

        //for (int i = 0; i < MAX_ENEMIES; ++i) {
        //    enemies[i].moveTowards(player);
        //}

        //Sleep(100);


    }
    return NULL;
}

void StartGame() {
    SetFocus(ICG_GetMainWindow());

    if (gameRunning) return;
    gameRunning = true;

    //DrawStartupAnimation(&gameRunning);

    // Reset the screen
    screenMatrix = 0;
    player.x = 320;
    player.y = 90;
    player.location = 0;
    score = 0; // Reset score

    //Create SquareBlock Pyramid
    PyramidMatrix();

    //Create Disc
    CreateDisc();


    // Threads
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)gameLogicThread, NULL, 0, NULL);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)renderThread, NULL, 0, NULL);

}

void WhenKeyPressed(int k) {
    keypressed = k;
}

void ICGUI_main() {
    ICG_Button(5, 5, 120, 25, "START GAME", StartGame);
    FRM1 = ICG_FrameMedium(5, 40, 1, 1);
    ICG_SetOnKeyPressed(WhenKeyPressed);
    CreateImage(screenMatrix, 700, 700, ICB_UINT);

    ReadImage("Assests/sprites.bmp", Sprites);
    MagnifyX3(Sprites, Sprites3X);
}
