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
    DisplayImage(FRM1, screenMatrix);
    Sleep(30);

    if (player.lifes <= 0) {
        ShowGameOverScreen();
    }
    else if (Game::GetState() == GameState::Paused)
    {
		DrawPaused();
    }
    else if(Game::GetState() == GameState::Running && !isGameOver)
    {
        counter = 0;
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
        DrawSideObjects();

        // Draw lives
        DrawLives();

        // Draw enemies
        DrawEnemies();

        if (!player.mazeOrder) {
            // Draw player after map if not falling
            DrawPlayer();
        }
    }

}

void GameSession::ShowGameOverScreen() {

    if (counter % 60 < 20)
    {
        FillRect(screenMatrix, 220, 370, 260, 30, 0x000000);
    }
    else
    {
            RenderString(screenMatrix, "GAME OVER", 220, 375, 30);
    }
    counter++;
}


void GameSession:: DrawPaused()
{
    FillRect(screenMatrix, 270, 370, 170, 30, 0x000000);
    RenderString(screenMatrix,"PAUSED", 270, 375, 30);
}

GameSession::~GameSession()
{
    _screenMatrix = 0;
    DisplayImage(*_screenHandle, _screenMatrix);
    delete playerptr;
}