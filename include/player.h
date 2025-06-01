
//Struct que define as propriedades do jogador
typedef struct {

    Vector2 pos; //Vetor para a posição do jogador (x,y)
    Vector2 direction; //Vetor para a direção do jogador (x,y)

    float speed; //Velocidade do jogador

} tPlayer;


//Declaração das funções
void MovePlayer(tPlayer *player);
void DrawPlayer(tPlayer *player);