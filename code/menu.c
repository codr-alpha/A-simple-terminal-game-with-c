#include <ncurses.h>
#include <string.h>

int menu(){
	int res = 1;
	initscr();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);
	raw();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	int col, row;
	getmaxyx(stdscr, row, col);
	int line[4];
	line[1] = row / 2 - 3;
	line[2] = row / 2;
	line[3] = row / 2 + 3;
	int col_ = col / 2 - 2;
	mvprintw(line[1], col_, "PLAY");
	mvprintw(line[2], col_, "LOGS");
	mvprintw(line[3], col_, "EXIT");
	mvchgat(line[1], col_, 4, A_BLINK | A_BOLD, 2, NULL);
	mvchgat(line[2], col_, 4, A_BOLD, 1, NULL);
	mvchgat(line[3], col_, 4, A_BOLD, 1, NULL);
	refresh();
	int ch;
	while((ch = getch()) != 10){ // while wee didn't press enter
		if(ch == KEY_UP && res != 1){
			mvchgat(line[res], col_, 4, A_BOLD, 1, NULL);
			res--;
			mvchgat(line[res], col_, 4, A_BOLD | A_BLINK, 2, NULL);
		}else if(ch == KEY_DOWN && res != 3){
			mvchgat(line[res], col_, 4, A_BOLD, 1, NULL);
			res++;
			mvchgat(line[res], col_, 4, A_BOLD | A_BLINK, 2, NULL);
		}
	}
	refresh();
	endwin();
	clear();
	return res;
}