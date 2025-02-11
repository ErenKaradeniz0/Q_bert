#ifndef MAIN_H
#define MAIN_H

#include "icb_gui.h"
#include "PrintHelper.h"
#include "intro.h"
#include "Player.h"
#include "Enemy.h"
#include "Maze.h"
#include "Sound.h"

// Global variables
extern ICBYTES screenMatrix, Sprites, Sprites3X;
extern int FRM1;
extern int keypressed;
extern bool keyPressedControl;
extern bool isAnimationFaster;
extern int score;

extern Player player;
extern Enemy enemyBall1;
extern Enemy enemyBall2;
extern Enemy enemySnake;

// Function declarations
void ICGUI_Create();
DWORD WINAPI GameControllerMain(LPVOID lpParam);
DWORD WINAPI InputThread(LPVOID lpParam);
DWORD WINAPI turnDiscThread(LPVOID lpParam);
DWORD WINAPI EnemyBall1Thread(LPVOID lpParam);
DWORD WINAPI EnemyBall2Thread(LPVOID lpParam);
DWORD WINAPI EnemySnakeThread(LPVOID lpParam);
DWORD WINAPI playerSoundThread(LPVOID lpParam);
DWORD WINAPI enemyBall1SoundThread(LPVOID lpParam);
DWORD WINAPI enemyBall2SoundThread(LPVOID lpParam);
DWORD WINAPI SnakeSoundThread(LPVOID lpParam);
void StartStopGame(void* FRM1_PTR);
void WhenKeyPressed(int k);
void WhenKeyReleased(int k);
void ToggleIntroAnimationSpeed(int state);
void ICGUI_main();

#endif // MAIN_H
