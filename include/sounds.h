#ifndef SOUNDS_H_
#define SOUNDS_H_

#include <raylib.h>


typedef struct {
    Sound somBomba;
} AudioManager;
AudioManager Carregasom(void);
void Eliminasom(AudioManager som);

#endif