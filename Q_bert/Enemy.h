#pragma once
#include "Maze.h"
class Enemy {
public:
	bool Type;
    int x;
    int y;
    int state;
    bool willFall;
    bool isAlive;
    SquareBlock currentTile;

    Enemy();
    void move();
    void Spawn(bool Type, int state, bool isAlive = true);
	void MoveAnimation(SquareBlock GoalBlock);
	void FallOffEdge(int move);
};
