#ifndef MAP_H_
#define MAP_H_
#include <stdio.h>

typedef struct {

    char* file_name; //nome do arquivo de texto que contém a matriz do mapa
    char** matrix; //matriz do mapa
    int mapId; //ID do mapa
    int rows; //número de linhas totais do mapa (excluindo a barra da HUD)
    int columns; //número de colunas totais do mapa (excluindo a barra da HUD)
    int tile_size; //tamanho de cada quadradinho (correspondente a um elemento da matriz do mapa)
    Texture2D wall_sprite; //Sprites da parede normal
} tMap;


//Struct com as cordenadas referentes à matriz do mapa (baseadas em linha e coluna)
typedef struct {

    int row;
    int column;

} tMapPos;

int GetMapMatrix(tMap* map, FILE *map_text); //Abre o arquivo de texto com o mapa e o preenche na matriz do mapa
void DrawWalls(tMap* map); //Percorre a matriz do mapa e desenha na tela uma parede para cada elemento que tiver 'W' (Deve ser utilizada na área de desenho na main.c
int InitMaps(tMap **maps, int *num_maps);
#endif