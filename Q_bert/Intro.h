#ifndef INTRO_H
#define INTRO_H
#include "icb_gui.h"

void DrawStartupAnimation();
void DrawSideObjects();
void ShowHighScoreScreen(int currentScore);

struct HighScore {
    char name[4];
    int score;
};

#endif // INTRO_H
