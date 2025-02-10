#include "Game.h"
#include "Main.h"
#include "GameSession.h"

extern bool isGameOver;

HANDLE Game::gameRunningEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
HANDLE Game::gameStoppingEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
HANDLE Game::gameMainThread = nullptr;
int Game::wait = NULL;
extern bool isVictory;

int Game::Start(void* param)
{
	if (gameMainThread != nullptr)
		return COMMAND_FAILED;

	gameMainThread = CreateThread(NULL, 0, GameControllerMain, param, 0, NULL);

	SetEvent(gameRunningEvent);
	ResetEvent(gameStoppingEvent);

	isGameOver = false;
	return COMMAND_SUCCESS;
}

int Game::Stop()
{
	if (gameMainThread == nullptr || wait != NULL)
		return COMMAND_FAILED;

	ResetEvent(gameRunningEvent);
	SetEvent(gameStoppingEvent);

	gameMainThread = nullptr;

	return COMMAND_SUCCESS;
}

void Game::SleepI(int ms)
{
	WaitForSingleObject(gameStoppingEvent, ms);
}


int Game::Pause()
{
	if (gameMainThread == nullptr || wait != NULL)
		return COMMAND_FAILED;

	wait = INFINITE;
	ResetEvent(gameRunningEvent); //true -> false

	return COMMAND_SUCCESS;
}

int Game::Resume()
{
	if (gameMainThread == nullptr || wait != INFINITE)
		return COMMAND_FAILED;

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

