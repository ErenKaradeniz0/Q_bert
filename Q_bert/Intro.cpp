#include "icb_gui.h"
#include "PrintHelper.h"
#include "Sound.h"
#include "Main.h"
#include "Game.h"
#include "Intro.h"

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstring>

using namespace std;

extern ICBYTES Sprites3X;
extern int FRM1;
extern ICBYTES screenMatrix;
ICBYTES QbertTitle, PinkC, QB, IntroletterSprite;
ICBYTES normalBall, bouncedBall, QbertText, diskSprite;
ICBYTES Qbert, Tile, LeftAr, RightAr, ChText, PlayerText, PlayerOne, QbertHS1, QbertHS2;

ICBYTES IntroCoordinates{
    // Numbers (0-9)
    { 387, 6, 21, 21},      //  0 - idx 1
    { 414, 6, 21, 21},      //  1 - idx 2
    { 435, 6, 21, 21},      //  2 - idx 3
    { 459, 6, 21, 21},      //  3 - idx 4
    { 483, 6, 21, 21},      //  4 - idx 5
    { 507, 6, 21, 21},      //  5 - idx 6
    { 531, 6, 21, 21},      //  6 - idx 7
    { 555, 6, 21, 21},      //  7 - idx 8
    { 579, 6, 21, 21},      //  8 - idx 9
    { 606, 6, 21, 21},      //  9 - idx 10

    // Letters (A-Z)
    { 387, 27, 21, 21},     //  A - idx 11
    { 411, 27, 21, 21},     //  B - idx 12
    { 435, 27, 21, 21},     //  C - idx 13
    { 459, 27, 21, 21},     //  D - idx 14
    { 483, 27, 21, 21},     //  E - idx 15
    { 507, 27, 21, 21},     //  F - idx 16
    { 531, 27, 21, 21},     //  G - idx 17
    { 555, 27, 21, 21},     //  H - idx 18
    { 579, 27, 15, 21},     //  I - idx 19
    { 603, 27, 21, 21},     //  J - idx 20
    { 627, 27, 21, 21},     //  K - idx 21
    { 651, 27, 21, 21},     //  L - idx 22
    { 675, 27, 21, 21},     //  M - idx 23
    { 699, 27, 21, 21},     //  N - idx 24
    { 723, 27, 21, 21},     //  O - idx 25
    { 747, 27, 21, 21},     //  P - idx 26
    { 771, 27, 21, 21},     //  Q - idx 27
    { 795, 27, 21, 21},     //  R - idx 28
    { 819, 27, 21, 21},     //  S - idx 29
    { 843, 27, 21, 21},     //  T - idx 30
    { 867, 27, 21, 21},     //  U - idx 31
    { 891, 27, 21, 21},     //  V - idx 32
    { 915, 27, 21, 21},     //  W - idx 33
    { 939, 27, 21, 21},     //  X - idx 34
    { 963, 27, 21, 21},     //  Y - idx 35
    { 987, 27, 21, 21},     //  Z - idx 36

    // Special Elements
    { 846, 129, 15, 15},     //  = - idx 37
    { 387, 99, 282, 45},     //  QBERT Logo - idx 38
    { 387, 249, 150, 78},    //  Qbert text - idx 39
    { 816, 121, 27, 24},     //  Pink c Logo - idx 40
    { 6, 72, 45, 28},        //  Bounced Ball - idx 41
    { 54, 66, 45, 32},       //  Normal Ball - idx 42
    { 240, 3, 47, 45},       //  Qbert Character - idx 43
    { 720, 438, 144, 18},    //  Change to Text - idx 44
    { 795, 354, 21, 24},     //  -> Symbol - idx 45
    { 819, 354, 21, 24},     //  <- Symbol - idx 46
    { 195, 588, 45, 39},     //  Yellow Tile - idx 47
    { 1, 1068, 48, 30},      //  Disk Frame 1 - idx 48
    { 49, 1068, 48, 30},     //  Disk Frame 2 - idx 49
    { 97, 1068, 48, 30},     //  Disk Frame 3 - idx 50
    { 145, 1068, 48, 30},    //  Disk Frame 4 - idx 51
    { 672, 96, 24, 30 },     //  Mini Qbert - idx 52

    // Player Text
    { 552, 337, 153, 24},    //  Player Text 1 - idx 53
    { 552, 361, 153, 24},    //  Player Text 2 - idx 54
    { 552, 385, 153, 24},    //  Player Text 3 - idx 55
    { 552, 409, 153, 24},    //  Player Text 4 - idx 56
    { 552, 433, 153, 24},    //  Player Text 5 - idx 57
    { 552, 457, 153, 24},    //  Player Text 6 - idx 58
    { 723, 355, 21, 30},     //  Player one  - idx 59

    { 192, 6, 47, 45},       //  Bowing Qbert Character  - idx 60
    { 1152, 240, 81, 96},    //  HighScore Qbert Character 1 - idx 61
    { 1236, 240, 81, 96}     //  HighScore Qbert Character 2 - idx 62

};

