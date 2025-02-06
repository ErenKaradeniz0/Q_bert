#include "Enemy.h"
#include "Maze.h"
#include "Player.h"
#include "PrintHelper.h"
#include "icb_gui.h"

extern Player player;
extern Enemy enemy1;
extern Enemy enemy2;

Enemy::Enemy() : Type(false), x(275), y(160), currentTile(0), state(7), isAlive(true){}

void Enemy::Spawn(bool Type, int x, int y, int location, int state, bool isAlive) {
    this->Type = Type;
    this->x = x;
    this->y = y;
    this->currentTile = currentTile;
    this->state = state;
    this->isAlive = isAlive;
    while (this->y < 170) {
        this->y += 5;
        Sleep(30);
    }
    if(!Type)
		this->state = 2;
}

void Enemy::move() {
    // Calculate the difference between enemy and player positions
    int deltaX = player.x - x;
    int deltaY = player.y - y;
    int moveLocation = currentTile;

    // Determine the primary state to move
    if (abs(deltaX) > abs(deltaY)) {
        if (deltaX > 0) {
            if (SquareBlocks[currentTile].right >= 0) {
                moveLocation = SquareBlocks[currentTile].right;
            }
        }
        else {
            if (SquareBlocks[currentTile].left >= 0) {
                moveLocation = SquareBlocks[currentTile].left;
            }
        }
    }
    else {
        if (deltaY > 0) {
            if (SquareBlocks[currentTile].down >= 0) {
                moveLocation = SquareBlocks[currentTile].down;
            }
        }
        else {

            if (SquareBlocks[currentTile].up >= 0) {
                moveLocation = SquareBlocks[currentTile].up;
            }
        }

    }
    if (!Type) {

        Sleep(50);
        this->state = 2;
    }

    // Animate the movement
    MoveAnimation(SquareBlocks[moveLocation].centerX - x, SquareBlocks[moveLocation].centerY - y, moveLocation);
}

void Enemy::MoveAnimation(int deltaX, int deltaY, int moveLocation) {
    int steps = 10;
    int stepX = (deltaX + 5) / steps;
    int stepY = (deltaY + 50) / steps;

    this->state = 1;
    y -= 40;
    for (int i = 0; i < steps; ++i) {
        x += stepX;
        y += stepY;
        Sleep(30);
    }
    this->state = 2; // Slime Jump

    currentTile = moveLocation;
    if (player.currentTile == enemy1.currentTile || player.currentTile == enemy2.currentTile) {
        player.lostLife(false);
    }
}
