#ifndef PRINTHELPER_H
#define PRINTHELPER_H

#include "icb_gui.h"

void RenderChar(ICBYTES& screen, char c, int x, int y);
void RenderString(ICBYTES& screen, const char* text, int x, int y, int spacing = 25);
void ShowGameOverScreen();
void DrawScore();
void DrawLives();
void DrawMap();
void SelectEffectDisc(int k);
void DrawDisc();
void DrawPlayer();
void DrawPaused();
void DrawEnemies();
#endif // PRINTHELPER_H
