#include "icb_gui.h"
#include "Sound.h"


//VOID* SoundThread2(LPVOID lpParam) {
//    const char* soundFile = (const char*)lpParam;
//    PlaySound(soundFile, NULL, SND_SYNC);
//
//    return 0;  // VOID* dönüþ tipiyle NULL döndür
//}
//
//void CreateSoundThread(const char* soundPath) {
//    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SoundThread2, (LPVOID)soundPath, 0, NULL);
//}

VOID* LiftSound() {
    /*ICBYTES A;
    ReadWave(A, "Sounds/Lift.wav");
    A *= 1;
    WaveOut(A, 0);*/
    
    PlaySound("Sounds/Lift.wav", NULL, SND_SYNC);

    return 0;  // VOID* dönüþ tipiyle NULL döndür
}

VOID* JumpSound() {
    /*ICBYTES C;
    ReadWave(C, "C://Users/deniz/Desktop/UniversityLessons4thClass/Parallel Programming/Projects/Q_bert/Q_bert/Sounds/Jump.wav");
    C *= 6;
    WaveOut(C, 1);*/

    PlaySound("Sounds/Jump.wav", NULL, SND_FILENAME);

    return 0;  // VOID* dönüþ tipiyle NULL döndür
}


VOID* Jump2Sound() {
    /*ICBYTES B;
    ReadWave(B, "Sounds/Jump2.wav");
    B *= 1;
    WaveOut(B, 1);*/

    PlaySound("Sounds/Jump2.wav", NULL, SND_SYNC);

    return 0;  // VOID* dönüþ tipiyle NULL döndür
}

