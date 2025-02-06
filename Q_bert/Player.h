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
    void JumpDiscAnimation(int disc_id,int goal_x, int goal_y);
    void DiskAndPlayerMovingAnimation(int disc_id);
    void move(char key);
};

// standalone function
void CreatePlayer();
