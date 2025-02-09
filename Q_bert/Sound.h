#pragma once
#include <windows.h>

// Ses fonksiyonlar� i�in prototip tan�mlamalar�
VOID* HelloSound();
VOID* LiftSound();
VOID* JumpSound();
VOID* SpeechSound();
VOID* PlayerFallSound();

// Animasyon i�in ses kontrol fonksiyonu
void PlayStartupSounds(int currentFrame);