#include "Sound.h"
#include "Game.h"

VOID* HelloSound() {
    PlaySound("Sounds/Hello.wav", NULL, SND_ASYNC);
    return 0;
}

VOID* LiftSound() {
    PlaySound("Sounds/Lift.wav", NULL, SND_ASYNC);
    return 0;
}

VOID* JumpSound() {
    PlaySound("Sounds/Jump.wav", NULL, SND_ASYNC);
    return 0;
}

VOID* SpeechSound() {
    PlaySound("Sounds/Speech.wav", NULL, SND_ASYNC);
    return 0;
}

VOID* PlayerFallSound() {
    PlaySound("Sounds/PlayerFall.wav", NULL, SND_ASYNC);
    return 0;
}

bool helloPlayed = false;
bool liftPlayed = false;
bool speechPlayed = false;
bool jumpSoundPlayed[4] = { false, false, false, false }; // Her z�plama i�in bir flag

void PlayStartupSounds(int currentFrame) {
    // Hello sesi i�in kontrol (frame 30)
    if (currentFrame == 30 && !helloPlayed) {
        HelloSound();
        helloPlayed = true;
    }

    // Lift sesi i�in kontrol (frame 90)
    if (currentFrame == 90 && !liftPlayed) {
        LiftSound();
        liftPlayed = true;
    }

    // Z�plama sesleri i�in kontrol
    // Her z�plama ba�lang�c�nda ses �al
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

    // QBert text i�in speech sesi
    if (currentFrame == 420 && !speechPlayed) {
        SpeechSound();
        speechPlayed = true;
    }
}