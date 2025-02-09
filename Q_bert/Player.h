#pragma once

class Player {
public:
    int x, y, currentTile;
    int direction;
    bool willFall;
    bool mazeOrder;
    int lifes;
	bool showLostLifeText;
    int lostLifeCounter;

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
