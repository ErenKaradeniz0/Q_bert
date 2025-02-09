#pragma once
#include <windows.h>

// Ses fonksiyonlarý için prototip tanýmlamalarý
VOID* HelloSound();
VOID* LiftSound();
VOID* JumpSound();
VOID* SpeechSound();
VOID* PlayerFallSound();

// Animasyon için ses kontrol fonksiyonu
void PlayStartupSounds(int currentFrame);