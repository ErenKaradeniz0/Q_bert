#include "GameSession.h"
#include "PrintHelper.h"
#include "intro.h"
#include "Game.h"
#include "Enemy.h"
#include "Sound.h"

int FRM1;
ICBYTES screenMatrix, Sprites, Sprites3X;
Player player;
Enemy enemyBall1;
Enemy enemyBall2;
Enemy enemySnake;

int Victorycounter = 0;
bool isGameOver = false;
bool isVictory = false;
bool isPaused = false;
static bool victoryPlayed = false;
extern int score;
extern int keypressed;

GameSession::GameSession(int* screenHandle, int x, int y)
{
    Victorycounter = 0;
    isGameOver = false;
    isVictory = false;
    victoryPlayed = false;
    _screenHandle = screenHandle;
    CreateImage(_screenMatrix, x, y, ICB_UINT);
    //player = new Player(&_screenMatrix);
}

void GameSession::Refresh(int sleepTime)
{
    DisplayImage(FRM1, screenMatrix);
    Sleep(30);

    if (player.lifes <= 0) {
        ShowGameOverScreen();
    }
    else if (Game::GetState() != GameState::Stopped && !isGameOver) {
        counter = 0;
        screenMatrix = 0; // Clear the screen

        if (player.mazeOrder) {
            // Draw player first if falling
            DrawPlayer();
        }
        if (enemySnake.mazeOrder) {
			// Draw Snake if falling
			DrawSnake();
        }

        // Draw map
        DrawMap();

        // Draw Disc()
        DrawDisc();

        // Draw score
        DrawScore();

        // Draw Qbert logo
        DrawSideObjects();

        // Draw lives
        DrawLives();

		// Draw red balls
        DrawRedBalls();

        if (!player.mazeOrder) {
            // Draw player after map if not falling
            DrawPlayer();
        }
        if (!enemySnake.mazeOrder) {
            // Draw Snake if falling
            DrawSnake();
        }
		if (isPaused)
			DrawPaused();
        if (score == 700) {
            isVictory = true;
            ShowVictoryScreen();
        }
    }
}

void GameSession::ShowGameOverScreen() {
    Game::Pause(false);
    if (counter % 60 < 20) {
        FillRect(screenMatrix, 220, 370, 260, 30, 0x000000);
    }
    else {
        RenderString(screenMatrix, "GAME OVER", 220, 375, 30);
    }
    counter++;
    if (counter == 90) {
        ShowHighScoreScreen(score);
        Game::Resume();
        Game::Stop();
    }
}

void GameSession::ShowVictoryScreen() {
    static bool victoryPlayed = false;

    if (!victoryPlayed) {
        VictorySound();
        victoryPlayed = true;
    }

    for (int i = 0; i < 28; ++i) {
        if (Victorycounter % 3 == 0) {
            SquareBlocks[i].state = 0;
        }
        else if (Victorycounter % 3 == 1) {
            SquareBlocks[i].state = 1;
        }
        else if (Victorycounter % 3 == 2) {
            SquareBlocks[i].state = 2;
        }
    }
    Victorycounter++;
    if (Victorycounter == 60) {
        ShowHighScoreScreen(score);
        Victorycounter = 0;
        Game::Stop();
    }
}

void GameSession::DrawPaused() {
    FillRect(screenMatrix, 270, 370, 170, 30, 0x000000);
    RenderString(screenMatrix, "PAUSED", 270, 375, 30);
}

GameSession::~GameSession() {
    _screenMatrix = 0;
    DisplayImage(*_screenHandle, _screenMatrix);
    delete playerptr;
}