vector<pair<string, int>> highScores;
char currentName[4] = "AAA";
int currentLetterIndex = 0;

void LoadHighScores() {
    ifstream file("highscores.txt");
    if (file.is_open()) {
        string name;
        int score;
        while (file >> name >> score) {
            if (name.length() > 3) name = name.substr(0, 3);
            highScores.push_back({ name, score });
        }
        file.close();
    }
}

void SaveHighScores() {
    ofstream file("highscores.txt");
    if (file.is_open()) {
        for (const auto& score : highScores) {
            file << score.first << " " << score.second << "\n";
        }
        file.close();
    }
}

void ShowHighScoreScreen(int currentScore) {
    highScores.clear();
    LoadHighScores();

    Copy(Sprites3X,
        IntroCoordinates.I(1, 61),
        IntroCoordinates.I(2, 61),
        IntroCoordinates.I(3, 61),
        IntroCoordinates.I(4, 61),
        QbertHS1);

    Copy(Sprites3X,
        IntroCoordinates.I(1, 62),
        IntroCoordinates.I(2, 62),
        IntroCoordinates.I(3, 62),
        IntroCoordinates.I(4, 62),
        QbertHS2);

    bool isEnteringName = false;
    if (highScores.size() < 5 || currentScore > highScores.back().second) {
        isEnteringName = true;
        currentLetterIndex = 0;
        strcpy_s(currentName, "AAA");
    }

    int frame = 0;

    while (Game::GetState() != Stopped) {
        screenMatrix = 0x1A0F5F;

        PasteNon0(QbertHS1, 480, 50, screenMatrix);
        PasteNon0(QbertHS2, 110, 50, screenMatrix);

        RenderString(screenMatrix, "HIGH SCORES", 205, 100);

        int startY = 150;
        for (size_t i = 0; i < highScores.size(); i++) {
            char buffer[50];

            sprintf_s(buffer, sizeof(buffer), "%d. %s", (int)(i + 1), highScores[i].first.c_str());
            RenderString(screenMatrix, buffer, 200, startY + i * 40);

            sprintf_s(buffer, sizeof(buffer), "%d", highScores[i].second);
            RenderString(screenMatrix, buffer, 400, startY + i * 40);
        }

        if (isEnteringName) {
            RenderString(screenMatrix, "ENTER YOUR NAME:", 150, 400);

            for (int i = 0; i < 3; i++) {
                char letterStr[2] = { currentName[i], '\0' };
                if (i == currentLetterIndex && frame % 30 < 15) {
                    RenderString(screenMatrix, "_", 300 + i * 30, 450);
                }
                else {
                    RenderString(screenMatrix, letterStr, 300 + i * 30, 450);
                }
            }

            if (keypressed != 0) {
                if (keypressed >= 'A' && keypressed <= 'Z') {
                    currentName[currentLetterIndex] = keypressed;
                    if (currentLetterIndex < 2) currentLetterIndex++;
                }
                else if (keypressed >= 'a' && keypressed <= 'z') {
                    currentName[currentLetterIndex] = toupper(keypressed);
                    if (currentLetterIndex < 2) currentLetterIndex++;
                }
                else if (keypressed == VK_LEFT && currentLetterIndex > 0) {
                    currentLetterIndex--;
                }
                else if (keypressed == VK_RIGHT && currentLetterIndex < 2) {
                    currentLetterIndex++;
                }
                else if (keypressed == VK_BACK && currentLetterIndex > 0) {
                    currentLetterIndex--;
                    currentName[currentLetterIndex] = 'A';
                }
                else if (keypressed == VK_RETURN) {
                    string name(currentName, 3);
                    highScores.push_back({ name, currentScore });
                    sort(highScores.begin(), highScores.end(),
                        [](const auto& a, const auto& b) { return a.second > b.second; });
                    if (highScores.size() > 5) {
                        highScores.resize(5);
                    }
                    SaveHighScores();
                    isEnteringName = false;
                }
                keypressed = 0;
            }
        }
        else {
            if (frame % 60 < 30) {
                RenderString(screenMatrix, "PRESS ENTER TO CONTINUE", 75, 500);
            }

            if (keypressed == VK_RETURN) {
                Game::Stop();
                break;
            }
        }
        Impress12x20(screenMatrix, 20, 625, "github:ErenKaradeniz0", 0x000000);
        Impress12x20(screenMatrix, 20, 650, "github:svvlgr", 0x000000);
        Impress12x20(screenMatrix, 20, 675, "github:UlasDenizCakmazel", 0x000000);

        DisplayImage(FRM1, screenMatrix);
        Game::SleepI(33);
        frame++;
    }
}

