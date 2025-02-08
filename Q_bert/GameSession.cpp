// GameSession.cpp

#include "GameSession.h"
#include "PrintHelper.h"
#include "intro.h"
#include "Game.h"
#include "Main.h"

//extern Player player;

GameSession::GameSession(int* screenHandle, int x, int y)
{
    isGameOver = false;
    _screenHandle = screenHandle;
    CreateImage(_screenMatrix, x, y, ICB_UINT);
    //player = new Player(&_screenMatrix);
}

void GameSession::Refresh(int sleepTime)
{
    if (isGameOver)
    {
		ShowGameOverScreen();

    }
	else if (Game::GetState() == GameState::Paused)
	{
		DrawPaused();
	}
    else
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

GameSession::~GameSession()
{
    _screenMatrix = 0;
    DisplayImage(*_screenHandle, _screenMatrix);
    delete playerptr;
}

