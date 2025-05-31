
typedef struct {

    Vector2 pos;
    Vector2 direction;
    float speed;

} tPlayer;

void MovePlayer(tPlayer *player);
void DrawPlayer(tPlayer *player);