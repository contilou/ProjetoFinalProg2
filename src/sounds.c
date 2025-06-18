#include <raylib.h>
#include <stdio.h>
#include "sounds.h"

AudioManager Carregasom(void) {
    InitAudioDevice(); // O dispositivo de áudio ainda é iniciado aqui

    AudioManager audio = {0}; // Cria a struct e zera seus valores

    // Carrega cada som e o atribui ao campo correspondente na struct
    audio.somBomba = LoadSound("audio/bomba.wav");
    // Verificação para ver se os audios foram corretamentes carregados
    if (audio.somBomba.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'bomba.wav'");
    return audio; // Retorna a struct com todos os sons carregados
}

void Eliminasom(AudioManager audio) {
    UnloadSound(audio.somBomba);
    // UnloadSound(audio.musicaMenu);

    CloseAudioDevice(); // Fecha o dispositivo de áudio
}
