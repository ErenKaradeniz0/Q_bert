#pragma once

class Player {
public:
    int x, y, location;
    int direction;
    bool jumpStatus;
    bool willFall;
    bool mazeOrder;
    int lifes;

    Player();
    void BlockMoveAnimation(char key, int goal_x, int goal_y);
    void FallOffEdge(char key);
    void move(char key);
};

// standalone function
void CreatePlayer();
