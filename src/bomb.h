#include <raylib.h>
#include <math.h>
#include "player.h"
#include "map.h"

// salva as informações de cada uma das 3 bombas
typedef struct{
    int bombsLeft;
    bool isPlanted[3];
    double planted_times[3];
    Vector2 positions[3];
} tBomb;

// checa se isPlanted[i] é true, e se a diferença de tempo desde que foi plantada é menor do que 3 (planted_times[i]).
// enquanto ambos forem verdadeiros, o retângulo vermelho será desenhado na posição positions[i].
void PlantBomb(tBomb *bomb, tMap *mapa){
    for (int i=0; i<3; i++){
        if (bomb->isPlanted[i]==true && GetTime()-bomb->planted_times[i]<3){
            DrawRectangle(bomb->positions[i].x, bomb->positions[i].y, mapa->tile_size, mapa->tile_size, RED);
        }
    }
}

// Enquanto houver bombas disponíveis, quando a tecla B for pressionada,
// a próxima bomba disponível (3-bombsLeft) será "plantada". Para isso, 
// isPlanted[i] será true, o tempo em que a tecla foi pressionada será 
// gravada em planted_times[i] e a posição da bomba, em positions[i].
// Finalmente, uma bomba a menos estará disponível para ser usada. 
void BombsManager(tPlayer *player, tMap *map, tBomb *bomb){
    if (bomb->bombsLeft==0)return;
    if (IsKeyPressed(KEY_B)){
        int current = 3-bomb->bombsLeft;
        bomb->isPlanted[current] = true;
        Vector2 vector;
        vector.x = player->direction.x*map->tile_size + player->screenPos.x;
        vector.y = player->direction.y*map->tile_size + player->screenPos.y;
        bomb->positions[current] = vector;
        bomb->planted_times[current] = GetTime();
        bomb->bombsLeft--;
        PlantBomb(bomb, map);
    }
    
}