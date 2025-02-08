// GameSession.cpp

#include "GameSession.h"
#include "PrintHelper.h"
#include "intro.h"
#include "Game.h"
#include "Enemy.h"

int FRM1;
ICBYTES screenMatrix, Sprites, Sprites3X;
Player player; // Global player
Enemy enemyBall1; // Global enemy 1
Enemy enemyBall2; // Global enemy 2
Enemy enemySnake; // Global enemy 2

bool isGameOver = false;

GameSession::GameSession(int* screenHandle, int x, int y)
{
    isGameOver = false;
    _screenHandle = screenHandle;
    CreateImage(_screenMatrix, x, y, ICB_UINT);
    //player = new Player(&_screenMatrix);
}

void GameSession::Refresh(int sleepTime)
{
    if (player.lifes <= 0) {
        ShowGameOverScreen();
    }
	else if (Game::GetState() == GameState::Paused)
		DrawPaused();
    else if(Game::GetState() == GameState::Running && !isGameOver)
    {
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


void GameSession:: ShowGameOverScreen() {
    isGameOver = true;
    Game::SleepI(30); 

    const int BLINK_COUNT = 10;
    const int BLINK_DELAY = 300;

    for (int i = 0; i < BLINK_COUNT; i++) {
		if (i % 2 == 0)
			FillRect(screenMatrix, 200, 350, 275, 75, 0x000000);
		else
			RenderString(screenMatrix, "GAME OVER", 225, 375, 25);
        Game::SleepI(BLINK_DELAY);

        DisplayImage(FRM1, screenMatrix);
    }
	Game::Stop();
}

GameSession::~GameSession()
{
    _screenMatrix = 0;
    DisplayImage(*_screenHandle, _screenMatrix);
    delete playerptr;
}