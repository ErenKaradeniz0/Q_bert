#pragma once

class Enemy {
public:
	bool Type;
    int x;
    int y;
    int state;
    bool isAlive;
    int currentTile;

    Enemy();
    void move();
    void Spawn(bool Type,int x, int y, int location, int state, bool isAlive = true);
	void MoveAnimation(int deltaX, int deltaY, int moveLocation);
};