void DrawSideObjects() {
    static int currentFrame = 0;
    static int playerTextFrame = 53;

    // Copy sprites once
    Copy(Sprites3X,
        IntroCoordinates.I(1, 38),
        IntroCoordinates.I(2, 38),
        IntroCoordinates.I(3, 38),
        IntroCoordinates.I(4, 38),
        Qbert);

    Copy(Sprites3X,
        IntroCoordinates.I(1, 47),
        IntroCoordinates.I(2, 47),
        IntroCoordinates.I(3, 47),
        IntroCoordinates.I(4, 47),
        Tile);

    Copy(Sprites3X,
        IntroCoordinates.I(1, 46),
        IntroCoordinates.I(2, 46),
        IntroCoordinates.I(3, 46),
        IntroCoordinates.I(4, 46),
        LeftAr);

    Copy(Sprites3X,
        IntroCoordinates.I(1, 45),
        IntroCoordinates.I(2, 45),
        IntroCoordinates.I(3, 45),
        IntroCoordinates.I(4, 45),
        RightAr);

    Copy(Sprites3X,
        IntroCoordinates.I(1, 44),
        IntroCoordinates.I(2, 44),
        IntroCoordinates.I(3, 44),
        IntroCoordinates.I(4, 44),
        ChText);

    if (currentFrame % 3 == 0) {
        playerTextFrame++;
        if (playerTextFrame > 58) {
            playerTextFrame = 53;
        }
    }

    Copy(Sprites3X,
        IntroCoordinates.I(1, playerTextFrame),
        IntroCoordinates.I(2, playerTextFrame),
        IntroCoordinates.I(3, playerTextFrame),
        IntroCoordinates.I(4, playerTextFrame),
        PlayerText);

    Copy(Sprites3X,
        IntroCoordinates.I(1, 59),
        IntroCoordinates.I(2, 59),
        IntroCoordinates.I(3, 59),
        IntroCoordinates.I(4, 59),
        PlayerOne);

    currentFrame++;

    // Draw Q*bert logo and TM
    PasteNon0(Qbert, 380, 10, screenMatrix);
    ICG_SetFont(20, 0, "Arial");
    Impress12x20(screenMatrix, 670, 10, "TM", 0xFFD700);

    // Draw Choose text
    PasteNon0(ChText, 25, 80, screenMatrix);

    // Arrow animation
    int animPhase = currentFrame % 20;

    if (animPhase < 10) {
        PasteNon0(RightAr, 15, 100, screenMatrix);
        PasteNon0(LeftAr, 155, 100, screenMatrix);
    }

    if (animPhase >= 5 && animPhase < 15) {
        PasteNon0(RightAr, 40, 100, screenMatrix);
        PasteNon0(LeftAr, 130, 100, screenMatrix);
    }

    // Animated tile
    int tileY = 100 + (int)(5 * sin(currentFrame * 0.2f));
    PasteNon0(Tile, 75, tileY, screenMatrix);


    // player text
    PasteNon0(PlayerText, 10, 10, screenMatrix);

    // one 
    PasteNon0(PlayerOne, 170, 0, screenMatrix);

}

