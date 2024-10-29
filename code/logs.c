#include <ncurses.h>
#include <string.h>
#include <unistd.h>

void LOGS(){
	initscr();
	raw();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	start_color();
	init_pair(3, COLOR_BLACK, COLOR_CYAN);
	int col, row;
	getmaxyx(stdscr, row, col);
	mvprintw(3, (col - 7) / 2, "HISTORY");
	mvchgat(3, (col - 7) / 2, 7, A_BOLD, 3, NULL);
	int n = 0;
	FILE* fp;
	fp = fopen("data.txt", "r");
	char str[100][300];
	if(fp != NULL)
		while(fgets(str[n++], 300, fp) != NULL);
	n--;
	if(fp != NULL)
		fclose(fp);
	for(int i = 0; i < n; i++)
		mvprintw(5 + i, (col - strlen(str[n - 1 - i])) / 2, str[n - 1 - i]);
	mvprintw(row - 1, 0, "Press any key to continue.");
	refresh();
	getch();

	endwin();
	clear();
}