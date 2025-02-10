#pragma once
#include "Maze.h"
class Enemy {
public:
	bool isHatch;
    int x;
    int y;
    int state;
    bool willFall;
    bool isAlive;
    bool playRedBallJumpSound;
    bool playSnakeEggJumpSound;
    bool playSnakeJumpSound;
    bool playSnakeFallSound;
    SquareBlock currentTile;

    Enemy();
    void move();
    void Hatch(Enemy enemy);
    void Spawn(bool isHatch, int state, bool isAlive = true);
	void MoveAnimation(SquareBlock GoalBlock);
	void FallOffEdge(int move);
    void JumpToDiskAndFall(int disc_id);
};
