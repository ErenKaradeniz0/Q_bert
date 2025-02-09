#include "Player.h"
#include "Maze.h"
#include "Printhelper.h"
#include "Sound.h"
#include "icb_gui.h" 
#include <cmath>
#include "Main.h"
#include "Game.h"

extern int FRM1;
extern int score;
extern Player player;
extern int keypressed;
extern bool keyPressedControl;

Player::Player() : x(0), y(0), currentTile(0), direction(7), jumpStatus(false), willFall(false), mazeOrder(false), lifes(3){}

void CreatePlayer() {
    player.x = SquareBlocks[0].centerX;
    player.y = SquareBlocks[0].centerY;
    player.currentTile = 0;
    player.lifes = 3; // Reset lives
    score = 0; // Reset score
	player.mazeOrder = false; // Reset the falling flag
}

void Player::MoveAnimation(char key, int goal_x, int goal_y) {
    int br_x = 0, br_y = 0;

    Game::SleepI(50);

    direction++;
    player.y -= 40;

    Game::SleepI(50);

    player.x < goal_x ? br_x = 5 : br_x = -5;
    player.y < goal_y ? br_y = 5 : br_y = -5;

    while (player.x != goal_x || player.y != goal_y) {
        if (player.x != goal_x) {
            player.x += br_x;
        }
        if (player.y != goal_y) {
            player.y += br_y;
        }
        Game::SleepI(15);
    }

    direction--;
    player.x = goal_x;
    player.y = goal_y;
    Game::SleepI(50);


    if (SquareBlocks[currentTile].blk_clr_state == 0 && jumpStatus) {
        SquareBlocks[currentTile].blk_clr_state = 1;
        jumpStatus = !jumpStatus;
        score += 25; // Update score when tile color is changed
    }

    //PlaySound("Sounds/Jump.wav", NULL, SND_ASYNC);
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)JumpSound, NULL, 0, NULL);
}

void Player::lostLife(bool isFall) {
    lifes--; // Decrease life
    Game::SleepI(100);

    if (isFall) {
        player.x = SquareBlocks[0].centerX;
        player.y = SquareBlocks[0].centerY;
        player.currentTile = 0;
        direction = 7;
        mazeOrder = false; // Reset the falling flag
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
          Game::SleepI(15);
    }
    lostLife(true); // Decrease life on fall

}

void Player::JumpDiscAnimation(int disc_id, int goal_x, int goal_y) {
    int br_x = 0, br_y = 0;
    Game::SleepI(50);

    direction++;
    player.y -= 40;
    player.x < goal_x ? br_x = 5 : br_x = -5;
    player.y < goal_y ? br_y = 5 : br_y = -5;

    Game::SleepI(50);


    while (br_x < 0 ? player.x >= goal_x : player.x < goal_x || br_y < 0 ? player.y >= goal_y : player.y < goal_y) {

        if (br_x < 0 ? player.x >= goal_x : player.x < goal_x) {
            player.x += br_x;
        }
        if (br_y < 0 ? player.y >= goal_y : player.y < goal_y) {
            player.y += br_y;
        }
        Game::SleepI(15);
    }

    direction--;
    player.x = goal_x;
    player.y = goal_y;

    Discs[disc_id].move_state = true;
    DiskAndPlayerMovingAnimation(disc_id);

}

void Player::DiskAndPlayerMovingAnimation(int disc_id) {
    int br_x, br_y;
    int goal_x = SquareBlocks[0].x + 20;
    int goal_y = SquareBlocks[0].y - 60;
    int precision = static_cast<int>(sqrt(pow(goal_x - Discs[disc_id].x, 2) + pow(goal_y - Discs[disc_id].y, 2)));
    precision /= 10;
    br_x = (goal_x - Discs[disc_id].x) / (precision+5);
    br_y = (goal_y - Discs[disc_id].y) / (precision+3);

    /*const char* soundPath = "Sounds/Lift.wav";
    CreateSoundThread(soundPath);*/

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LiftSound, NULL, 0, NULL);


    while (br_x < 0 ? Discs[disc_id].x >= goal_x : Discs[disc_id].x < goal_x || Discs[disc_id].y > goal_y) {
        if (br_x < 0 ? Discs[disc_id].x >= goal_x : Discs[disc_id].x < goal_x) {
            Discs[disc_id].x += br_x;
            player.x += br_x;
        }
        if (Discs[disc_id].y > goal_y){
            Discs[disc_id].y += br_y;
            player.y += br_y;
        }
        Game::SleepI(40);
    }
    direction = 7;
    Sleep(50);
    direction++;
    

    while (player.y < SquareBlocks[0].centerY) {
        player.y += 5;
        Sleep(15);
    }
    direction--;
    player.x = SquareBlocks[0].centerX;
    player.y = SquareBlocks[0].centerY;
    currentTile = 0;

    Discs[disc_id].show_state = false;
   
    if (SquareBlocks[currentTile].blk_clr_state == 0 && jumpStatus) {
        SquareBlocks[currentTile].blk_clr_state = 1;
        jumpStatus = !jumpStatus;
        score += 25; // Update score when tile color is changed
    }


}

void Player::move(char key) {
    int block_id = 0;
    jumpStatus = true;
    switch (key)
    {
    case 'l':
        direction = 3;
        if (SquareBlocks[currentTile].left >= 0) {
            //Also checks disk is available to jump
            SquareBlocks[currentTile].left == 40 ? block_id = currentTile : currentTile = SquareBlocks[currentTile].left;
        }
        else {
            jumpStatus = false;
            willFall = true;
        }
        break;
    case 'r':
        direction = 5;
        if (SquareBlocks[currentTile].right >= 0) {
            currentTile = SquareBlocks[currentTile].right;
        }
        else {
            jumpStatus = false;
            willFall = true;
        }
        break;
    case 'd':
        direction = 7;
        if (SquareBlocks[currentTile].down >= 0) {
            currentTile = SquareBlocks[currentTile].down;
        }
        else {
            jumpStatus = false;
            willFall = true;
        }
        break;
    case 'u':
        direction = 1;
        if (SquareBlocks[currentTile].up >= 0) {
            //Also checks disk is available to jump
            SquareBlocks[currentTile].up == 45 ? block_id = currentTile : currentTile = SquareBlocks[currentTile].up;
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
                    keyPressedControl = false;
                    JumpDiscAnimation(i,Discs[i].center_x - 12, Discs[i].center_y - 30);
                    keyPressedControl=true;
                }
            }

        }
        else {
            MoveAnimation(key, SquareBlocks[currentTile].centerX, SquareBlocks[currentTile].centerY);
        }
    }
    else {
        MoveAnimation(key, x + (key == 'l' ? -20 : key == 'r' ? 20 : key == 'w' ? -20 : key == 's' ? 20 : 0), y + (key == 'u' ? -10 : key == 'd' ? 10 : 0));
    }
}

