#pragma once
#include <windows.h>

constexpr auto COMMAND_FAILED = 0;
constexpr auto COMMAND_SUCCESS = 1;

enum GameState
{
	Running,
	Paused,
	GameOver,
	Stopped
};

class Game
{
private:
	static HANDLE gameRunningEvent;
	static HANDLE gameStoppingEvent;
	static HANDLE gameMainThread;
	static int wait;

public:
	static int Start(void* param);
	static int Stop();
	static void SleepI(int ms);
	static int Pause(bool isPaused);
	static int Resume();

	static GameState GetState();

	// This function is used to while loop the game child threads
	static bool Run();

	// This function is used to while loop the game main thread
	static bool RunMain();

};

