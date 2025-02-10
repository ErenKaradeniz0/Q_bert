#include "Sound.h"
#include "Game.h"

int SoundPlayer::soundCounter = 0;

bool helloPlayed = false;
bool liftPlayed = false;
bool introBallPlayed = false;
bool speechPlayed = false;
bool jumpSoundPlayed[4] = { false, false, false, false };

void ResetSoundFlags() {
    helloPlayed = false;
    liftPlayed = false;
    introBallPlayed = false;
    speechPlayed = false;
    for (int i = 0; i < 4; i++) {
        jumpSoundPlayed[i] = false;
    }
}

VOID* HelloSound() {
    SoundPlayer::PlaySound("Sounds/Hello.wav");
    return 0;
}

VOID* LiftSound() {
    SoundPlayer::PlaySound("Sounds/DiscSound.wav");
    return 0;
}

VOID* IntroBallSound() {
    SoundPlayer::PlaySound("Sounds/IntroBall.wav");
    return 0;
}

VOID* JumpSound() {
    SoundPlayer::PlaySound("Sounds/Jump.wav");
    return 0;
}

VOID* SpeechSound() {
    SoundPlayer::PlaySound("Sounds/Speech.wav");
    return 0;
}

VOID* PlayerFallSound() {
    SoundPlayer::PlaySound("Sounds/PlayerFall.wav");
    return 0;
}

VOID* RedBallJumpSound() {
    SoundPlayer::PlaySound("Sounds/RedBallJump.wav");
    return 0;
}

VOID* SnakeEggJumpSound() {
    SoundPlayer::PlaySound("Sounds/SnakeEggJump.wav");
    return 0;
}

VOID* SnakeJumpSound() {
    SoundPlayer::PlaySound("Sounds/SnakeJump.wav");
    return 0;
}

VOID* SnakeFallSound() {
    SoundPlayer::PlaySound("Sounds/SnakeFall.wav");
    return 0;
}

VOID* VictorySound() {
    SoundPlayer::PlaySound("Sounds/Victory.wav");
    return 0;
}


void PlayStartupSounds(int currentFrame) {
    // Hello sesi için kontrol (frame 30)
    if (currentFrame == 30 && !helloPlayed) {
        HelloSound();
        helloPlayed = true;
    }

    // Lift sesi için kontrol (frame 90)
    if (currentFrame == 90 && !liftPlayed) {
        LiftSound();
        liftPlayed = true;
    }

    // Zýplama sesleri için kontrol
    // Her zýplama baþlangýcýnda ses çal
    if (currentFrame == 240 && !jumpSoundPlayed[0]) {
        JumpSound();
        jumpSoundPlayed[0] = true;
    }
    if (currentFrame == 290 && !jumpSoundPlayed[1]) {
        JumpSound();
        jumpSoundPlayed[1] = true;
    }
    if (currentFrame == 320 && !jumpSoundPlayed[2]) {
        JumpSound();
        jumpSoundPlayed[2] = true;
    }
    if (currentFrame == 350 && !jumpSoundPlayed[3]) {
        JumpSound();
        jumpSoundPlayed[3] = true;
    }

    // QBert text için speech sesi
    if (currentFrame == 420 && !speechPlayed) {
        SpeechSound();
        speechPlayed = true;
    }

    if (currentFrame == 393 && !introBallPlayed) {
        IntroBallSound();
        introBallPlayed = true;
    }
}