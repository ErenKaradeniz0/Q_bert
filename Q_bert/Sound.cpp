#include <windows.h> 
#include <icb_gui.h>
#include "Sound.h"
#include "Game.h"

bool helloPlayed = false;
bool liftPlayed = false;
bool introBallPlayed = false;
bool speechPlayed = false;
bool jumpSoundPlayed[4] = { false, false, false, false };

ICBYTES waveHello, waveDisc, waveIntroBall, waveJumpSound, waveSpeechSound, waveFallingSound, waveDiscSound,
waveSwearingSound, waveRedBallJumpSound, waveSnakeEggJumpSound, waveSnakeJumpSound, waveplaySnakeFallSound;

void ResetSoundFlags() {
    helloPlayed = false;
    liftPlayed = false;
    introBallPlayed = false;
    speechPlayed = false;
    for (int i = 0; i < 4; i++) {
        jumpSoundPlayed[i] = false;
    }
}

void prepareWave() {
    //Intro
    ReadWave(waveHello, "Sounds/Hello.wav");
    ReadWave(waveDisc, "Sounds/DiscSound.wav");
    ReadWave(waveIntroBall, "Sounds/IntroBall.wav");
    //Player
    ReadWave(waveJumpSound, "Sounds/Jump.wav");
    ReadWave(waveFallingSound, "Sounds/PlayerFall.wav");
    ReadWave(waveSpeechSound, "Sounds/Speech.wav");
    //Enemies
    ReadWave(waveRedBallJumpSound, "Sounds/RedBallJump.wav");
    ReadWave(waveSnakeEggJumpSound, "Sounds/SnakeEggJump.wav");
    ReadWave(waveSnakeJumpSound, "Sounds/SnakeJump.wav");
    ReadWave(waveplaySnakeFallSound, "Sounds/SnakeFall.wav");
}

// Update the WaveOut calls with the correct number of arguments
VOID* HelloSound() {
    WaveOut(waveHello, 0);
    return 0;
}

VOID* DiscSound() {
    WaveOut(waveDisc, 0);
    return 0;
}

VOID* IntroBallSound() {
    WaveOut(waveIntroBall, 0);
    return 0;
}

VOID* PlayerJumpSound() {
    WaveOut(waveJumpSound, 0);
    return 0;
}

VOID* PlayerFallSound() {
    WaveOut(waveFallingSound, 0);
    return 0;
}

VOID* PlayerSpeechSound() {
    WaveOut(waveSpeechSound, 0);
    return 0;
}

VOID* RedBallJumpSound() {
    WaveOut(waveRedBallJumpSound, 0);
    return 0;
}

VOID* SnakeEggJumpSound() {
    WaveOut(waveSnakeEggJumpSound, 0);
    return 0;
}

VOID* SnakeJumpSound() {
    WaveOut(waveSnakeJumpSound, 0);
    return 0;
}

VOID* SnakeFallSound() {
    WaveOut(waveplaySnakeFallSound, 0);
    return 0;
}

VOID* VictorySound() {
    WaveOut(waveIntroBall, 0); // Assuming Victory sound is the same as IntroBall
    return 0;
}

void PlayStartupSounds(int currentFrame) {
    if (currentFrame == 30 && !helloPlayed) {
        HelloSound();
        helloPlayed = true;
    }

    if (currentFrame == 90 && !liftPlayed) {
        DiscSound();
        liftPlayed = true;
    }

    if (currentFrame == 240 && !jumpSoundPlayed[0]) {
        PlayerJumpSound();
        jumpSoundPlayed[0] = true;
    }
    if (currentFrame == 290 && !jumpSoundPlayed[1]) {
        PlayerJumpSound();
        jumpSoundPlayed[1] = true;
    }
    if (currentFrame == 320 && !jumpSoundPlayed[2]) {
        PlayerJumpSound();
        jumpSoundPlayed[2] = true;
    }
    if (currentFrame == 350 && !jumpSoundPlayed[3]) {
        PlayerJumpSound();
        jumpSoundPlayed[3] = true;
    }

    if (currentFrame == 420 && !speechPlayed) {
        PlayerSpeechSound();
        speechPlayed = true;
    }

    if (currentFrame == 393 && !introBallPlayed) {
        IntroBallSound();
        introBallPlayed = true;
    }
}
