#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include "tetris.h"


int **create_matrix(int H, int W);
//void spawn_new_piece(int piece[4][4], int *x, int *y);
void spawn_new_piece();
void generate_piece(int piece[4][4]);
void initGame();
void userInput(UserAction_t action, bool hold);
int can_move(int piece[4][4], int x, int y, int dx, int dy);
void rotate_piece(int piece[4][4], int x, int y);
void add_piece_to_field(GameInfo_t *gameInfo, int piece[4][4], int x, int y);
bool check_and_remove_full_lines(GameInfo_t *gameInfo);
void update_score(GameInfo_t *gameInfo);
int move_piece_down(int piece[4][4], int *x, int *y);

void save_high_score(const char *filename, int high_score);
int load_high_score(const char *filename);

void update_score(GameInfo_t *gameInfo);

GameInfo_t updateCurrentState();

int (*get_current_piece())[4];
int get_current_x();
int get_current_y();



#endif //TETRIS_BACKEND_H