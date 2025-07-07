#include <raylib.h>
#include <math.h>
#include "bomb.h"
#include "sounds.h"
// salva as informações de cada uma das 3 bombas


void explosionEffect(tBomb *bomb, tMap *mapa, int index){
    for(int i=0;i<9;i++){
        int xPos = bomb->area[index][i].column;
        int yPos = bomb->area[index][i].row;
        DrawRectangle(xPos * mapa->tile_size, yPos * mapa->tile_size, mapa->tile_size, mapa->tile_size, LIGHTGRAY);
    }
}

// checa se isPlanted[i] é true, e se a diferença de tempo desde que foi plantada é menor do que 3 (planted_times[i]).
// enquanto ambos forem verdadeiros, o retângulo vermelho será desenhado na posição positions[i].
void PlantBomb(tBomb *bomb, tMap *mapa){
    for (int i=0; i<3; i++){
        if (bomb->isPlanted[i]==true){
            if(GetTime()-bomb->planted_times[i]<3){
                DrawRectangle(bomb->positions[i].column * mapa->tile_size, bomb->positions[i].row * mapa->tile_size, mapa->tile_size, mapa->tile_size, DARKGRAY);
        }
            else{
                bomb->exploded[i]=true;
                bomb->bombsLeft++;
                bomb->isPlanted[i]=false;
                if (GetTime() - bomb->explosion_times[i]<1.5){
                    explosionEffect(bomb, mapa, i);
                }
            }
       }
        else if (bomb->exploded[i]==true){
            mapa->matrix[bomb->positions[i].row][bomb->positions[i].column] = ' ';
            if (GetTime() - bomb->explosion_times[i]<1.5){
                explosionEffect(bomb, mapa, i);
            }
            else{
                bomb->exploded[i]=false;
            }
       }
    }
}

// Enquanto houver bombas disponíveis, quando a tecla B for pressionada,
// a próxima bomba disponível (3-bombsLeft) será "plantada". Para isso, 
// isPlanted[i] será true, o tempo em que a tecla foi pressionada será 
// gravada em planted_times[i] e a posição da bomba, em positions[i].
// Finalmente, uma bomba a menos estará disponível para ser usada.
int x[]={0,0,0,0,1,2,-1,-2};
int y[]={1,2,-1,-2,0,0,0,0}; 
int total_planted = 0;

// Finalmente, uma bomba a menos estará disponível para ser usada. 
void BombsManager(tPlayer *player, tMap *map, tBomb *bomb, AudioManager audio){
    PlantBomb(bomb, map);
    if (bomb->bombsLeft==0)return;
    if (IsKeyPressed(KEY_B)){
        tMapPos vector;
        vector.column = player->direction.x + player->matrixPos.column;
        vector.row = player->direction.y + player->matrixPos.row;
        if (map->matrix[vector.row][vector.column]!=' '){
            return;
        }
        
        PlaySound(audio.somBomba);
        int current = total_planted%3;
        bomb->exploded[current] = false;
        bomb->isPlanted[current] = true;
        bomb->positions[current] = vector;
        bomb->planted_times[current] = GetTime();
        bomb->explosion_times[current] = GetTime()+3;
        map->matrix[bomb->positions[current].row][bomb->positions[current].column] = 'b';
        //armazena os dados em area[current]
        bomb->area[current][0] = bomb->positions[current];
        for (int i=1; i<9; i++){

            // checa se a posição anterior não é uma parede
            if((i % 2 == 0)){

                switch(map->matrix[bomb->area[current][i-1].row][bomb->area[current][i-1].column]){

                    case 'W':
                    case 'D':
                        bomb->area[current][i] = (tMapPos){-1,-1};
                        continue;
                        break;
                        
                }

            }

            bomb->area[current][i].column = bomb->positions[current].column + x[i-1];
            bomb->area[current][i].row = bomb->positions[current].row + y[i-1];
        }
        bomb->bombsLeft--;
        total_planted++;
        //PlantBomb(bomb, map);
    }
    
}


void resetBombInfo(tBomb *bomb){
    bomb->bombsLeft = 3;
    for (int i=0; i<3; i++){
        bomb->isPlanted[i]=false;
        bomb->exploded[i]=false;
    }
}