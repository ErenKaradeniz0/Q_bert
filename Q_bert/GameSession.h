// GameSession.h

#ifndef GAMESESSION_H
#define GAMESESSION_H

#include "icb_gui.h"
#include "Player.h"

class GameSession
{
    int* _screenHandle;
    ICBYTES _screenMatrix;

public:
    bool isGameOver;
    Player* playerptr;

    GameSession(int* screenHandle, int x, int y);
    void Refresh(int sleepTime = 30);
    void ShowGameOverScreen();
    ~GameSession();
};

#endif // GAMESESSION_H