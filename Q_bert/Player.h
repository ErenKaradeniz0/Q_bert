#pragma once

class Player {
public:
    int x, y, currentTile;
    int direction;
    bool jumpStatus;
    bool willFall;
    bool mazeOrder;
    int lifes;

    Player();
    void MoveAnimation(char key, int goal_x, int goal_y);
    void FallOffEdge(char key);
    void move(char key);
    void lostLife(bool isFall);
};

// standalone function
void CreatePlayer();
