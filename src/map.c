#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "player.h"

//Abre o arquivo de texto com o mapa e o preenche na matriz do mapa
int GetMapMatrix(tMap *map, FILE *map_text){

    //Inicializa a matriz do mapa-------------
    map->matrix = (char**) malloc(map->rows * sizeof(char*));

    if(map->matrix == NULL){
        return -1;
    }

    for (int i = 0; i < map->rows; i++){

        *(map->matrix + i) = (char*) malloc(map->columns * sizeof(char));
        if(*(map->matrix + i) == NULL){
            return -1;
        }
    }
    //----------------------------------------

    //Percorre a matriz do mapa e preenche cada elemento com o caractere correspondente do arquivo,------
    for(int i = 0; i < map->rows; i++){

        for(int j = 0; j < map->columns; j++){

            char c = fgetc(map_text);
            while(c == '\n'){ //se o caractere obtido for uma quebrada de linha, ele será substituido pelo caractere seguinte até que a variável c tenha um caracter válido para a matriz
                c = fgetc(map_text);
            }
            map->matrix[i][j] = c;
        }
    }
    //---------------------------------------------------------------------------------------------------

    fclose(map_text); //Fecha o documento

    return 1;
}

//Percorre a matriz do mapa e desenha na tela uma parede indestrutivel para cada elemento que tiver 'W'  (Deve ser utilizada na área de desenho na main.c
//E desenha na tela uma parede destruitivel para cada elemento que tiver 'D' 
void DrawWalls(tMap* map){
    for(int i = 0; i < map->rows; i++){
        for(int j = 0; j < map->columns; j++){
            if (map->matrix[i][j] == 'W') {
                DrawTexture(map->wall_sprite, j * map->tile_size, i * map->tile_size, WHITE);
            } 
        }
    }
}



int InitMaps(tMap **maps, int *num_maps){

    FILE *map_text;
    int count = 1;   
    
    *maps = (tMap *) malloc(2 * sizeof(tMap));
    
    if (*maps == NULL)
    {
        return -1;
    }
    
    while (true) 
    {
        
        char current_file[17];
        snprintf(current_file, 17, "%s%d%s", "maps/mapa", count, ".txt");
        map_text = fopen(current_file, "r");

        if(map_text == NULL){
            break;
        } 

        *maps = (tMap *) realloc(*maps, sizeof(tMap) * count );
        Texture2D wall_sprite = LoadTexture("sprites/parede.png");
        (*maps)[count - 1] = (tMap){current_file, NULL, count, 25, 60, 20, wall_sprite};

        if(GetMapMatrix(&(*maps)[count - 1], map_text) != 1){
            return -1;
        }
        
        fclose(map_text);
        count++;
    }
    *num_maps = count;

    return 0;
} 

