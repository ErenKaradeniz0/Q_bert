#include "PrintHelper.h"
#include "icb_gui.h"

extern ICBYTES Sprites3X;
extern int FRM1;
extern bool gameRunning;

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
    { 240, 3, 45, 45},      //  Qbert Character - idx 43
};

void DrawStartupAnimation1(bool* gameRunningPtr) {
    ICBYTES startScreen, QbertTitle, PinkC, QB;
    CreateImage(startScreen, 700, 700, ICB_UINT);

    CreateImage(QbertTitle, IntroCoordinates.I(3, 38), IntroCoordinates.I(4, 38), ICB_UINT);
    CreateImage(PinkC, IntroCoordinates.I(3, 40), IntroCoordinates.I(4, 40), ICB_UINT);
    CreateImage(QB, IntroCoordinates.I(3, 43), IntroCoordinates.I(4, 43), ICB_UINT);
    for (int frame = 0; frame < 240; frame++) {
        startScreen = 0x1A0F5F;

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
            RenderString(startScreen, "12", 130, 283);
            RenderString(startScreen, "2025", 130, 283);
            RenderString(startScreen, "ESD STUDIOS", 280, 280);
            RenderString(startScreen, "ALL RIGHTS RESERVED", 130, 320);
        }

        if (frame > 90) {
            RenderString(startScreen, "1 COIN = 1 PLAY", 150, 500);
            Copy(Sprites3X,
                IntroCoordinates.I(1, 43),
                IntroCoordinates.I(2, 43),
                IntroCoordinates.I(3, 43),
                IntroCoordinates.I(4, 43),
                QB);
            PasteNon0(QB, 350, 600, startScreen);
        }

        DisplayImage(FRM1, startScreen);
        Sleep(33);
    }
}

void DrawStartupAnimation2(bool* gameRunningPtr) {
    ICBYTES startScreen, QbertTitle, QB;
    CreateImage(startScreen, 700, 700, ICB_UINT);

    CreateImage(QbertTitle, IntroCoordinates.I(3, 38), IntroCoordinates.I(4, 38), ICB_UINT);
    CreateImage(QB, IntroCoordinates.I(3, 43), IntroCoordinates.I(4, 43), ICB_UINT);

    const float gravity = 1.5f;
    const float timeToTarget = 20.0f;
    const float initialVy = -12.0f;

    struct JumpSegment {
        int startX, startY;
        int targetX, targetY;
        int startFrame;
        float vx;
    };

    JumpSegment jumps[] = {
        {50, 80, 65, 180, 40},
        {65, 180, 90, 330, 90},
        {90, 330, 115, 480, 120}
    };

    for (auto& jump : jumps) {
        jump.vx = (jump.targetX - jump.startX) / timeToTarget;
    }

    MSG msg;
    for (int frame = 0; frame < 500 && *gameRunningPtr; frame++) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                *gameRunningPtr = false;
                return;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        startScreen = 0x1A0F5F;

        if (frame > 30) {
            Copy(Sprites3X,
                IntroCoordinates.I(1, 38),
                IntroCoordinates.I(2, 38),
                IntroCoordinates.I(3, 38),
                IntroCoordinates.I(4, 38),
                QbertTitle);
            PasteNon0(QbertTitle, 20, 20, startScreen);

            Copy(Sprites3X,
                IntroCoordinates.I(1, 43),
                IntroCoordinates.I(2, 43),
                IntroCoordinates.I(3, 43),
                IntroCoordinates.I(4, 43),
                QB);

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

        if (frame > 60) {
            RenderString(startScreen, "JUMP ON SQUARES TO", 120, 120);
            RenderString(startScreen, "CHANGE THEM TO", 120, 150);
            RenderString(startScreen, "THE TARGET COLOR", 120, 180);
        }

        if (frame > 90) {
            RenderString(startScreen, "STAY ON PLAYFIELD", 145, 240);
            RenderString(startScreen, "JUMPING OFF RESULTS", 145, 270);
            RenderString(startScreen, "IN A FATAL PLUMMET", 145, 300);
            RenderString(startScreen, "UNLESS A DISK IS THERE", 145, 330);
        }

        if (frame > 120) {
            RenderString(startScreen, "AVOID ALL OBJECTS", 170, 390);
            RenderString(startScreen, "AND CREATURES THAT", 170, 420);
            RenderString(startScreen, "ARE NOT GREEN", 170, 450);
        }

        if (frame > 150) {
            RenderString(startScreen, "USE SPINNING DISKS", 195, 510);
            RenderString(startScreen, "TO LURE SNAKE TO", 195, 540);
            RenderString(startScreen, "HIS DEATH", 195, 570);
        }

        DisplayImage(FRM1, startScreen);
        Sleep(33);
    }
}
