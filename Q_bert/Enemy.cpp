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

Enemy::Enemy() : isHatch(false), currentTile(SquareBlock()), state(7), willFall(false), isAlive(false) {}

void Enemy::Spawn(bool isHatch, int state, bool isAlive) {
    srand(time(NULL));  // Seed the random number generator with the current time
    SquareBlock randomtile = SquareBlocks[rand() % 2 + 1];
    this->mazeOrder = false;
    this->isHatch = isHatch;
    this->x = randomtile.centerX;
    this->y = 0;
    this->state = state;
    this->isAlive = isAlive;
    int limit = (state == 3) ? 150 : 160;
    while (this->y < limit) {
        this->y += 5;
        Game::SleepI(30);
        if (this->y == limit)
            this->currentTile = randomtile; // Set the current tile at %50 of animation
    }
    if (!isHatch) {
        if (state == 1)
            state = 2;
        else if (state == 3)
            state = 4;
    }
}

void Enemy::FallOffEdge(int move) {
    currentTile.id = -1;
    if (!isHatch) {
        if (state == 2)
            state = 1;
        else if (state == 4)
            state = 3;
    }

    if (isHatch) {
        enemySnake.playSnakeFallSound = true;
    }

    int x_change = (move == 0) ? 5 : (move == 1) ? -5 : 0;
    int y_change = 5;
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
    willFall = false;
    isAlive = false;

}

void Enemy::JumpToDiskAndFall(int disc_id) {
    mazeOrder = true;
    playSnakeJumpSound = true;

    int target_x = Discs[disc_id].center_x;
    int target_y = Discs[disc_id].center_y;

    y -= 40;

    for (int i = 0; i < 20; i++) {
        if (x < target_x) x += 3;
        if (x > target_x) x -= 3;
        if (y < target_y) y += 3;
        if (y > target_y) y -= 3;
        Game::SleepI(15);
    }

    playSnakeFallSound = true;
    x = target_x;

    for (int i = 0; i < 40; i++) {
        y += 12;
        Game::SleepI(15);
    }
    isAlive = false;
    currentTile.id = -1;
}

void Enemy::move() {
    if (state == 4 && currentTile.id > 20) {
        isHatch = true;
        state = 4;
        Game::SleepI(1000);
    }

    if (!isHatch) {
        Game::SleepI(50);
        if (state == 1)
            state = 2;
        else if (state == 3)
            state = 4;

        int randomMove = rand() % 2;
        randomMove = 0;
        if (randomMove == 0) {
            if (currentTile.right != nullptr) {
                MoveAnimation(*currentTile.right);
            }
            else {
                willFall = true;
            }
        }
        else {
            if (currentTile.down != nullptr) {
                MoveAnimation(*currentTile.down);
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
        if ((currentTile.id == 10 && Discs[0].move_state && player.currentTile.id == Discs[0].disc_id) ||
            (currentTile.id == 14 && Discs[1].move_state && player.currentTile.id == Discs[1].disc_id)) {
            JumpToDiskAndFall(currentTile.id == 10 ? 0 : 1);
            return;
        }

        int deltaX = player.x - x;
        int deltaY = player.y - y;
        SquareBlock destinationTile;

        if (deltaX > 0 && currentTile.right != nullptr) {
            state = 9;
            Game::SleepI(100);
            destinationTile = *currentTile.right;
            Game::SleepI(100);
            state = 10;
        }
        else if (deltaX < 0 && currentTile.left != nullptr) {
            state = 7;
            Game::SleepI(100);
            destinationTile = *currentTile.left;
            Game::SleepI(100);
            state = 8;
        }
        else if (deltaY > 0 && currentTile.down != nullptr) {
            state = 11;
            Game::SleepI(100);
            destinationTile = *currentTile.down;
            Game::SleepI(100);
            state = 12;
        }
        else if (deltaY < 0 && currentTile.up != nullptr) {
            state = 5;
            Game::SleepI(100);
            destinationTile = *currentTile.up;
            Game::SleepI(100);
            state = 6;
        }

        if (destinationTile.id != -1) {
            MoveAnimation(destinationTile);
        }
    }
}

void Enemy::MoveAnimation(SquareBlock GoalBlock) {
    int goal_x = GoalBlock.centerX;
    int goal_y = GoalBlock.centerY;
    int br_x = (x < goal_x) ? 5 : -5;
    int br_y = (y < goal_y) ? 5 : -5;
    this->y -= 40;
    Game::SleepI(50);

    if (!isHatch) {
        if (state == 1 || state == 2) {
            enemyBall1.playRedBallJumpSound = true;
        }
        else if (state == 3 || state == 4) {
            enemyBall2.playSnakeEggJumpSound = true;
        }
    }
    else {
        enemySnake.playSnakeJumpSound = true;
    }

    if (state == 2) state = 1;
    else if (state == 4) state = 3;

    int initial_diff_x = abs(goal_x - x);
    int initial_diff_y = abs(goal_y - y);
    int totalSteps = max(initial_diff_x, initial_diff_y) / 5;
    int halfSteps = totalSteps / 2 + totalSteps / 3;
    int stepCount = 0;

    while (x != goal_x || y != goal_y) {
        if (x != goal_x) {
            x += br_x;
        }
        if (y != goal_y) {
            y += br_y;
        }

        stepCount++;

        if (isAlive && stepCount == halfSteps) {
            currentTile = GoalBlock;
        }

        Game::SleepI(15);
    }

    if (state == 1) state = 2;
    else if (state == 3) state = 4;

    x = goal_x;
    y = goal_y;
}