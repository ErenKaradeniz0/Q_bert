#include "Game.h"
#include "Main.h"
#include "GameSession.h"

extern bool isGameOver;

HANDLE Game::gameRunningEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
HANDLE Game::gameStoppingEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
HANDLE Game::gameStoppedEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
HANDLE Game::gameMainThread = nullptr;
int Game::wait = NULL;
extern bool isVictory;
extern bool isPaused;

int Game::Start(void* param)
{
	if (gameMainThread != nullptr)
		return COMMAND_FAILED;

	gameMainThread = CreateThread(NULL, 0, GameControllerMain, param, 0, NULL);
	ResetEvent(gameStoppedEvent);
	SetEvent(gameRunningEvent);
	ResetEvent(gameStoppingEvent);

	isGameOver = false;
	return COMMAND_SUCCESS;
}

int Game::Stop()
{
	if (gameMainThread == nullptr || wait != NULL)
		return COMMAND_FAILED;

	// Reset the enemies
	enemyBall1.isAlive = false;
	enemyBall2.isAlive = false;
	enemySnake.isAlive = false;
	enemyBall1.currentTile.id = -1;
	enemyBall2.currentTile.id = -1;
	enemySnake.currentTile.id = -1;

	ResetEvent(gameRunningEvent);
	SetEvent(gameStoppingEvent);


	ResetSoundFlags();
	gameMainThread = nullptr;

	return COMMAND_SUCCESS;
}

void Game::CompleteStop()
{
	SetEvent(gameStoppedEvent);
}

bool Game::IsStopped()
{
	return WaitForSingleObject(gameStoppedEvent, 0) == WAIT_OBJECT_0;
}

void Game::SleepI(int ms)
{
	WaitForSingleObject(gameStoppingEvent, ms);
}


int Game::Pause(bool Paused)
{
	if (gameMainThread == nullptr || wait != NULL)
		return COMMAND_FAILED;
	isPaused = Paused;
	wait = INFINITE;
	ResetEvent(gameRunningEvent); //true -> false
	return COMMAND_SUCCESS;
}

int Game::Resume()
{
	if (gameMainThread == nullptr || wait != INFINITE)
		return COMMAND_FAILED;
	isPaused = false;

	SetEvent(gameRunningEvent);
	wait = NULL;

	return COMMAND_SUCCESS;
}

GameState Game::GetState()
{
	if (gameMainThread == nullptr)
		return Stopped;
	else if (wait == INFINITE)
		return Paused;
	else
		return Running;
}

bool Game::Run()
{
	if (isVictory) {
		return false;
	}
	return WaitForSingleObject(gameRunningEvent, wait) == WAIT_OBJECT_0;
}

bool Game::RunMain()
{
	return WaitForSingleObject(gameStoppingEvent, 0) == WAIT_TIMEOUT;
}

