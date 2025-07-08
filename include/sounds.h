#ifndef SOUNDS_H_
#define SOUNDS_H_

#include <raylib.h>


typedef struct {
    Sound somBomba;
    Sound somExplosion;
    Sound somStart;
    Sound somMorte;
    Sound somNext_Map;
    Sound somChave;
    Sound somDano;
} AudioManager;
AudioManager Carregasom(void);
void Eliminasom(AudioManager som);

#endif