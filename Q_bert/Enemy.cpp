#include "Enemy.h"
#include "Maze.h"
#include "Player.h"
#include "PrintHelper.h"
#include "Sound.h"
#include "icb_gui.h"
#include <cstdlib>  // For srand() and rand()
#include <ctime>    // For time()
#include "Main.h"
#include "Game.h"

extern Player player;
extern Enemy enemyBall1;
extern Enemy enemyBall2;
extern Enemy enemySnake;

Enemy::Enemy() : isHatch(false),currentTile(SquareBlock()), state(7), willFall(false), isAlive(false){}

void Enemy::Spawn(bool isHatch, int state, bool isAlive) {
    srand(time(NULL));  // Seed the random number generator with the current time
    this->isHatch = isHatch;
	this->currentTile = SquareBlocks[rand() % 2 + 1];
    this->x = currentTile.centerX;
    this->y = 0;
    this->state = state;
    this->isAlive = isAlive;
	int limit = 160;
	if (state == 3)
        limit = 150;
    while (this->y < limit) {
        this->y += 5;
        Game::SleepI(30);
    }
    if(!isHatch)
        if (state == 1)
            state = 2;
        else if (state == 3)
            state = 4;
}
void Enemy::FallOffEdge(int move) {
    if (!isHatch)
        if (state == 2)
            state = 1;
        else if (state == 4)
            state = 3;
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
        Game::SleepI(15);
    }
    isAlive = false;
    willFall = false;
}
void Enemy::Hatch(Enemy enemy) {


}
void Enemy::move() {
    if (state == 4 && currentTile.id > 20) {
        isHatch = true;
        state = 4;
		Game::SleepI(1000);
        Hatch(enemySnake);
    }

    if (!isHatch) {
        Game::SleepI(50);
        if (state == 1)
            state = 2;
        else if (state == 3)
            state = 4;

        // Randomly choose to move right or down
        int randomMove = rand() % 2; // 0 or 1
        randomMove = 0;
        if (randomMove == 0) {
            if (currentTile.right >= 0) {
                MoveAnimation(SquareBlocks[currentTile.right]);
            }
            else if (state == 4) {

            }
            else {
                willFall = true;
            }
        }
        else {
            if (currentTile.down >= 0) {
                MoveAnimation(SquareBlocks[currentTile.down]);
            }
            else if (state == 4) {

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
        // Calculate the difference between enemy and player positions
        int deltaX = player.x - x;
        int deltaY = player.y - y;
        int destinationTileId = -1;

        // Determine the primary state to move
        if (deltaX > 0 && currentTile.right >= 0) {
            state = 9;
            Game::SleepI(100);
            destinationTileId = currentTile.right;
            Game::SleepI(100);
            state = 10;
        }
        else if (deltaX < 0 && currentTile.left >= 0) {
            state = 7;
            Game::SleepI(100);
            destinationTileId = currentTile.left;
            Game::SleepI(100);
            state = 8;
        }

        else if (deltaY > 0 && currentTile.down >= 0) {
            state = 11;
            Game::SleepI(100);
            destinationTileId = currentTile.down;
            Game::SleepI(100);
            state = 12;
        }
        else if (deltaY < 0 && currentTile.up >= 0) {
            state = 5;
			Game::SleepI(100);
            destinationTileId = currentTile.up;
            Game::SleepI(100);
            state = 6;

        }

        // Animate the 
        currentTile;
        destinationTileId;
        if (destinationTileId != -1) {
            MoveAnimation(SquareBlocks[destinationTileId]);
        }
    }
}

void Enemy::MoveAnimation(SquareBlock GoalBlock) {
    int goal_x = GoalBlock.centerX;
    int goal_y = GoalBlock.centerY;
    int br_x = 0, br_y = 0;
    this->y -= 40;
    Game::SleepI(50);

    if (state == 2)
        state = 1;
    else if (state == 4)
        state = 3;

    x < goal_x ? br_x = 5 : br_x = -5;
    y < goal_y ? br_y = 5 : br_y = -5;
    while (x != goal_x || y != goal_y) {
        if (x != goal_x) {
            x += br_x;
        }
        if (y != goal_y) {
            y += br_y;
        }
        Game::SleepI(15);
    }

    if (state == 1)
        state = 2;
    else if (state == 3)
        state = 4;

    x = goal_x;
    y = goal_y;
    Game::SleepI(50);

    currentTile = GoalBlock;
    if (player.currentTile == enemyBall1.currentTile.id || player.currentTile == enemyBall2.currentTile.id) {
        player.lostLife(false);
		//InterruptableInterruptableSleep(1000);
    }

    /*const char* soundPath = "Sounds/Jump2.wav";
    CreateSoundThread(soundPath);*/

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Jump2Sound, NULL, 0, NULL);



}  
