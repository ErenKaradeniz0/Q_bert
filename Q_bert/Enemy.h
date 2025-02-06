#pragma once

class Enemy {
public:
    int x;
    int y;
    int location;
    int direction;
    bool isAlive;

    Enemy();
    void move();
    void Spawn(int x, int y, int location, int direction, bool isAlive = true);
};
