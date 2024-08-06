#ifndef TETRIS_FRONTEND_H
#define TETRIS_FRONTEND_H
#include "tetris.h"

void draw_board(GameInfo_t gameInfo);
void draw_hud(GameInfo_t GameInfo);
void rectangle(int y1, int x1, int y2, int x2);
void display_controls();
void init_colors();

#endif //TETRIS_FRONTEND_H