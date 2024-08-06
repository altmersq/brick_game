#include "tetris.h"
#include "tetris_frontend.h"
#include "tetris_backend.h"
#include "fsm.h"
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

extern int (*get_current_piece())[4];
extern int get_current_x();
extern int get_current_y();


void sleep_for_milliseconds(long milliseconds) {
    struct timespec req, rem;
    req.tv_sec = milliseconds / 1000;
    req.tv_nsec = (milliseconds % 1000) * 1000000L;
    nanosleep(&req, &rem);
}

int main_menu() {
    int choice = 0;
    int ch;

    while (true) {
        clear();
        rectangle(2, 5, 23, 36);
        mvprintw(5, 18, "Tetris");
        mvprintw(7, 16, (choice == 0) ? "> Play" : "  Play");
        mvprintw(9, 16, (choice == 1) ? "> Controls" : "  Controls");
        mvprintw(11, 16, (choice == 2) ? "> Exit" : "  Exit");
        refresh();

        ch = getch();

        switch (ch) {
            case KEY_UP:
                choice = (choice == 0) ? 2 : choice - 1;
                break;
            case KEY_DOWN:
                choice = (choice == 2) ? 0 : choice + 1;
                break;
            case 10: // ENTER key
                return choice;
        }
    }
}

int main() {
    initscr();            // Инициализация ncurses
    cbreak();             // Отключение буферизации ввода
    noecho();             // Отключение отображения ввода
    keypad(stdscr, TRUE); // Включение обработки специальных клавиш
    init_colors();

    while (true) {
        int choice = main_menu();

        switch (choice) {
            case 0: // Играть
                nodelay(stdscr, TRUE); 
                initGame();
                while (true) {

                    int ch = getch();
                    if (ch != ERR) {
                        switch (ch) {
                            case 10: // ENTER key
                                userInput(Start, false);
                                break;
                            case KEY_LEFT:
                                userInput(Left, false);
                                break;
                            case KEY_RIGHT:
                                userInput(Right, false);
                                break;
                            case KEY_DOWN:
                                userInput(Down, false);
                                break;
                            case 'r':
                                userInput(Action, false);
                                break;
                            case 'p':
                                userInput(Pause, false);
                                break;
                            case 'q':
                                endwin();
                                return 0;
                        }
                    }

                    GameInfo_t gameInfo = updateCurrentState();
                    GameState current_state = get_current_state();

                    if (gameInfo.pause == 0 && current_state != STATE_GAME_OVER) {
                        draw_board(gameInfo);
                        draw_hud(gameInfo);
                    } else if (current_state == STATE_GAME_OVER) {
                        save_high_score("high_score.txt", gameInfo.high_score);
                        nodelay(stdscr, FALSE);
                        mvprintw(10, 9, "Game Over! Your score: %d", gameInfo.score);
                        mvprintw(11, 12, "Press 'q' to quit ");
                        mvprintw(12, 21, "or");
                        mvprintw(13, 12, "'Enter' to restart");

                        refresh();

                        while (true) {
                            ch = getch();
                            if (ch == 'q') {
                                endwin();
                                return 0;
                            } else if (ch == 10) { // ENTER key
                                break;
                            }
                        }
                        break; // Возврат в главное меню после завершения игры
                    }

                    sleep_for_milliseconds(5);
                }
                break;
            case 1: // Управление
                display_controls();
                break;
            case 2: // Выход
                endwin();
                return 0;
        }
    }

    endwin();
    return 0;
}