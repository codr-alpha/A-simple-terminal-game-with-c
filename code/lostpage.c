#include <ncurses.h>
#include <unistd.h>

void lostpage(int score, char name[40]){
	initscr();
	int col, row;
	getmaxyx(stdscr, row, col);
	mvprintw(row / 2 - 3, (col - 8) / 2, "YOU LOST");
	mvprintw(row / 2 + 1, (col - 19) / 2, "YOUR SCORE : %d", score);
	mvchgat(row / 2 + 1, (col - 19) / 2, -1, A_BOLD, 0, NULL);
	mvprintw(row / 2 + 5, (col - 26) / 2, "Enter your name:");
	curs_set(TRUE);
	echo();
	refresh();
	getstr(name);
	noecho();
	curs_set(FALSE);
	endwin();
	clear();
}
