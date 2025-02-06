#include "Enemy.h"
#include "Maze.h"

#include "Player.h"
#include "PrintHelper.h"
#include "icb_gui.h"

extern Player player;
extern Enemy enemy1;
extern Enemy enemy2;

Enemy::Enemy() : x(275), y(160), location(0), direction(7) {}

void Enemy::Spawn(int x, int y, int location, int direction, bool isAlive) {
    this->x = x;
    this->y = y;
    this->location = location;
    this->direction = direction;
	this->isAlive = isAlive;
    while (this->y < 170) {
        this->y += 5;
        Sleep(30);
    }
}

void Enemy::move() {
    // Calculate the difference between enemy and player positions
    int deltaX = player.x - x;
    int deltaY = player.y - y;

    // Determine the primary direction to move
    if (abs(deltaX) > abs(deltaY)) {
        if (deltaX > 0) {
            direction = 5; // Move right
            if (SquareBlocks[location].right >= 0) {
                location = SquareBlocks[location].right;
            }
        }
        else {
            direction = 3; // Move left
            if (SquareBlocks[location].left >= 0) {
                location = SquareBlocks[location].left;
            }
        }
    }
    else {
        if (deltaY > 0) {
            direction = 7; // Move down
            if (SquareBlocks[location].down >= 0) {
                location = SquareBlocks[location].down;
            }
        }
        else {
            direction = 1; // Move up
            if (SquareBlocks[location].up >= 0) {
                location = SquareBlocks[location].up;
            }
        }
    }

    // Update enemy position based on new location
    x = SquareBlocks[location].x + 20;
    y = SquareBlocks[location].y - 10;
}


