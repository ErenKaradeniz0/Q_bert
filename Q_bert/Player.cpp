#include "Player.h"
#include "Maze.h"
#include "Printhelper.h"
#include "icb_gui.h" 

extern int FRM1;
extern int score;
extern Player player;
extern bool gameRunning;
extern int keypressed;

Player::Player() : x(0), y(0), location(0), direction(7), jumpStatus(false), willFall(false), mazeOrder(false), lifes(3) {}

void CreatePlayer() {
    player.x = 320;
    player.y = 90;
    player.location = 0;
    player.lifes = 3; // Reset lives
    score = 0; // Reset score
}

void Player::BlockMoveAnimation(char key, int goal_x, int goal_y) {
    int br_x = 0, br_y = 0;

    Sleep(50);

    direction++;
    y -= 40;

    Sleep(50);

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

    direction--;
    x = goal_x;
    y = goal_y;
    Sleep(50);

    if (SquareBlocks[location].blk_clr_state == 0 && jumpStatus) {
        SquareBlocks[location].blk_clr_state = 1;
        jumpStatus = !jumpStatus;
        score += 25; // Update score when tile color is changed
    }
}

void Player::FallOffEdge(char key) {
    willFall = false;
    // Calculate the total change in x
    int x_change = key == 'l' ? -5 : key == 'r' ? 5 : key == 'u' ? 5 : key == 'd' ? -5 : 0;
    int y_change = key == 'l' ? -5 : key == 'r' ? -5 : key == 'u' ? -5 : key == 'd' ? -5 : 0;
    // Falling animation
    for (int i = 0; i < 40; ++i) {
        if (i < 10) {
            y += y_change;
            x += x_change;
        }
        else {
            if (key == 'l' || key == 'u')
                mazeOrder = true; // Set the falling flag
            y += 5;
        }
        Sleep(15);
    }
    lifes--; // Decrease life on fall
    if (lifes <= 0) {
        gameRunning = false; // End game if no lives left
        ShowGameOverScreen();
        return;
    }
    // Reset player position to the top of the pyramid
    x = 320;
    y = 90;
    location = 0;
    direction = 7;
    mazeOrder = false; // Reset the falling flag
}

void Player::move(char key) {
    int block_id = 0;
    jumpStatus = true;
    switch (key)
    {
    case 'l':
        direction = 3;
        if (SquareBlocks[location].left >= 0) {
            //Also checks disk is available to jump
            SquareBlocks[location].left == 40 ? block_id = location : location = SquareBlocks[location].left;
        }
        else {
            jumpStatus = false;
            willFall = true;
        }
        break;
    case 'r':
        direction = 5;
        if (SquareBlocks[location].right >= 0) {
            location = SquareBlocks[location].right;
        }
        else {
            jumpStatus = false;
            willFall = true;
        }
        break;
    case 'd':
        direction = 7;
        if (SquareBlocks[location].down >= 0) {
            location = SquareBlocks[location].down;
        }
        else {
            jumpStatus = false;
            willFall = true;
        }
        break;
    case 'u':
        direction = 1;
        if (SquareBlocks[location].up >= 0) {
            //Also checks disk is available to jump
            SquareBlocks[location].up == 45 ? block_id = location : location = SquareBlocks[location].up;
        }
        else {
            jumpStatus = false;
            willFall = true;
        }
        break;
    default:
        break;
    }
    keypressed = 0;
    if (willFall) {
        FallOffEdge(key);
        return;
    }

    if (jumpStatus) {
        if (block_id != 0) {
            for (int i = 0; i < 2; i++) {
                if (Discs[i].block_id == block_id) {
                    BlockMoveAnimation(key, Discs[i].x - 20, Discs[i].y - 20);
                }
            }

        }
        else {
            BlockMoveAnimation(key, SquareBlocks[location].x + 20, SquareBlocks[location].y - 10);
        }
    }
    else {
        BlockMoveAnimation(key, x + (key == 'l' ? -20 : key == 'r' ? 20 : key == 'w' ? -20 : key == 's' ? 20 : 0), y + (key == 'u' ? -10 : key == 'd' ? 10 : 0));
    }
}