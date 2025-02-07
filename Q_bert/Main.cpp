#include "icb_gui.h"
#include "PrintHelper.h"
#include "intro.h"
#include "Player.h"
#include "Enemy.h"
#include "Maze.h"


// Globals
bool gameRunning = false;
bool stopThreads = false; // Flag to signal threads to stop
HANDLE renderMutex;
ICBYTES screenMatrix, Sprites, Sprites3X;
int FRM1;
int keypressed;
bool keyPressedControl;
int score = 0; // Global score variable

Player player; // Global player
Enemy enemyBall1; // Global enemy 1
Enemy enemyBall2; // Global enemy 2
Enemy enemySnake; // Global enemy 2

HANDLE inputThreadHandle = NULL;
HANDLE turnDiscThreadHandle = NULL;
HANDLE enemy1ThreadHandle = NULL;
HANDLE enemy2ThreadHandle = NULL;
HANDLE enemySnakeThreadHandle = NULL;
HANDLE soundThreadHandle = NULL;
HANDLE renderThreadHandle = NULL;

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

        // Draw Qbert logo
        DrawLogo();

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

DWORD WINAPI turnDiscThread(LPVOID lpParam) {
    int k = 0;
    while (gameRunning) {
        SelectEffectDisc(k);
        k++;
        if (k == 4)
            k = 0;
        Sleep(50);
    }
    return 0;
}

DWORD WINAPI renderThread(LPVOID lpParam) {
    while (gameRunning && !stopThreads) {
        renderGrid();
    }
    return 0;
}

DWORD WINAPI InputThread(LPVOID lpParam) {
    while (gameRunning && !stopThreads) {
        if (keyPressedControl) {
            if (keypressed == 37) player.move('l');
            else if (keypressed == 39) player.move('r');
            else if (keypressed == 38) player.move('u');
            else if (keypressed == 40) player.move('d');
            else if (keypressed == 'p') gameRunning = false; // Pause game
        }
    }
    return 0;
}

DWORD WINAPI EnemyBall1Thread(LPVOID lpParam) {
    while (gameRunning && !stopThreads) {
        Sleep(200);
        if(enemyBall1.isAlive)
            enemyBall1.move();
        else {
            enemyBall1.Spawn(false, 1);
        }
    }
    return 0;
}

DWORD WINAPI EnemyBall2Thread(LPVOID lpParam) {
    Sleep(3000);
    while (gameRunning && !stopThreads) {
        Sleep(200);
        if (enemyBall2.isAlive)
            enemyBall2.move();
        else {
            enemyBall2.Spawn(false, 1);
        }
    }
    return 0;
}
DWORD WINAPI EnemySnakeThread(LPVOID lpParam) {
    Sleep(1000);
    while (gameRunning && !stopThreads) {
        Sleep(200);
        if (enemySnake.isAlive)
            enemySnake.move();
        else {
            enemySnake.Spawn(false, 3);
        }
      }
    return 0;
}

DWORD WINAPI SoundThread(LPVOID lpParam) {
    while (gameRunning && !stopThreads) {
        //PlaySound
    }
    return 0;
}

void StartGame() {
    SetFocus(ICG_GetMainWindow());

    if (gameRunning) {
        stopThreads = true; // Signal threads to stop
        WaitForSingleObject(inputThreadHandle, INFINITE);
        WaitForSingleObject(enemy1ThreadHandle, INFINITE);
        WaitForSingleObject(enemy2ThreadHandle, INFINITE);
        WaitForSingleObject(enemySnakeThreadHandle, INFINITE);
        WaitForSingleObject(soundThreadHandle, INFINITE);
        WaitForSingleObject(renderThreadHandle, INFINITE);
        stopThreads = false; // Reset the stop flag
		keypressed = 0; // Reset keypressed
    }

    gameRunning = true;
    keyPressedControl=true;

    //DrawStartupAnimation1(&gameRunning);

    // Reset the screen
    screenMatrix = 0;

    //Create Pyramid
    PyramidMatrix();

    //Create Disc
    CreateDisc();

    CreatePlayer();

    // Threads
    inputThreadHandle = CreateThread(NULL, 0, InputThread, NULL, 0, NULL);
    turnDiscThreadHandle = CreateThread(NULL, 0, turnDiscThread, NULL, 0, NULL);
    enemy1ThreadHandle = CreateThread(NULL, 0, EnemyBall1Thread, NULL, 0, NULL);
    enemy2ThreadHandle = CreateThread(NULL, 0, EnemyBall2Thread, NULL, 0, NULL);
	enemySnakeThreadHandle = CreateThread(NULL, 0, EnemySnakeThread, NULL, 0, NULL);
    soundThreadHandle = CreateThread(NULL, 0, SoundThread, NULL, 0, NULL);
    renderThreadHandle = CreateThread(NULL, 0, renderThread, NULL, 0, NULL);
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
