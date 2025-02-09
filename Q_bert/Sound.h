#pragma once
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include <string>


VOID* HelloSound();
VOID* LiftSound();
VOID* IntroBallSound();
VOID* JumpSound();
VOID* SpeechSound();
VOID* PlayerFallSound();
VOID* RedBallJumpSound();
VOID* SnakeEggJumpSound();
VOID* SnakeJumpSound();
VOID* SnakeFallSound();
void PlayStartupSounds(int currentFrame);

// Yeni ses çalma sistemi
class SoundPlayer {
private:
    static int soundCounter;
public:
    static void PlaySound(const char* filename) {
        std::string alias = "sound" + std::to_string(soundCounter++);
        std::string openCommand = "open \"" + std::string(filename) + "\" type waveaudio alias " + alias;
        std::string playCommand = "play " + alias;

        mciSendStringA(openCommand.c_str(), NULL, 0, NULL);
        mciSendStringA(playCommand.c_str(), NULL, 0, NULL);

    
        CreateThread(NULL, 0, [](LPVOID param) -> DWORD {
            std::string* aliasPtr = (std::string*)param;
            Sleep(2000); 
            std::string closeCommand = "close " + *aliasPtr;
            mciSendStringA(closeCommand.c_str(), NULL, 0, NULL);
            delete aliasPtr;
            return 0;
            }, new std::string(alias), 0, NULL);
    }
};