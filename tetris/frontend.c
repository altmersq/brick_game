#include "tetris_frontend.h"
#include "tetris.h"
#include <ncurses.h>

extern int (*get_current_piece())[4];
extern int get_current_x();
extern int get_current_y();

void draw_board(GameInfo_t gameInfo) {
    int start_x = 6;
    int start_y = 3;
    clear();
    
    for (int i = 0; i < FIELD_H; i++) {
        for (int j = 0; j < FIELD_W; j++) {
            if (gameInfo.field[i][j] == 1) {
                attron(COLOR_PAIR(1));
                mvprintw(start_y + i, start_x + j * 3, "[#]");
                attroff(COLOR_PAIR(1));
            } else {
                attron(COLOR_PAIR(2));
                mvprintw(start_y + i, start_x + j * 3, "[ ]");
                attroff(COLOR_PAIR(2));
            }
        }
    }

    int (*current_piece)[4] = get_current_piece();
    int current_x = get_current_x();
    int current_y = get_current_y();

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (current_piece[i][j] == 1) {
                int x = current_x + j;
                int y = current_y + i;
                if (x >= 0 && x < FIELD_W && y >= 0 && y < FIELD_H) {
                    attron(COLOR_PAIR(1));
                    mvprintw(start_y + y, start_x + x * 3, "[#]");
                    attroff(COLOR_PAIR(1));
                }
            }
        }
    }

    refresh();
}

void draw_hud(GameInfo_t gameInfo) {
    rectangle(0, 0, 25, 60);
    rectangle(2, 5, 23, 36);
    mvprintw(4, 42, "Score: %d", gameInfo.score);
    mvprintw(6, 42, "High Score: %d", gameInfo.high_score);
    mvprintw(8, 42, "Level: %d", gameInfo.level);
    mvprintw(10, 42, "Speed: %d", gameInfo.speed);
    mvprintw(12, 42, "Next Piece:");

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (gameInfo.next[i][j] == 1) {
                mvprintw(14 + i, 42 + j * 3, "[#]");
            } else {
                mvprintw(14 + i, 42 + j * 3, "[ ]");
            }
        }
    }

    mvprintw(22, 42, "Pause status: %d", gameInfo.pause);
    refresh();
}

void rectangle(int y1, int x1, int y2, int x2) {
    mvhline(y1, x1, 0, x2 - x1);
    mvhline(y2, x1, 0, x2 - x1);
    mvvline(y1, x1, 0, y2 - y1);
    mvvline(y1, x2, 0, y2 - y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

void display_controls() {
    clear();
    mvprintw(5, 10, "Controls:");
    mvprintw(7, 10, "Arrow left  - Move figure left");
    mvprintw(8, 10, "Arrow right - Move figure right");
    mvprintw(9, 10, "Arrow down  - Move figure down");
    mvprintw(10, 10, "r          - Rotate figure");
    mvprintw(11, 10, "p          - Pause");
    mvprintw(12, 10, "q          - Exit");
    mvprintw(14, 10, "Press any key to quit");
    refresh();
    getch();
}

void init_colors() {
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);  
        init_pair(2, COLOR_GREEN, COLOR_BLACK); 
    }
}