void DrawStartupAnimation() {
    // Disk animation constants
    const int DISK_FRAME1 = 1;
    const int DISK_FRAME2 = 16 * 3 + 1;
    const int DISK_FRAME3 = 32 * 3 + 1;
    const int DISK_FRAME4 = 48 * 3 + 1;
    const int DISK_Y = 356 * 3 + 1;
    const int DISK_WIDTH = 16 * 3;
    const int DISK_HEIGHT = 10 * 3;

    // Physics constants for second animation
    const float gravity = 1.6f;
    const float timeToTarget = 20.0f;
    const float initialVy = -12.0f;

    // Jump segment structure
    struct JumpSegment {
        int startX, startY;
        int targetX, targetY;
        int startFrame;
        float vx;
    };

    // Initialize jump segments with updated frame timings
    JumpSegment jumps[] = {
        {50, 80, 65, 180, 220},
        {65, 180, 90, 330, 270},
        {90, 330, 115, 450, 300},
        {115, 480, 140, 570, 330}
    };

    // Calculate horizontal velocities
    for (auto& jump : jumps) {
        jump.vx = (jump.targetX - jump.startX) / timeToTarget;
    }

    // Combined animation loop - extended to 480 frames total
    for (int frame = 0; frame < 480; frame++) {
        if (Game::GetState() == Stopped)
            return;

        PlayStartupSounds(frame);

        // Set background color
        screenMatrix = 0x1A0F5F;

        // First Animation (0-180 frames)
        if (frame < 180) {
            if (frame > 30) {
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 38),
                    IntroCoordinates.I(2, 38),
                    IntroCoordinates.I(3, 38),
                    IntroCoordinates.I(4, 38),
                    QbertTitle);

                int yOffset = (frame % 16 > 8) ? 2 : -2;
                PasteNon0(QbertTitle, 220, 200 + yOffset, screenMatrix);

                ICG_SetFont(20, 0, "Arial");
                Impress12x20(screenMatrix, 520, 190 + yOffset, "TM", 0xFFD700);
            }

            if (frame > 60) {
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 40),
                    IntroCoordinates.I(2, 40),
                    IntroCoordinates.I(3, 40),
                    IntroCoordinates.I(4, 40),
                    PinkC);
                PasteNon0(PinkC, 100, 280, screenMatrix);

                RenderString(screenMatrix, "2025", 130, 283);
                RenderString(screenMatrix, "ESD STUDIOS", 280, 280);
                RenderString(screenMatrix, "ALL RIGHTS RESERVED", 130, 320);
            }
            if (frame > 90 && frame <= 180) {
                RenderString(screenMatrix, "1 COIN = 1 PLAY", 150, 500);

                // Calculate disk position and animation frame
                float progress = (frame - 90) / 90.0f;  // 90 frames for movement
                int diskX = static_cast<int>(progress * 700);

                // Select the appropriate disk frame based on animation timing
                int diskFrameOffset;
                switch ((frame / 5) % 4) {
                case 0: diskFrameOffset = DISK_FRAME1; break;
                case 1: diskFrameOffset = DISK_FRAME2; break;
                case 2: diskFrameOffset = DISK_FRAME3; break;
                default: diskFrameOffset = DISK_FRAME4; break;
                }

                // Copy and paste the current disk frame
                Copy(Sprites3X,
                    diskFrameOffset, DISK_Y,
                    DISK_WIDTH, DISK_HEIGHT,
                    diskSprite);
                PasteNon0(diskSprite, diskX, 600, screenMatrix);

                // Draw Q*bert on top of the disk
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 43),
                    IntroCoordinates.I(2, 43),
                    IntroCoordinates.I(3, 43),
                    IntroCoordinates.I(4, 43),
                    QB);
                PasteNon0(QB, diskX + 10, 570, screenMatrix);
            }
        }
        // Second Animation (180+ frames)
        else {
            if (frame > 210) {
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 38),
                    IntroCoordinates.I(2, 38),
                    IntroCoordinates.I(3, 38),
                    IntroCoordinates.I(4, 38),
                    QbertTitle);
                PasteNon0(QbertTitle, 20, 20, screenMatrix);
                ICG_SetFont(20, 0, "Arial");
                Impress12x20(screenMatrix, 320, 15, "TM", 0xFFD700);

                // Q*bert jumping animation
                int currentX = jumps[0].startX;
                int currentY = jumps[0].startY;

                for (const auto& jump : jumps) {
                    if (frame >= jump.startFrame && frame < jump.startFrame + 20) {
                        float t = frame - jump.startFrame;
                        currentX = jump.startX + jump.vx * t;
                        currentY = jump.startY + initialVy * t + (gravity * t * t) / 2;
                        int rotationOffset = (frame % 8 > 4) ? 3 : -3;
                        currentY += rotationOffset;
                        break;
                    }
                    else if (frame >= jump.startFrame + 20) {
                        currentX = jump.targetX;
                        currentY = jump.targetY;
                        if (frame < jump.startFrame + 30) {
                            int bounceOffset = (frame % 6 > 3) ? 4 : 0;
                            currentY -= bounceOffset;
                        }
                    }
                }

                if (frame >= 390 && frame < 400) {
                    Copy(Sprites3X,
                        IntroCoordinates.I(1, 60),  // Using Bowing Qbert
                        IntroCoordinates.I(2, 60),
                        IntroCoordinates.I(3, 60),
                        IntroCoordinates.I(4, 60),
                        QB);
                }
                else {
                    Copy(Sprites3X,
                        IntroCoordinates.I(1, 43),  // Using normal Q*bert
                        IntroCoordinates.I(2, 43),
                        IntroCoordinates.I(3, 43),
                        IntroCoordinates.I(4, 43),
                        QB);
                }
                PasteNon0(QB, currentX, currentY, screenMatrix);
            }

            if (frame > 240) {
                RenderString(screenMatrix, "JUMP ON SQUARES TO", 120, 120);
                RenderString(screenMatrix, "CHANGE THEM TO", 120, 150);
                RenderString(screenMatrix, "THE TARGET COLOR", 120, 180);
            }

            if (frame > 270) {
                RenderString(screenMatrix, "STAY ON PLAYFIELD", 145, 240);
                RenderString(screenMatrix, "JUMPING OFF RESULTS", 145, 270);
                RenderString(screenMatrix, "IN A FATAL PLUMMET", 145, 300);
                RenderString(screenMatrix, "UNLESS A DISK IS THERE", 145, 330);
            }

            if (frame > 300) {
                RenderString(screenMatrix, "AVOID ALL OBJECTS", 170, 390);
                RenderString(screenMatrix, "AND CREATURES THAT", 170, 420);
                RenderString(screenMatrix, "ARE NOT GREEN", 170, 450);
            }

            if (frame > 330) {
                RenderString(screenMatrix, "USE SPINNING DISKS", 195, 510);
                RenderString(screenMatrix, "TO LURE SNAKE TO", 195, 540);
                RenderString(screenMatrix, "HIS DEATH", 195, 570);
            }

            // Ball animation (360-420 frames)
            if (frame > 360 && frame <= 390) {
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 42),
                    IntroCoordinates.I(2, 42),
                    IntroCoordinates.I(3, 42),
                    IntroCoordinates.I(4, 42),
                    normalBall);

                float ballProgress = (frame - 360) / 30.0f;
                float easedProgress = ballProgress * ballProgress;  // Quadratic easing
                int ballY = 80 + (540 - 80) * easedProgress;

                if (ballY >= 540) {
                    ballY = 540;
                    Copy(Sprites3X,
                        IntroCoordinates.I(1, 41),
                        IntroCoordinates.I(2, 41),
                        IntroCoordinates.I(3, 41),
                        IntroCoordinates.I(4, 41),
                        bouncedBall);
                    PasteNon0(bouncedBall, 140, ballY, screenMatrix);
                }
                else {
                    PasteNon0(normalBall, 140, ballY, screenMatrix);
                }
            }

            if (frame > 390 && frame <= 420) {
                if (frame <= 393) {
                    Copy(Sprites3X,
                        IntroCoordinates.I(1, 41),
                        IntroCoordinates.I(2, 41),
                        IntroCoordinates.I(3, 41),
                        IntroCoordinates.I(4, 41),
                        bouncedBall);
                    PasteNon0(bouncedBall, 140, 540, screenMatrix);
                }
                else {
                    float t = (frame - 393) / 27.0f;
                    if (t > 1.0f) t = 1.0f;

                    int bounceX = 140 + 100 * t;
                    int bounceY = 540 - 100 * t + 300 * t * t;

                    if (bounceY < 700) {
                        Copy(Sprites3X,
                            IntroCoordinates.I(1, 42),
                            IntroCoordinates.I(2, 42),
                            IntroCoordinates.I(3, 42),
                            IntroCoordinates.I(4, 42),
                            normalBall);
                        PasteNon0(normalBall, bounceX, bounceY, screenMatrix);
                    }
                }
            }

            if (frame > 420) {
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 39),
                    IntroCoordinates.I(2, 39),
                    IntroCoordinates.I(3, 39),
                    IntroCoordinates.I(4, 39),
                    QbertText);

                float alpha = (frame - 420) / 30.0f;
                if (alpha > 1.0f) alpha = 1.0f;

                PasteNon0(QbertText, 120, 490, screenMatrix);
            }
        }

        // Display frame and wait
        DisplayImage(FRM1, screenMatrix);
        if (isAnimationFaster)
            Game::SleepI(0);
        else
            Game::SleepI(33);
    }
}