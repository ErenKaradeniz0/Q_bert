#include "icb_gui.h"

// Globals
const int GRID_SIZE = 7; // Size of the Q*bert pyramid
bool gameRunning = true;
HANDLE renderMutex;
ICBYTES screenMatrix, Sprites, Sprites3X;
ICBYTES CurrentTileMatrix, PlayerMatrix, EnemyMatrix;
int FRM1;
int keypressed;

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
    Discs[0] = { SquareBlocks[10].x-15,SquareBlocks[10].y-40,10,true,false }; //x,y,block_id,state
    SquareBlocks[14].up = 45; //rigth
    Discs[1] = { SquareBlocks[14].x+111,SquareBlocks[14].y-40,14,true,false }; //x,y,block_id,show_state,move_state
}


class Player {
public:
    int x, y, location; // Position on the pyramid
    int direction = 7;
    bool spacejump=false;
    Player() : x(0), y(0) {}

    void BlockMoveAnimation(char key, int goal_x, int goal_y) {
        int br_x = 0, br_y = 0;

        Sleep(50);

        direction++;
        y -= 40;
        x < goal_x ? br_x = 5 : br_x = -5;
        y < goal_y ? br_y = 5 : br_y = -5;

        Sleep(50);
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

        if (SquareBlocks[location].blk_clr_state == 0 && spacejump) {
            SquareBlocks[location].blk_clr_state = 1;
            spacejump = !spacejump;
        }
    }


    // OutSide Map Animation (Drop Map) (Death) --> Will Write Function

    void move(char key) {
        int block_id = 0;
        spacejump = true;
        switch (key)
        {
        case 'l':
            if (SquareBlocks[location].left >= 0) {
                SquareBlocks[location].left == 40 ? block_id=location : location = SquareBlocks[location].left;
                direction = 3;
            }
            else {
                spacejump = false;
            }
            break;
        case 'r':
            if (SquareBlocks[location].rigth >= 0) {
               location = SquareBlocks[location].rigth;
               direction = 5;
            }
            else {
                spacejump = false;
            }
            break;
        case 'd':
            if (SquareBlocks[location].down >= 0) {
                location = SquareBlocks[location].down;
                direction = 7;
            }
            else {
                spacejump = false;
            }
            break;
        case 'u':
            if (SquareBlocks[location].up >= 0) {
                SquareBlocks[location].up == 45 ? block_id = location : location = location = SquareBlocks[location].up;
                direction = 1;
            }
            else {
                spacejump = false;
            }
            break;
        default:
            break;
        }
        if (spacejump) {
            if (block_id!=0) {
                for (int i = 0; i < 2; i++) {
                    if (Discs[i].block_id == block_id) {
                        BlockMoveAnimation(key, Discs[i].x-20, Discs[i].y-20);
                    }
                }
                
            }
            else {
                BlockMoveAnimation(key, SquareBlocks[location].x + 20, SquareBlocks[location].y - 10);
            }
        }
        
        keypressed = 0;
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
    int temp=0;

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



void DrawEnemies() {
    //for (int i = 0; i < max_enemies; ++i) {
    //    fillrect(screenmatrix, enemies[i].x, enemies[i].y, 10, 10, 0x00ff00);
    //}
}
// Function Definitions
void renderGrid() {
    screenMatrix = 0;

    // Draw map
    DrawMap();

    // Draw Disc()
    DrawDisc();


    // Draw player
    DrawPlayer();

    // Draw enemies
    DrawEnemies();

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

        if (keypressed == 37 ) player.move('l');
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

    // Reset the screen
    screenMatrix = 0;
    player.x = 320;
    player.y = 90;
    player.location = 0;
    
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
