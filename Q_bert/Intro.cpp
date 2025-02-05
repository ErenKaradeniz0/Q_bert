#include "PrintHelper.h"
#include "icb_gui.h"
#include <xmemory>

extern ICBYTES Sprites3X;
extern int FRM1;
extern bool gameRunning;
extern ICBYTES screenMatrix;

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
    { 846, 129, 15, 15},    //  = - idx 37
    { 387, 99, 282, 45},    //  QBERT Logo - idx 38
    { 387, 249, 150, 78},   //  Qbert text - idx 39
    { 816, 121, 27, 24},    //  Pink c Logo - idx 40
    { 6, 72, 45, 30},       //  Bounced Ball - idx 41
    { 54, 66, 45, 30},      //  Normal Ball - idx 42
    { 240, 3, 47, 45},      //  Qbert Character - idx 43
    { 720, 438, 144, 48},    //  Change to Text - idx 44
    { 795, 354, 21, 24},       //  -> Symbol - idx 45
    { 819, 354, 21, 24},      //  <- Symbol - idx 46
    { 195, 588, 45, 39},      //  Yellow Tile - idx 47

};


void DrawLogo() {
    static ICBYTES Qbert, Tile, LeftAr, RightAr, ChText;
    static bool initialized = false;
    static int currentFrame = 0;

    // One-time initialization
    if (!initialized) {
        CreateImage(Qbert, IntroCoordinates.I(3, 38), IntroCoordinates.I(4, 38), ICB_UINT);
        CreateImage(Tile, IntroCoordinates.I(3, 47), IntroCoordinates.I(4, 47), ICB_UINT);
        CreateImage(LeftAr, IntroCoordinates.I(3, 46), IntroCoordinates.I(4, 46), ICB_UINT);
        CreateImage(RightAr, IntroCoordinates.I(3, 45), IntroCoordinates.I(4, 45), ICB_UINT);
        CreateImage(ChText, IntroCoordinates.I(3, 44), IntroCoordinates.I(4, 44), ICB_UINT);

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

        initialized = true;
    }

    currentFrame++;

    // Draw Q*bert logo and TM
    PasteNon0(Qbert, 380, 10, screenMatrix);
    ICG_SetFont(20, 0, "Arial");
    Impress12x20(screenMatrix, 670, 10, "TM", 0xFFD700);

    // Draw Choose text
    PasteNon0(ChText, 500, 80, screenMatrix);

    // Arrow animation
    int animPhase = currentFrame % 20;

    if (animPhase < 10) {
        PasteNon0(RightAr, 490, 110, screenMatrix);
        PasteNon0(LeftAr, 630, 110, screenMatrix);
    }

    if (animPhase >= 5 && animPhase < 15) {
        PasteNon0(RightAr, 515, 110, screenMatrix);
        PasteNon0(LeftAr, 605, 110, screenMatrix);
    }

    // Animated tile
    int tileY = 110 + (int)(5 * sin(currentFrame * 0.2f));
    PasteNon0(Tile, 550, tileY, screenMatrix);
}

