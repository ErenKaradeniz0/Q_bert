#pragma once
#include "Maze.h"
class Player {
public:
    int x, y;
    int direction;
	SquareBlock currentTile;
    bool willFall;
    bool mazeOrder;
    int lifes;
	bool showLostLifeText;
    int lostLifeCounter;
	bool playJumpSound;

    Player();
    void MoveAnimation(char key, int goal_x, int goal_y);
    void FallOffEdge(char key);
    void JumpDiscAnimation(int disc_id,int goal_x, int goal_y);
    void DiskAndPlayerMovingAnimation(int disc_id);
    void move(char key);
    void lostLife(bool isFall);
};

// standalone function
void CreatePlayer();
