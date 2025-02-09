// GameSession.h

#ifndef GAMESESSION_H
#define GAMESESSION_H

#include "icb_gui.h"
#include "Player.h"

class GameSession
{
    long long counter;
    int* _screenHandle;
    ICBYTES _screenMatrix;

public:
    bool isGameOver;
    Player* playerptr;

    GameSession(int* screenHandle, int x, int y);
    void Refresh(int sleepTime = 30);
    void ShowGameOverScreen();
    void DrawPaused();
    ~GameSession();
};

#endif // GAMESESSION_H