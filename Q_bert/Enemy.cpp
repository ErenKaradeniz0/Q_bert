#include "Enemy.h"
#include "Maze.h"
#include "Player.h"
#include "PrintHelper.h"
#include "icb_gui.h"
#include <cstdlib>  // For srand() and rand()
#include <ctime>    // For time()

extern Player player;
extern Enemy enemy1;
extern Enemy enemy2;

Enemy::Enemy() : Type(false),currentTile(SquareBlock()), state(7), willFall(false), isAlive(false){}

void Enemy::Spawn(bool Type, int state, bool isAlive) {
    srand(time(NULL));  // Seed the random number generator with the current time
    this->Type = Type;
	this->currentTile = SquareBlocks[rand() % 2 + 1];
    this->x = currentTile.centerX;
    this->y = 0;
    this->state = state;
    this->isAlive = isAlive;
    while (this->y < 160) {
        this->y += 5;
        Sleep(30);
    }
    if(!Type)
		this->state = 2;
}
void Enemy::FallOffEdge(int move) {
    if (!Type)
        this->state = 1;
    // Calculate the total change in x

    int x_change = (move == 0) ? 5 : (move == 1) ? -5 : 0;
	int y_change = 5;
    // Falling animation
    this->y -= 40;
    for (int i = 0; i < 40; ++i) {
        if (i < 10) {
            y += y_change;
            x += x_change;
		}
		else {
			y += 5;
		}
        Sleep(15);
    }
    isAlive = false;
    willFall = false;
}
void Enemy::move() {
    if (!Type) {
        Sleep(50);
        this->state = 2;

        // Randomly choose to move right or down
        int randomMove = rand() % 2; // 0 or 1
        if (randomMove == 0) {
            if (currentTile.right >= 0) {
                MoveAnimation(SquareBlocks[currentTile.right]);
            }
            else {
                willFall = true;
            }
        }
        else {
            if (currentTile.down >= 0) {
                MoveAnimation(SquareBlocks[currentTile.down]);
            }
            else {
                willFall = true;
            }

        }
        if (willFall) 
            FallOffEdge(randomMove);
    return; 

    }
    else {
        /*
        //Calculate the difference between enemy and player positions
        int deltaX = player.x - x;
        int deltaY = player.y - y;
        int destinationTile;

        // Determine the primary state to move
        if (abs(deltaX) > abs(deltaY)) {
            if (deltaX > 0) {
                if (currentTile.right >= 0) {
                    destinationTile = currentTile.right;
                }
            }
            else {
                if (currentTile.left >= 0) {
                    destinationTile = currentTile.right;
                }
            }
        }
        else {
            if (deltaY > 0) {
                if (currentTile.down >= 0) {
                    destinationTile = currentTile.down;
                }
            }
            else {

                if (currentTile.up >= 0) {
                    destinationTile = currentTile.up;
                }
            }
        }
        // Animate the movement
        MoveAnimation(SquareBlocks[destinationTile]);
        */
    }
}

void Enemy::MoveAnimation(SquareBlock GoalBlock) {
    int goal_x = GoalBlock.centerX;
    int goal_y = GoalBlock.centerY;
    int br_x = 0, br_y = 0;
    this->y -= 40;
    Sleep(50);
    this->state = 1;
    x < goal_x ? br_x = 5 : br_x = -5;
    y < goal_y ? br_y = 5 : br_y = -5;
    while (x != goal_x || y != goal_y) {
        if (x != goal_x) {
            x += br_x;
        }
        if (y != goal_y) {
            y += br_y;
        }
        Sleep(15);
    }
    this->state = 2; // Slime Jump
    x = goal_x;
    y = goal_y;
    Sleep(50);

    currentTile = GoalBlock;
    if (player.currentTile == enemy1.currentTile.id || player.currentTile == enemy2.currentTile.id) {
        player.lostLife(false);
    }
}  
