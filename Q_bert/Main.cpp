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
int keypressed;
bool keyPressedControl;
bool isAnimationFaster = false;
int score = 0; // Global score variable

// Create window
void ICGUI_Create() {
    ICG_MWTitle("Q_Bert");
    ICG_MWSize(740, 800);
}

DWORD WINAPI InputThread(LPVOID lpParam) {
    while (Game::Run()) {
        if (keyPressedControl) {
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

DWORD WINAPI SnakeSoundThread(LPVOID lpParam) {
    while (Game::Run()) {
        if (enemySnake.playSnakeJumpSound) {
            enemySnake.playSnakeJumpSound = false;
            SnakeJumpSound();
        }
        if (enemySnake.playSnakeFallSound) {
            enemySnake.playSnakeFallSound = false;
            SnakeFallSound();
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
            Sleep(3000);
            enemyBall1.Spawn(false, 1);
        }
    }
    return 0;
}

DWORD WINAPI EnemyBall2Thread(LPVOID lpParam) {
    while (Game::Run()) {
        Game::SleepI(200);
        if (enemyBall2.isAlive)
            enemyBall2.move();
        else {
            Sleep(6000);
            enemyBall2.Spawn(false, 1);
        }
    }
    return 0;
}

DWORD WINAPI EnemySnakeThread(LPVOID lpParam) {
    while (Game::Run()) {
        Game::SleepI(200);
        if (enemySnake.isAlive)
            enemySnake.move();
        else {
            Sleep(2000);
            enemySnake.Spawn(false, 3);
        }
    }
    return 0;
}

DWORD WINAPI playerSoundThread(LPVOID lpParam) {
    while (Game::Run()) {
        if (player.playJumpSound) {
            player.playJumpSound = false;
            PlayerJumpSound();
        }
        if (player.playFallingSound) {
            player.playFallingSound = false;
            PlayerFallSound();
        }
        if (player.playDiscSound) {
            player.playDiscSound = false;
            DiscSound();
        }
        if (player.playSpeechSound) {
            player.playSpeechSound = false;
            PlayerSpeechSound();
        }
    }
    return 0;
}

DWORD WINAPI enemyBall1SoundThread(LPVOID lpParam) {
    while (Game::Run()) {
        if (enemyBall1.playRedBallJumpSound) {
            enemyBall1.playRedBallJumpSound = false;
            RedBallJumpSound();
        }
    }
    return 0;
}

DWORD WINAPI enemyBall2SoundThread(LPVOID lpParam) {
    while (Game::Run()) {
        if (enemyBall2.playRedBallJumpSound) {
            enemyBall2.playRedBallJumpSound = false;
            RedBallJumpSound();
        }
    }
    return 0;
}

DWORD WINAPI GameControllerMain(LPVOID lpParam) {
    GameSession* gameptr = new GameSession((int*)lpParam, 700, 700);

    keyPressedControl = true;

    ResetSoundFlags();

    DrawStartupAnimation();

    // Reset the screen
    screenMatrix = 0;

    // Create Pyramid
    PyramidMatrix();

    // Create Disc
    CreateDisc();

    CreatePlayer();

    HANDLE threads[9];
    threads[0] = CreateThread(NULL, 0, InputThread, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, turnDiscThread, NULL, 0, NULL);
    threads[2] = CreateThread(NULL, 0, EnemyBall1Thread, NULL, 0, NULL);
    threads[3] = CreateThread(NULL, 0, EnemyBall2Thread, NULL, 0, NULL);
    threads[4] = CreateThread(NULL, 0, EnemySnakeThread, NULL, 0, NULL);

    // Sound Threads
    threads[5] = CreateThread(NULL, 0, playerSoundThread, NULL, 0, NULL);
    threads[6] = CreateThread(NULL, 0, enemyBall1SoundThread, NULL, 0, NULL);
    threads[7] = CreateThread(NULL, 0, enemyBall2SoundThread, NULL, 0, NULL);
    threads[8] = CreateThread(NULL, 0, SnakeSoundThread, NULL, 0, NULL);



    while (Game::RunMain()) {
        gameptr->Refresh(); // Refresh the screen
        if ((keypressed == 'P' || keypressed == 'p') && Game::GetState() == Running) {
            Game::Pause(true);
        }
        else if ((keypressed == 'R' || keypressed == 'r') && Game::GetState() == Paused) {
            Game::Resume();
        }
    }

	WaitForMultipleObjects(9, threads, TRUE, INFINITE);
    // Delete the game object
    delete gameptr;

	Game::CompleteStop();
    return 0;
}

void StartStopGame(void* FRM1_PTR) {
    SetFocus(ICG_GetMainWindow());

    GameState state = Game::GetState();

    if (state == Stopped && Game::IsStopped())
    {
        Game::Start(FRM1_PTR);
    }
	else if (state == Running)
    {
        Game::Stop();
    }
}

void WhenKeyPressed(int k) {
    keypressed = k;
}

void WhenKeyReleased(int k) {
    keypressed = 0;
}

void ToggleIntroAnimationSpeed(int state) {
    isAnimationFaster = (state != 0); // If state is 1 (checked), set true; otherwise, set false
    SetFocus(ICG_GetMainWindow());
}

void ICGUI_main() {
    prepareWave(); // Prepare the sound waves
    FRM1 = ICG_FrameMedium(5, 40, 1, 1);
    int* FRM1_PTR = new int(FRM1);

    ICG_Button(5, 5, 150, 25, "(I/O) Power Button", StartStopGame, FRM1_PTR);
    ICG_Static(160, 0, 500, 20, "Created by: Eren Karadeniz, Sevval Gur, Ulas Deniz Cakmazel");
    ICG_Static(160, 20, 550, 20, "Arrow Keys: Move  |   P: Pause    |   R: Resume   |   Power Button: Toggle Game");
    ICG_CheckBox(575, 0, 175, 20, "Faster Intro", ToggleIntroAnimationSpeed);

    ICG_SetOnKeyPressed(WhenKeyPressed);
    ICG_SetOnKeyUp(WhenKeyReleased);
    CreateImage(screenMatrix, 700, 700, ICB_UINT);

    ReadImage("Assests/sprites.bmp", Sprites);
    MagnifyX3(Sprites, Sprites3X);
}