// Sound.h

#ifndef SOUND_H
#define SOUND_H

#include <windows.h>
#include <icb_gui.h>

// Function declarations
void ResetSoundFlags();
void prepareWave();
VOID* HelloSound();
VOID* DiscSound();
VOID* IntroBallSound();
VOID* PlayerJumpSound();
VOID* PlayerFallSound();
VOID* PlayerSpeechSound();
VOID* RedBallJumpSound();
VOID* SnakeEggJumpSound();
VOID* SnakeJumpSound();
VOID* SnakeFallSound();
VOID* VictorySound();
void PlayStartupSounds(int currentFrame);

// Global variables
extern bool helloPlayed;
extern bool liftPlayed;
extern bool introBallPlayed;
extern bool speechPlayed;
extern bool jumpSoundPlayed[4];
extern ICBYTES waveHello, waveDisc, waveIntroBall, waveJumpSound, waveSpeechSound, waveFallingSound, waveDiscSound,
waveSwearingSound, waveRedBallJumpSound, waveSnakeEggJumpSound, waveSnakeJumpSound, waveplaySnakeFallSound;

#endif // SOUND_H
