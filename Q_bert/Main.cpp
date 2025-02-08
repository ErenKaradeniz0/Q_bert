#include "icb_gui.h"
#include "PrintHelper.h"
#include "intro.h"
#include "Player.h"
#include "Enemy.h"
#include "Maze.h"
#include "Main.h"

// Globals
bool gameRunning = false;
bool gamePaused = false;
HANDLE stopThreads = CreateEvent(NULL, TRUE, FALSE, NULL); // Event to signal threads to stop
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

void InterruptableSleep(int ms)
{
	WaitForSingleObject(stopThreads, ms);
}
bool Continue()
{
	return WaitForSingleObject(stopThreads, 0) == WAIT_TIMEOUT;
}


DWORD WINAPI renderThread(LPVOID lpParam) {
    while (gameRunning) {
        screenMatrix = 0; // Clear the screen

        if (!Continue()) break;

        if (player.mazeOrder) {
            // Draw player first if falling
            DrawPlayer();
        }

        if (!Continue()) break;

        // Draw map
        DrawMap();

        if (!Continue()) break;

        // Draw Disc()
        DrawDisc();

        if (!Continue()) break;

        // Draw score
        DrawScore();

        if (!Continue()) break;

        // Draw Qbert logo
        DrawLogo();

        if (!Continue()) break;

        // Draw lives
        DrawLives();

        if (!Continue()) break;

        // Draw enemies
        DrawEnemies();

        if (!Continue()) break;

        if (!player.mazeOrder) {
            // Draw player after map if not falling
            DrawPlayer();
        }

        if (gamePaused) {
            DrawPaused();
        }

        DisplayImage(FRM1, screenMatrix);
        InterruptableSleep(30);
    }
    return 0;
}

DWORD WINAPI InputThread(LPVOID lpParam) {
    while (Continue()) {
        if (keyPressedControl) {
            if (keypressed == 'P' || keypressed == 'p') {
                gamePaused = !gamePaused; // Toggle pause state
                keypressed = 0; // Reset keypress
                InterruptableSleep(200); // Prevent rapid re-press
                continue;
            }

            if (!gamePaused) { // Only move if the game is not paused
                if (keypressed == 37) player.move('l');
                else if (keypressed == 39) player.move('r');
                else if (keypressed == 38) player.move('u');
                else if (keypressed == 40) player.move('d');
            }
        }
    }
    return 0;
}

DWORD WINAPI turnDiscThread(LPVOID lpParam) {
    int k = 0;
    while (gameRunning && Continue()) {
        SelectEffectDisc(k);
        k++;
        if (k == 4)
            k = 0;
        InterruptableSleep(50);
    }
    return 0;
}

DWORD WINAPI EnemyBall1Thread(LPVOID lpParam) {
    while (gameRunning && Continue()) {
        InterruptableSleep(200);
        if (enemyBall1.isAlive)
            enemyBall1.move();
        else {
            enemyBall1.Spawn(false, 1);
        }
    }
    return 0;
}

DWORD WINAPI EnemyBall2Thread(LPVOID lpParam) {
    InterruptableSleep(3000);
    while (gameRunning && Continue()) {
        InterruptableSleep(200);
        if (enemyBall2.isAlive)
            enemyBall2.move();
        else {
            enemyBall2.Spawn(false, 1);
        }
    }
    return 0;
}

DWORD WINAPI EnemySnakeThread(LPVOID lpParam) {
    InterruptableSleep(1000);
    while (gameRunning && Continue()) {
        InterruptableSleep(200);
        if (enemySnake.isAlive)
            enemySnake.move();
        else {
            enemySnake.Spawn(false, 3);
        }
    }
    return 0;
}

DWORD WINAPI SoundThread(LPVOID lpParam) {
    while (gameRunning && Continue()) {
        //PlaySound
    }
    return 0;
}

void StopGame() {
	SetEvent(stopThreads); // Thread'lere durma sinyali gönder

    HANDLE threadHandles[] = {
        renderThreadHandle,
        inputThreadHandle,
        turnDiscThreadHandle,
        enemy1ThreadHandle,
        enemy2ThreadHandle,
        enemySnakeThreadHandle,
        soundThreadHandle
    };

    // Threadlerin gerçekten bittiğini doğrula (5 saniye timeout)
    WaitForMultipleObjects(7, threadHandles, TRUE, INFINITE);

    //stopThreads = false; // Reset stop flag
    keypressed = 0; // Reset keypressed
}

void StartGame() {
    SetFocus(ICG_GetMainWindow());
    if (gameRunning) {
		StopGame();
    }
	ResetEvent(stopThreads); // Reset the stop signal
    gameRunning = true;

    keyPressedControl = true;

    //DrawStartupAnimation1(&gameRunning);

    // Reset the screen
    screenMatrix = 0;

    // Create Pyramid
    PyramidMatrix();

    // Create Disc
    CreateDisc();

    CreatePlayer();

    // Yeni thread'leri oluştur
    inputThreadHandle = CreateThread(NULL, 0, InputThread, NULL, 0, NULL);
    turnDiscThreadHandle = CreateThread(NULL, 0, turnDiscThread, NULL, 0, NULL);
    renderThreadHandle = CreateThread(NULL, 0, renderThread, NULL, 0, NULL);
    enemy1ThreadHandle = CreateThread(NULL, 0, EnemyBall1Thread, NULL, 0, NULL);
    enemy2ThreadHandle = CreateThread(NULL, 0, EnemyBall2Thread, NULL, 0, NULL);
    enemySnakeThreadHandle = CreateThread(NULL, 0, EnemySnakeThread, NULL, 0, NULL);
    soundThreadHandle = CreateThread(NULL, 0, SoundThread, NULL, 0, NULL);
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
