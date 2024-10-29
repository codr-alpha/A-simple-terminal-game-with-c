#include <string.h>
#include <ncurses.h>

void exit_page(){
	initscr();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_GREEN);
	char str[] = "Have a nice day!!!";
	int col, row;
	getmaxyx(stdscr, col, row);
	mvprintw(col / 2, (row - strlen(str)) / 2, str);
	mvchgat(col / 2, (row - strlen(str)) / 2, strlen(str), A_BOLD, 1, NULL);
	refresh();
	getch();
	clear();
	endwin();
}