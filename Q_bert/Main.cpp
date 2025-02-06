#include "icb_gui.h"
#include "PrintHelper.h"
#include "intro.h"
#include "Player.h"
#include "Enemy.h"
#include "Maze.h"

// Globals
bool gameRunning = false;
HANDLE renderMutex;
ICBYTES screenMatrix, Sprites, Sprites3X;
int FRM1;
int keypressed;
int score = 0; // Global score variable

Player player; // Global player
Enemy enemy1; // Global enemy 1
Enemy enemy2; // Global enemy 2

// Create window
void ICGUI_Create() {
    ICG_MWTitle("Q_Bert");
    ICG_MWSize(740, 800);
}

// Function Definitions
void renderGrid() {
    if (gameRunning) {
        screenMatrix = 0; // Clear the screen
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

        // Draw lives
        DrawLives();

        // Draw enemies
        DrawEnemies();

        if (!player.mazeOrder) {
            // Draw player after map if not falling
            DrawPlayer();
        }
    }

    DisplayImage(FRM1, screenMatrix);
    Sleep(30);
}

VOID* turnDiscThread() {
    int k = 0;
    while (gameRunning) {
        SelectEffectDisc(k);
        k++;
        if (k == 4)
            k = 0;
        Sleep(50);
    }

    return NULL;
}

VOID* renderThread() {
    while (gameRunning) {
        renderGrid();
    }
    return NULL;
}

VOID* InputThread() {
    while (gameRunning) {
        if (keypressed == 37) player.move('l');
        else if (keypressed == 39) player.move('r');
        else if (keypressed == 38) player.move('u');
        else if (keypressed == 40) player.move('d');
        else if (keypressed == 'p') gameRunning = false; // Pause game
    }
    return NULL;
}

VOID* Enemy1Thread() {
    enemy1.Spawn(280, 0, 1, 1);
    while (gameRunning) {

    }
    return NULL;
}

VOID* Enemy2Thread() {
	Sleep(3000);
    enemy2.Spawn(375, 0, 2, 3);
    while (gameRunning) {

    }
    return NULL;
}

VOID* SoundThread() {
    while (gameRunning) {
		//PlaySound
    }
    return NULL;
}

void StartGame() {
    SetFocus(ICG_GetMainWindow());

    if (gameRunning) return;
    gameRunning = true;

    //DrawStartupAnimation1(&gameRunning);

    // Reset the screen
    screenMatrix = 0;

    CreatePlayer();

    //Create Pyramid
    PyramidMatrix();

    //Create Disc
    CreateDisc();

    // Threads
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)InputThread, NULL, 0, NULL);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Enemy1Thread, NULL, 0, NULL);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Enemy2Thread, NULL, 0, NULL);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)turnDiscThread, NULL, 0, NULL);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SoundThread, NULL, 0, NULL);
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
