#include "icb_gui.h"

#include "Player.h"
#include "Enemy.h"
#include "Maze.h"
#include "Sound.h"
#include "Main.h"

#include "Game.h"
#include "GameSession.h"
// Globals
bool gamePaused = false;
HANDLE renderMutex;
int keypressed;
bool keyPressedControl;
int score = 0; // Global score variable

HANDLE inputThreadHandle = NULL;
HANDLE turnDiscThreadHandle = NULL;
HANDLE enemy1ThreadHandle = NULL;
HANDLE enemy2ThreadHandle = NULL;
HANDLE enemySnakeThreadHandle = NULL;
//HANDLE soundThreadHandle = NULL;

// Create window
void ICGUI_Create() {
    ICG_MWTitle("Q_Bert");
    ICG_MWSize(740, 800);
}

bool paused = false;
DWORD WINAPI InputThread(LPVOID lpParam) {
    while (Game::Run()) {
        if (keyPressedControl) {
			//Sleep(200); // Prevent rapid re-press
            if (Game::GetState() == Running) { // Only move if the game is not paused
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
    while (Game::Run()) {
        SelectEffectDisc(k);
        k++;
        if (k == 4)
            k = 0;
        Game::SleepI(50);
    }
    return 0;
}

DWORD WINAPI EnemyBall1Thread(LPVOID lpParam) {
    while (Game::Run()) {
        Game::SleepI(200);
        if (enemyBall1.isAlive)
            enemyBall1.move();
        else {
            enemyBall1.Spawn(false, 1);
        }
    }
    return 0;
}

DWORD WINAPI EnemyBall2Thread(LPVOID lpParam) {
    Game::SleepI(3000);
    while (Game::Run()) {
        Game::SleepI(200);
        if (enemyBall2.isAlive)
            enemyBall2.move();
        else {
            enemyBall2.Spawn(false, 1);
        }
    }
    return 0;
}

DWORD WINAPI EnemySnakeThread(LPVOID lpParam) {
    Game::SleepI(1000);
    while (Game::Run()) {
        Game::SleepI(200);
        if (enemySnake.isAlive)
            enemySnake.move();
        else {
            enemySnake.Spawn(false, 3);
        }
    }
    return 0;
}

//DWORD WINAPI SoundThread(LPVOID lpParam) {
//    
//    return 0;
//}

DWORD WINAPI GameControllerMain(LPVOID lpParam)
{
    GameSession* gameptr = new GameSession((int*)lpParam, 700, 700);

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
    enemy1ThreadHandle = CreateThread(NULL, 0, EnemyBall1Thread, NULL, 0, NULL);
    enemy2ThreadHandle = CreateThread(NULL, 0, EnemyBall2Thread, NULL, 0, NULL);
	enemySnakeThreadHandle = CreateThread(NULL, 0, EnemySnakeThread, NULL, 0, NULL);
    //soundThreadHandle = CreateThread(NULL, 0, SoundThread, NULL, 0, NULL);

    //Game::Run() freezes when Game::Pause() called
	while (Game::RunMain()) 
    {
        gameptr->Refresh(); // Refresh the screen
        if ((keypressed == 'P' || keypressed == 'p') && Game::GetState() == Running)
		{
			Game::Pause();
		}
		else if ((keypressed == 'R' || keypressed == 'r') && Game::GetState() == Paused)
		{
			Game::Resume();
		}
    }

    //Delete the game object
    delete gameptr;

    return 0;
}
void StartStopGame(void* FRM1_PTR) {
    SetFocus(ICG_GetMainWindow());

    GameState state = Game::GetState();

    if (state == Stopped)
        Game::Start(FRM1_PTR);
    else
        Game::Stop();
}

void WhenKeyPressed(int k) {
    keypressed = k;
}

void WhenKeyReleased(int k) {
	keypressed = 0;
}

void ICGUI_main() {
    FRM1 = ICG_FrameMedium(5, 40, 1, 1);
    int* FRM1_PTR = new int(FRM1);
    ICG_Button(5, 5, 150, 25, "START/STOP GAME", StartStopGame, FRM1_PTR);
    ICG_SetOnKeyPressed(WhenKeyPressed);
	ICG_SetOnKeyUp(WhenKeyReleased);
    CreateImage(screenMatrix, 700, 700, ICB_UINT);

    ReadImage("Assests/sprites.bmp", Sprites);
    MagnifyX3(Sprites, Sprites3X);
}
