#include <raylib.h>
#include <stdio.h>
#include "sounds.h"

AudioManager Carregasom(void) {
    InitAudioDevice(); // O dispositivo de áudio ainda é iniciado aqui

    AudioManager audio = {0}; // Cria a struct e zera seus valores

    // Carrega cada som e o atribui ao campo correspondente na struct
    audio.somBomba = LoadSound("audio/bomba.wav");
    audio.somExplosion = LoadSound("audio/explosion.wav");
    audio.somStart = LoadSound("audio/start.wav");
    audio.somMorte = LoadSound("audio/morte.wav");
    audio.somNext_Map = LoadSound("audio/next_map.wav");
    audio.somChave = LoadSound("audio/chave.wav");
    audio.somDano = LoadSound("audio/dano.wav");
    audio.somBotao = LoadSound("audio/botao.wav");
    audio.musicaMenu = LoadMusicStream("audio/menu.mp3");
    audio.musicaPausa = LoadMusicStream("audio/pausa.mp3");
    audio.musicaJogo = LoadMusicStream("audio/musica.mp3");

    SetMusicVolume(audio.musicaMenu, 0.5);
    SetMusicVolume(audio.musicaJogo, 0.05);
    SetSoundVolume(audio.somExplosion, 0.05);
    SetSoundVolume(audio.somChave, 0.1);
    SetSoundVolume(audio.somStart, 0.4);
    SetSoundVolume(audio.somMorte, 0.4);
    SetSoundVolume(audio.somBotao, 0.3);
    // Verificação para ver se os audios foram corretamentes carregados
    if (audio.somBomba.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'bomba.wav'");
    if (audio.somExplosion.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'explosion.wav'");
    if (audio.somStart.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'start.wav'");
    if (audio.somMorte.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'morte.wav'");
    if (audio.somNext_Map.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'next_Map.wav'");
    if (audio.somChave.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'chave.wav'");
    if (audio.somDano.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'dano.wav'");
    if (audio.somBotao.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'botao.wav'");
    if (audio.musicaMenu.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'menu.wav'");
    if (audio.musicaPausa.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'pausa.wav'");
    if (audio.musicaJogo.stream.buffer == NULL) TraceLog(LOG_WARNING, "AUDIO: Falha ao carregar 'jogo.wav'");
    return audio; // Retorna a struct com todos os sons carregados
}

void EliminaAudio(AudioManager audio) {
    UnloadSound(audio.somBomba);
    UnloadSound(audio.somExplosion);
    UnloadSound(audio.somStart);
    UnloadSound(audio.somMorte);
    UnloadSound(audio.somNext_Map);
    UnloadSound(audio.somChave);
    UnloadSound(audio.somDano);
    UnloadSound(audio.somBotao);

    UnloadMusicStream(audio.musicaMenu);
    UnloadMusicStream(audio.musicaPausa);
    UnloadMusicStream(audio.musicaJogo);
    CloseAudioDevice(); // Fecha o dispositivo de áudio
}
