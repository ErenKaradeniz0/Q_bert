#ifndef MAIN_H
#define MAIN_H

#include "icb_gui.h"
#include "PrintHelper.h"
#include "intro.h"
#include "Player.h"
#include "Maze.h"

// Globals
extern bool gameRunning;
extern HANDLE renderMutex;
extern ICBYTES screenMatrix, Sprites, Sprites3X;
extern ICBYTES CurrentTileMatrix, PlayerMatrix, EnemyMatrix, DiscMatrix;
extern int FRM1;
extern int keypressed;
extern int score;
extern Player player;

// Function Prototypes
void ICGUI_Create();
void DrawPlayer();
void DrawEnemies();
void renderGrid();
VOID* renderThread();
VOID* InputThread();
VOID* turnDiscThread();
void StartGame();
void WhenKeyPressed(int k);
void ICGUI_main();

#endif // MAIN_H