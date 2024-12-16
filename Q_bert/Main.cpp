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
class Player {
public:
    int x, y; // Position on the pyramid
    Player() : x(0), y(0) {}
    void move(char key) {
        switch (key)
        {
        case 'l':
            x -= 5;
            y -= 5;
            break;
        case 'r':
            x += 5;
            y += 5;
            break;
        case 'd':
            x -= 5;
            y += 5;
            break;
        case 'u':
            x += 5;
            y -= 5;
            break;
        default:
            break;
        }
    }
};
Player player; // Global player

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
    Copy(Sprites3X, 2, 224 * 3 + 1, 32 * 3, 32 * 3, CurrentTileMatrix);

    // Center Coordinates
    int startX = 300;
    int startY = 100;
    int offsetX = 47;
    int offsetY = 70;
    int x;
    // Draw pyramid
    for (int Innerlayer = 0; Innerlayer < 4; Innerlayer++) {
        int baseY = startY + Innerlayer * 2 * offsetY;
        for (int Outerlayer = 0; Innerlayer * 2 + Outerlayer < GRID_SIZE; Outerlayer++) { // 7 Outerlayer
            int y = baseY + Outerlayer * offsetY;
           
            // Left side
            x = startX - Outerlayer * offsetX;
            PasteNon0(CurrentTileMatrix, x, y, screenMatrix);

            //// Right side
            x = startX + Outerlayer * offsetX;
            PasteNon0(CurrentTileMatrix, x, y, screenMatrix);
        }
    }
}

ICBYTES Coordinates{
    { 3, 6, 45, 45},     // 1
    { 51, 1, 45, 48 },
    { 97, 6, 45, 42 },
    { 192, 6, 45, 42 },
    { 240, 1, 45, 48 },
    { 291, 6, 45, 42 },
    { 339, 1, 45, 48 }
};
void DrawPlayer() {
    int i = 6;
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

        if (keypressed == 37) player.move('l');
        else if (keypressed == 39) player.move('r');
        else if (keypressed == 38) player.move('u');
        else if (keypressed == 40) player.move('d');
        else if (keypressed == 'p') gameRunning = false; // Pause game

        //for (int i = 0; i < MAX_ENEMIES; ++i) {
        //    enemies[i].moveTowards(player);
        //}

        Sleep(100);
    }
    return NULL;
}

void StartGame() {
    SetFocus(ICG_GetMainWindow());

    // Reset the screen
    screenMatrix = 0;
    player.x = 325;
    player.y = 90;

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
