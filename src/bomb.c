#include <raylib.h>
#include <math.h>
#include "bomb.h"
#include "sounds.h"
// salva as informações de cada uma das 3 bombas


// checa se isPlanted[i] é true, e se a diferença de tempo desde que foi plantada é menor do que 3 (planted_times[i]).
// enquanto ambos forem verdadeiros, o retângulo vermelho será desenhado na posição positions[i].
void PlantBomb(tBomb *bomb, tMap *mapa){
    for (int i=0; i<3; i++){
        if (bomb->isPlanted[i]==true && GetTime()-bomb->planted_times[i]<3){
            DrawRectangle(bomb->positions[i].column * mapa->tile_size, bomb->positions[i].row * mapa->tile_size, mapa->tile_size, mapa->tile_size, RED);
        }
    }
}

// Enquanto houver bombas disponíveis, quando a tecla B for pressionada,
// a próxima bomba disponível (3-bombsLeft) será "plantada". Para isso, 
// isPlanted[i] será true, o tempo em que a tecla foi pressionada será 
// gravada em planted_times[i] e a posição da bomba, em positions[i].
// Finalmente, uma bomba a menos estará disponível para ser usada. 
void BombsManager(tPlayer *player, tMap *map, tBomb *bomb, AudioManager audio){
    PlantBomb(bomb, map);
    if (bomb->bombsLeft==0)return;
    if (IsKeyPressed(KEY_B)){
        PlaySound(audio.somBomba);
        int current = 3-bomb->bombsLeft;
        bomb->isPlanted[current] = true;
        tMapPos vector;
        vector.column = player->direction.x + player->matrixPos.column;
        vector.row = player->direction.y + player->matrixPos.row;
        bomb->positions[current] = vector;
        bomb->planted_times[current] = GetTime();
        bomb->bombsLeft--;
        PlantBomb(bomb, map);
    }
    
}