void DrawStartupAnimation1(bool* gameRunningPtr) {
    // Main image buffers
    ICBYTES startScreen, QbertTitle, PinkC, QB, letterSprite;
    ICBYTES normalBall, bouncedBall, QbertText;  // Yeni bufferlar
    CreateImage(startScreen, 700, 700, ICB_UINT);
    CreateImage(QbertTitle, IntroCoordinates.I(3, 38), IntroCoordinates.I(4, 38), ICB_UINT);
    CreateImage(PinkC, IntroCoordinates.I(3, 40), IntroCoordinates.I(4, 40), ICB_UINT);
    CreateImage(QB, IntroCoordinates.I(3, 43), IntroCoordinates.I(4, 43), ICB_UINT);
    CreateImage(letterSprite, 21, 21, ICB_UINT);
    CreateImage(normalBall, IntroCoordinates.I(3, 42), IntroCoordinates.I(4, 42), ICB_UINT);  // Normal top için
    CreateImage(bouncedBall, IntroCoordinates.I(3, 41), IntroCoordinates.I(4, 41), ICB_UINT); // Zýplayan top için
    CreateImage(QbertText, IntroCoordinates.I(3, 39), IntroCoordinates.I(4, 39), ICB_UINT);   // Qbert text için


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

    // Initialize jump segments
    JumpSegment jumps[] = {
        {50, 80, 65, 180, 160},      // Offset startFrame by 120
        {65, 180, 90, 330, 210},     // Offset startFrame by 120
        {90, 330, 115, 450, 240},    // Offset startFrame by 120
        {115, 480, 140, 570, 270}    // Offset startFrame by 120
    };

    // Calculate horizontal velocities
    for (auto& jump : jumps) {
        jump.vx = (jump.targetX - jump.startX) / timeToTarget;
    }

    // Helper function to draw a single character
    auto DrawCharacter = [&](char c, int x, int y) {
        int spriteIndex = 0;

        if (c >= '0' && c <= '9') {
            spriteIndex = (c - '0') + 1;
        }
        else if (c >= 'A' && c <= 'Z') {
            spriteIndex = (c - 'A') + 11;
        }
        else if (c == '=') {
            spriteIndex = 37;
        }
        else {
            return;
        }

        Copy(Sprites3X,
            IntroCoordinates.I(1, spriteIndex),
            IntroCoordinates.I(2, spriteIndex),
            IntroCoordinates.I(3, spriteIndex),
            IntroCoordinates.I(4, spriteIndex),
            letterSprite);

        PasteNon0(letterSprite, x, y, startScreen);
        };

    // Helper function to draw text string
    auto DrawText = [&](const char* text, int x, int y, int spacing = 25) {
        int currentX = x;
        while (*text) {
            if (*text != ' ') {
                DrawCharacter(*text, currentX, y);
            }
            currentX += spacing;
            text++;
        }
        };

    // Message handling
    MSG msg;

    // Combined animation loop
    for (int frame = 0; frame < 390 && *gameRunningPtr; frame++) {
        // Process Windows messages
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                *gameRunningPtr = false;
                return;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Set background color
        startScreen = 0x1A0F5F;

        // First Animation (0-120 frames)
        if (frame < 120) {
            if (frame > 30) {
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 38),
                    IntroCoordinates.I(2, 38),
                    IntroCoordinates.I(3, 38),
                    IntroCoordinates.I(4, 38),
                    QbertTitle);

                int yOffset = (frame % 16 > 8) ? 2 : -2;
                PasteNon0(QbertTitle, 220, 200 + yOffset, startScreen);

                ICG_SetFont(20, 0, "Arial");
                Impress12x20(startScreen, 520, 190 + yOffset, "TM", 0xFFD700);
            }

            if (frame > 60) {
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 40),
                    IntroCoordinates.I(2, 40),
                    IntroCoordinates.I(3, 40),
                    IntroCoordinates.I(4, 40),
                    PinkC);
                PasteNon0(PinkC, 100, 280, startScreen);

                DrawText("2025", 130, 283);
                DrawText("ESD STUDIOS", 280, 280);
                DrawText("ALL RIGHTS RESERVED", 130, 320);
            }

            if (frame > 90) {
                DrawText("1 COIN = 1 PLAY", 150, 500);

                Copy(Sprites3X,
                    IntroCoordinates.I(1, 43),
                    IntroCoordinates.I(2, 43),
                    IntroCoordinates.I(3, 43),
                    IntroCoordinates.I(4, 43),
                    QB);
                PasteNon0(QB, 350, 600, startScreen);
            }
        }
        // Second Animation (120+ frames)
        else {
            if (frame > 150) {
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 38),
                    IntroCoordinates.I(2, 38),
                    IntroCoordinates.I(3, 38),
                    IntroCoordinates.I(4, 38),
                    QbertTitle);
                PasteNon0(QbertTitle, 20, 20, startScreen);
                ICG_SetFont(20, 0, "Arial");
                Impress12x20(startScreen, 320, 15, "TM", 0xFFD700);

                Copy(Sprites3X,
                    IntroCoordinates.I(1, 43),
                    IntroCoordinates.I(2, 43),
                    IntroCoordinates.I(3, 43),
                    IntroCoordinates.I(4, 43),
                    QB);

                // Calculate Q*bert position
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

                PasteNon0(QB, currentX, currentY, startScreen);
            }

            if (frame > 180) {
                DrawText("JUMP ON SQUARES TO", 120, 120);
                DrawText("CHANGE THEM TO", 120, 150);
                DrawText("THE TARGET COLOR", 120, 180);
            }

            if (frame > 210) {
                DrawText("STAY ON PLAYFIELD", 145, 240);
                DrawText("JUMPING OFF RESULTS", 145, 270);
                DrawText("IN A FATAL PLUMMET", 145, 300);
                DrawText("UNLESS A DISK IS THERE", 145, 330);
            }

            if (frame > 240) {
                DrawText("AVOID ALL OBJECTS", 170, 390);
                DrawText("AND CREATURES THAT", 170, 420);
                DrawText("ARE NOT GREEN", 170, 450);
            }

            if (frame > 270) {
                DrawText("USE SPINNING DISKS", 195, 510);
                DrawText("TO LURE SNAKE TO", 195, 540);
                DrawText("HIS DEATH", 195, 570);
            }

            bool ballHitHead = false;

            if (frame > 300 && frame <= 330) {  // Ýlk düþüþ animasyonu
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 42),
                    IntroCoordinates.I(2, 42),
                    IntroCoordinates.I(3, 42),
                    IntroCoordinates.I(4, 42),
                    normalBall);

                float ballProgress = (frame - 300) / 30.0f;
                if (ballProgress > 1.0f) ballProgress = 1.0f;

                int ballY = 80 + (540 - 80) * ballProgress;

                if (ballY >= 540) {  // Top Q*bert'in baþýna ulaþtýðýnda
                    ballY = 540;  // Pozisyonu sabitle
                    Copy(Sprites3X,
                        IntroCoordinates.I(1, 41),
                        IntroCoordinates.I(2, 41),
                        IntroCoordinates.I(3, 41),
                        IntroCoordinates.I(4, 41),
                        bouncedBall);
                    PasteNon0(bouncedBall, 140, ballY, startScreen);
                    ballHitHead = true;
                }
                else {
                    PasteNon0(normalBall, 140, ballY, startScreen);
                }
            }

            if (frame > 330 && ballHitHead) {  // Zýplama animasyonu
                float t = (frame - 330) / 20.0f;
                if (t > 1.0f) t = 1.0f;

                int bounceX = 140 + 50 * t;
                int bounceY = 540 - 100 * t + 200 * t * t;

                if (bounceY < 700) {
                    if (t < 0.5f) {
                        Copy(Sprites3X,
                            IntroCoordinates.I(1, 42),
                            IntroCoordinates.I(2, 42),
                            IntroCoordinates.I(3, 42),
                            IntroCoordinates.I(4, 42),
                            normalBall);
                        PasteNon0(normalBall, bounceX, bounceY, startScreen);
                    }
                    else {
                        Copy(Sprites3X,
                            IntroCoordinates.I(1, 41),
                            IntroCoordinates.I(2, 41),
                            IntroCoordinates.I(3, 41),
                            IntroCoordinates.I(4, 41),
                            bouncedBall);
                        PasteNon0(bouncedBall, bounceX, bounceY, startScreen);
                    }
                }
            }

            if (frame > 330) {
                // Zýplama animasyonu
                float t = (frame - 330) / 20.0f;  // 20 frame'de tamamlanacak
                if (t > 1.0f) t = 1.0f;

                // Parabolik hareket
                int bounceX = 140 + 50 * t;  // Saða doðru hareket
                int bounceY = 540 - 100 * t + 500 * t * t;  // Yukarý çýkýp aþaðý düþme

                Copy(Sprites3X,
                    IntroCoordinates.I(1, 42),
                    IntroCoordinates.I(2, 42),
                    IntroCoordinates.I(3, 42),
                    IntroCoordinates.I(4, 42),
                    normalBall);
                PasteNon0(normalBall, bounceX, bounceY, startScreen);

            }

            if (frame > 360) {
                // Qbert text animasyonu
                Copy(Sprites3X,
                    IntroCoordinates.I(1, 39),
                    IntroCoordinates.I(2, 39),
                    IntroCoordinates.I(3, 39),
                    IntroCoordinates.I(4, 39),
                    QbertText);

                // Fade-in efekti için alpha deðeri
                float alpha = (frame - 360) / 30.0f;  // 30 frame'de tam görünür olacak
                if (alpha > 1.0f) alpha = 1.0f;

                PasteNon0(QbertText, 120, 490, startScreen);
            }

        }

        // Display frame and wait
        DisplayImage(FRM1, startScreen);
        Sleep(33);
    }
}