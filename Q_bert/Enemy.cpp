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
    SquareBlock randomtile = SquareBlocks[rand() % 2 + 1];
    this->isHatch = isHatch;
    this->x = randomtile.centerX;
    this->y = 0;
    this->state = state;
    this->isAlive = isAlive;
	int limit = 160;
	if (state == 3)
        limit = 150;
    while (this->y < limit) {
        this->y += 5;
        Game::SleepI(30);
		if (this->y == 75)
			this->currentTile = randomtile; // Set the current tile at %50 of animation
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

    if (isHatch) {
        enemySnake.playSnakeFallSound = true;
    }

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

void Enemy::JumpToDiskAndFall(int disc_id) {
    playSnakeJumpSound = true;

    // Diskin konumunu al
    int target_x = Discs[disc_id].center_x;
    int target_y = Discs[disc_id].center_y;

    // Diske zýplama
    int original_x = x;
    int original_y = y;
    y -= 40; // Ýlk yükseliþ

    // Diske doðru hareket
    for (int i = 0; i < 20; i++) {
        if (x < target_x) x += 3;
        if (x > target_x) x -= 3;
        if (y < target_y) y += 3;
        if (y > target_y) y -= 3;
        Game::SleepI(15);
    }

    // Disk pozisyonuna ulaþtýktan sonra düþme
    playSnakeFallSound = true;
    x = target_x; // x'i sabitle

    // Sadece aþaðý düþ
    for (int i = 0; i < 40; i++) {
        y += 12;
        Game::SleepI(15);
    }

    // Yýlaný temizle
    isAlive = false;
    currentTile.id = -1;
}

void Enemy::move() {
    if (state == 4 && currentTile.id > 20) {
        isHatch = true;
        state = 4;
        Game::SleepI(1000);
        Hatch(enemySnake);
    }

    if (!isHatch) {
        // Ball enemies movement - unchanged
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
            else if (state == 4) {
            }
            else {
                willFall = true;
            }
        }
        else {
            if (currentTile.down != nullptr) {
                MoveAnimation(*currentTile.down);
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
        // Snake disc check with Q*bert position verification
        if ((currentTile.id == 10 && Discs[0].move_state && player.currentTile.id == Discs[0].disc_id) ||
            (currentTile.id == 14 && Discs[1].move_state && player.currentTile.id == Discs[1].disc_id)) {
            JumpToDiskAndFall(currentTile.id == 10 ? 0 : 1);
            return;
        }

        // Normal snake movement - unchanged
        int deltaX = player.x - x;
        int deltaY = player.y - y;
        SquareBlock destinationTile = SquareBlock();

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
    int br_x = 0, br_y = 0;
    this->y -= 40;
    Game::SleepI(50);

    if (!isHatch) {
        if (state == 1 || state == 2) {  // Red Ball
            enemyBall1.playRedBallJumpSound = true;
        }
        else if (state == 3 || state == 4) {  // Snake Egg
            enemyBall2.playSnakeEggJumpSound = true;
        }
    }
    else {  // Snake
        enemySnake.playSnakeJumpSound = true;
    }

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
    currentTile = GoalBlock;
    if (player.currentTile.id == enemyBall1.currentTile.id || player.currentTile.id == enemyBall2.currentTile.id || player.currentTile.id == enemySnake.currentTile.id) {
        player.lostLife(false);
    }
}  
