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

Player::Player() : x(0), y(0), currentTile(SquareBlock()), direction(7), willFall(false), mazeOrder(false), lifes(3){}

void CreatePlayer() {
    player.x = SquareBlocks[0].centerX;
    player.y = SquareBlocks[0].centerY;
    player.currentTile = SquareBlocks[0];
    player.lifes = 3; // Reset lives
    score = 0; // Reset score
	player.mazeOrder = false; // Reset the falling flag
}

void Player::MoveAnimation(char key, int goal_x, int goal_y) {
    int br_x = 0, br_y = 0;
	player.playJumpSound = true; 

    direction++;
    player.y -= 40;

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

    if (key == 'l') {
        currentTile = *currentTile.left;
    }
    else if (key == 'u') {
        currentTile = *currentTile.up;
    }
    else if (key == 'd') {
        currentTile = *currentTile.down;
    }
    else if (key == 'r') {
        currentTile = *currentTile.right;
    }

    willFall = false;

    if (SquareBlocks[currentTile.id].state == 0) {
        SquareBlocks[currentTile.id].state = 2;
        score += 25; // Update score when tile color is changed
    }
}

void Player::lostLife(bool isFall) {

    lifes--; // Decrease life
    Game::SleepI(100);
	enemyBall1.isAlive = false;
	enemyBall1.currentTile.id = -1;
	enemyBall2.isAlive = false;
    enemyBall1.currentTile.id = -1;
	enemySnake.isAlive = false;
    enemyBall1.currentTile.id = -1;
    if (isFall) {
        player.x = SquareBlocks[0].centerX;
        player.y = SquareBlocks[0].centerY;
        player.currentTile = SquareBlocks[0];
        direction = 7;
        mazeOrder = false; // Reset the falling flag
    }
    else {
		player.showLostLifeText = true;
        player.playSpeechSound = true;

    }
}

void Player::FallOffEdge(char key) {
    willFall = false;
	player.playFallingSound = true;

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
    
    if (!Discs[disc_id].show_state) {
        FallOffEdge(disc_id == 0 ? 'l' : 'u');
        return;
    }

	player.currentTile.id = Discs[disc_id].disc_id;
    int br_x = 0, br_y = 0;
    direction++;
    player.y -= 40;
    player.x < goal_x ? br_x = 5 : br_x = -5;
    player.y < goal_y ? br_y = 5 : br_y = -5;

    player.playJumpSound = true;

    while (br_x < 0 ? player.x >= goal_x : player.x < goal_x || br_y < 0 ? player.y >= goal_y : player.y < goal_y) {
        Game::SleepI(15);

        if (br_x < 0 ? player.x >= goal_x : player.x < goal_x) {
            player.x +=  br_x;
        }
        if (br_y < 0 ? player.y >= goal_y : player.y < goal_y) {
            player.y += br_y;
        }
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

    player.playDiscSound = true;

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
    currentTile = SquareBlocks[0];

    Discs[disc_id].show_state = false;
   
    if (SquareBlocks[currentTile.id].state == 0) {
        SquareBlocks[currentTile.id].state = 2;
        score += 25; // Update score when tile color is changed
    }


}

void Player::move(char key) {
    willFall = true;
    switch (key)
    {
    case 'l':
        direction = 3;
        if (currentTile.left != nullptr) {
            //Also checks disk is available to jump
            MoveAnimation(key, currentTile.left->centerX, currentTile.left->centerY);
        }
        break;
    case 'r':
        direction = 5;
        if (currentTile.right != nullptr) {
            MoveAnimation(key, currentTile.right->centerX, currentTile.right->centerY);
        }
        break;
    case 'd':
        direction = 7;
        if (currentTile.down != nullptr) {
            MoveAnimation(key, currentTile.down->centerX, currentTile.down->centerY);
        }
        break;
    case 'u':
        direction = 1;
        if (currentTile.up != nullptr) {
            //Also checks disk is available to jump
            MoveAnimation(key, currentTile.up->centerX, currentTile.up->centerY);
        }
        break;
    default:
        break;
    }
    keypressed = 0;


    if (currentTile.id == 10 && key == 'l') {
        if (Discs[0].show_state) {  // Only use disc if it's still available
            keyPressedControl = false;
            JumpDiscAnimation(0, Discs[0].center_x - 12, Discs[0].center_y - 30);
            keyPressedControl = true;
            willFall = false;
        }
        else {
            // Disc was already used, Q*bert should fall
            FallOffEdge(key);
            return;
        }
    }
    else if (currentTile.id == 14 && key == 'u') {
        if (Discs[1].show_state) {  // Only use disc if it's still available
            keyPressedControl = false;
            JumpDiscAnimation(1, Discs[1].center_x - 12, Discs[1].center_y - 30);
            keyPressedControl = true;
            willFall = false;
        }
        else {
            // Disc was already used, Q*bert should fall
            FallOffEdge(key);
            return;
        }
    }

    if (willFall) {
        FallOffEdge(key);
        return;
    }
}

