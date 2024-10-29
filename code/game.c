#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "lostpage.h"

char fighter[5][5];
char rocket[5][10];
char bomber[3][3];
char backslash;
int xposition_of_rocket, yposition_of_rocket;
int health_of_rocket;
int score;
int row, col;
int tme;

bool rocket_location[100][300];
bool rocket_shot_location[100][300];

bool bomb_location[100][300];

bool safine_bari_location[100][300];
bool health_package_location[100][300];
int health_of_safine_bari[100][300];
int x_center_of_safine_bari[100][300];
int y_center_of_safine_bari[100][300];

bool bomber_location[100][300];
bool bomber_type[100][300];
int health_of_bomber[100][300];
int x_center_of_bomber[100][300];
int y_center_of_bomber[100][300];

bool fighter_shot_location[100][300];
bool fighter_location[100][300];
int health_of_fighter[100][300];
int x_center_of_fighter[100][300];
int y_center_of_fighter[100][300];

int x_center_of_droid[100][300];
int y_center_of_droid[100][300];
bool droid_location[100][300];

const int fps = 120;

void init();
void erase_rocket();
void set_rocket();
void erase_safine_bari(int x, int y);
void set_safine_bari(int x, int y, int health);
void erase_bomber(int x, int y);
void set_bomber(int x, int y, int health, int tpe);
void erase_fighter(int x, int y);
void set_fighter(int x, int y, int health);
void erase_droid(int x, int y);
void set_droid(int x, int y);
void process_frame();
void frame();
void save_data(int score, char name[20]);

void PLAY(){
	srand(time(0));
	init();
	initscr();
	raw();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	getmaxyx(stdscr, row, col);
	xposition_of_rocket = col / 2;
	yposition_of_rocket = row - 6;
	set_rocket();
	frame();
	while(health_of_rocket > 0){
		usleep(1000000 / fps);
		int ch = getch();
		if((ch == KEY_RIGHT || ch == 'd' || ch == 'D') && (xposition_of_rocket + 1 + 4) < col){
			erase_rocket();
			xposition_of_rocket++;
			set_rocket();
		}
		if((ch == KEY_LEFT || ch == 'a' || ch == 'A') && (xposition_of_rocket - 1 - 4) >= 0){
			erase_rocket();
			xposition_of_rocket--;
			set_rocket();
		}
		if((ch == KEY_UP || ch == 'w' || ch == 'W') && (yposition_of_rocket - 1 - 2) >= 0){
			erase_rocket();
			yposition_of_rocket--;
			set_rocket();
		}
		if((ch == KEY_DOWN || ch == 's' || ch == 'S') && (yposition_of_rocket + 1) <= row - 6){
			erase_rocket();
			yposition_of_rocket++;
			set_rocket();
		}
		if(ch == 32 && rocket_shot_location[yposition_of_rocket - 4][xposition_of_rocket] == 0 && rocket_shot_location[yposition_of_rocket - 5][xposition_of_rocket] == 0) // 32 = enter
			rocket_shot_location[yposition_of_rocket - 3][xposition_of_rocket] = 1;
		process_frame();
		if(tme % 20 == 0){
			for(int i = row - 1; i > 0; i--)
				for(int j = 0; j < col; j++)
					bomb_location[i][j] = bomb_location[i - 1][j];
			for(int j = 0; j < col; j++)
				bomb_location[0][j] = 0;
		}
		if(tme % 12 == 0){
			for(int i = row - 1; i > 0; i--)
				for(int j = 0; j < col; j++) 
					if(safine_bari_location[i][j]){
						int x = x_center_of_safine_bari[i][j];
						int y = y_center_of_safine_bari[i][j];
						if(x != i || y != j)
							continue;
						if(x == row - 2){
							erase_safine_bari(x, y);
						}else{
							erase_safine_bari(x, y);
							set_safine_bari(x + 1, y, health_of_safine_bari[x][y]);
						}
					}
		}
		if(tme % 20 == 0){
			for(int i = row - 1; i > 0; i--)
				for(int j = 0; j < col; j++) 
					if(bomber_location[i][j]){
						int x = x_center_of_bomber[i][j];
						int y = y_center_of_bomber[i][j];
						if(x != i || y != j)
							continue;
						if(x == row - 2){
							erase_bomber(x, y);
						}else{
							erase_bomber(x, y);
							if(bomber_type[x][y])
								set_bomber(x + 1, y + 2, health_of_bomber[x][y], bomber_type[x][y]);
							else
								set_bomber(x + 1, y - 2, health_of_bomber[x][y], bomber_type[x][y]);
						}
					}
		}
		if(tme % 12 == 0){
			for(int i = row - 1; i > 0; i--)
				for(int j = 0; j < col; j++) 
					if(fighter_location[i][j]){
						int x = x_center_of_fighter[i][j];
						int y = y_center_of_fighter[i][j];
						if(x != i || y != j)
							continue;
						if(x == row - 2){
							erase_fighter(x, y);
						}else{
							erase_fighter(x, y);
							set_fighter(x + 1, y, health_of_fighter[x][y]);
						}
					}
		}
		if(tme % 12 == 0){
			int n = 0;
			int XX[100] = {};
			int YY[100] = {};
			for(int i = row - 1; i > 0; i--)
				for(int j = 0; j < col; j++) 
					if(droid_location[i][j]){
						int x = x_center_of_droid[i][j];
						int y = y_center_of_droid[i][j];
						if(x != i || y != j)
							continue;
						erase_droid(x, y);
						int dx = 1;
						int dy = 1;
						if(x > yposition_of_rocket)
							dx = -1;
						if(y > xposition_of_rocket)
							dy = -1;
						if(n < 100){
							XX[n] = x + dx;
							YY[n] = y + dy;
							n++;
						}
					}
			for(int i = 0; i < n; i++)
				set_droid(XX[i] , YY[i]);
		}
		if(tme % 7 == 0){
			for(int i = row - 1; i >= 0; i--)
				for(int j = 0; j < col; j++){
					if(i == 0)
						fighter_shot_location[i][j] = 0;
					else 
						fighter_shot_location[i][j] = fighter_shot_location[i - 1][j];
				}
			for(int i = row - 1; i >= 0; i--)
				for(int j = 0; j < col; j++)
					if(fighter_location[i][j]){
						int x = x_center_of_fighter[i][j];
						int y = y_center_of_fighter[i][j];
						if(x != i || y != j)
							continue;
						if(rand() % 10 == 0)
							fighter_shot_location[x + 2][y] = 1;
					}
		}
		if(tme % 250 == 0 && rand() % 2 == 0)
			bomb_location[0][rand() % col] = 1;
		if(tme % 2800 == 0)
			set_safine_bari(1, (rand() % (col - 2)) + 1, 100);
		if(tme % 800 == 0){
			if(rand() % 2 == 0)
				set_bomber(1, 1, 100, 1);
			else
				set_bomber(1, col - 2, 100, 0);
		}
		if(tme % 900 == 0)
			set_fighter(2, (rand() % (col - 4)) + 2, 100);
		if(tme % 1000 == 0)
			set_droid(1, rand() % (col - 1));
		process_frame();
		if(tme % 7 == 0){
			for(int i = 0; i < row; i++)
				for(int j = 0; j < col; j++)
					rocket_shot_location[i][j] = rocket_shot_location[i + 1][j];
		}
		process_frame();
		score += 1;
		tme += 1;
		frame();
	}
	endwin();
	clear();
	nodelay(stdscr, FALSE);
	char name[40];
	lostpage(score, name);
	save_data(score, name);
}

void frame(){
	clear();
    for(int j = 0; j < col; j++)
		mvprintw(row - 3, j, "-");

	mvprintw(row - 2, 3, "Score : %d", score);

	mvprintw(row - 2, col - 15, "Health : %d", health_of_rocket);

	// rocket
	for(int j = -2; j <= 2; j++)
		for(int i = -4; i <= 4; i++)
			if(rocket[2 + j][4 + i] != ' ')
				mvprintw(yposition_of_rocket + j, xposition_of_rocket + i, "%c", rocket[2 + j][4 + i]);

	// rocket shots
	for(int i = 0; i < row - 3; i++)
		for(int j = 0; j < col; j++)
			if(rocket_shot_location[i][j])
				mvprintw(i, j, "^");

	//safine bari
	for(int i = 0; i < row - 3; i++)
		for(int j = 0; j < col; j++)
			if(safine_bari_location[i][j])
				mvprintw(i, j, "o");

	// bomber
	for(int i = 0; i < row - 3; i++)
		for(int j = 0; j < col; j++)
			if(bomber_location[i][j]){
				int x = x_center_of_bomber[i][j];
				int y = y_center_of_bomber[i][j];
				mvprintw(i, j, "%c", bomber[1 + (i - x)][1 + (j - y)]);
			}

	//health package
	for(int i = 0; i < row - 3; i++)
		for(int j = 0; j < col; j++)
			if(health_package_location[i][j])
				mvprintw(i, j, "@");

	// bombs
	for(int i = 0; i < row - 3; i++)
		for(int j = 0; j < col; j++)
			if(bomb_location[i][j])
				mvprintw(i, j, "#");

	// fighter
	for(int i = 0; i < row - 3; i++)
		for(int j = 0; j < col; j++)
			if(fighter_location[i][j]){
				int x = x_center_of_fighter[i][j];
				int y = y_center_of_fighter[i][j];
				mvprintw(i, j, "%c", fighter[2 + (i - x)][2 + (j - y)]);
			}

	// fighter shot
	for(int i = 0; i < row - 3; i++)
		for(int j = 0; j < col; j++)
			if(fighter_shot_location[i][j])
				mvprintw(i, j, "*");

	// droid
	for(int i = 0; i < row - 3; i++)
		for(int j = 0; j < col; j++)
			if(droid_location[i][j]){
				if(x_center_of_droid[i][j] == i)
					mvprintw(i, j, "_");
				else
					mvprintw(i, j, "^");
			}
    refresh();
}

void erase_rocket(){
	for(int i = -2; i <= +2; i++)
		for(int j = -4; j <= +4; j++)
			rocket_location[yposition_of_rocket + i][xposition_of_rocket + j] = 0;
}

void set_rocket(){
	for(int i = -2; i <= +2; i++)
		for(int j = -4; j <= +4; j++)
			if(rocket[2 + i][4 + j] != ' ')
				rocket_location[yposition_of_rocket + i][xposition_of_rocket + j] = 1;
}

void erase_safine_bari(int x, int y){
	for(int i = -1; i <= 1; i++)
		for(int j = -1; j <= 1; j++)
			safine_bari_location[i + x][j + y] = 0;
}

void set_safine_bari(int x, int y, int health){
	for(int i = -1; i <= 1; i++)
		for(int j = -1; j <= 1; j++){
			x_center_of_safine_bari[i + x][j + y] = x;
			y_center_of_safine_bari[i + x][j + y] = y;
			safine_bari_location[i + x][j + y] = 1;
		}
	health_of_safine_bari[x][y] = health;
}

void erase_bomber(int x, int y){
	for(int i = -1; i <= 1; i++)
		for(int j = -1; j <= 1; j++)
			bomber_location[i + x][j + y] = 0;
}

void set_bomber(int x, int y, int health, int tpe){
	for(int i = -1; i <= 1; i++)
		for(int j = -1; j <= 1; j++){
			x_center_of_bomber[i + x][j + y] = x;
			y_center_of_bomber[i + x][j + y] = y;
			bomber_location[i + x][j + y] = 1;
		}
	health_of_bomber[x][y] = health;
	bomber_type[x][y] = tpe;
	if(rand() % 2 == 0)
		bomb_location[x + 2][y] = 1;
}

void erase_fighter(int x, int y){
	for(int i = -2; i <= 2; i++)
		for(int j = -2; j <= 2; j++)
			fighter_location[i + x][j + y] = 0;
}

void set_fighter(int x, int y, int health){
	for(int i = -2; i <= 2; i++)
		for(int j = -2; j <= 2; j++){
			x_center_of_fighter[i + x][j + y] = x;
			y_center_of_fighter[i + x][j + y] = y;
			fighter_location[i + x][j + y] = 1;
		}
	health_of_fighter[x][y] = health;
}

void erase_droid(int x, int y){
	for(int I = 0; I <= 1; I++)
		for(int J = 0; J <= 1; J++)
			droid_location[I + x][J + y] = 0;
}

void set_droid(int x, int y){
	int rm = 0;
	for(int i = 0; i <= 1; i++)
		for(int j = 0; j <= 1; j++)
			if(droid_location[i + x][j + y]){
				rm = 1;
				int xx = x_center_of_droid[i + x][j + y];
				int yy = y_center_of_droid[i + x][j + y];
				erase_droid(xx, yy);
			}
	if(rm)
		return;
	for(int i = 0; i <= 1; i++)
		for(int j = 0; j <= 1; j++){
			x_center_of_droid[i + x][j + y] = x;
			y_center_of_droid[i + x][j + y] = y;
			droid_location[i + x][j + y] = 1;
		}
}

void save_data(int score, char name[20]){
	FILE* fp;
	fp = fopen("data.txt", "a");
	time_t t;
   	time(&t);
   	char* str = ctime(&t);
   	for(int i = 0; i < (strlen(str) - 1); i++)
   		fprintf(fp, "%c", str[i]);
	fprintf(fp, "    %s SCORE : %d  ", name, score);
	fprintf(fp, "   %s PLAYED FOR %d seconds\n", name, tme / fps);
	fclose(fp);
}

void process_frame(){
	for(int i = 0; i < row; i++)
		for(int j = 0; j < col; j++){
			double health_of_rocket_impact = 1;//health_of_rocket / 100.0;
			if(rocket_shot_location[i][j] && bomb_location[i][j]){
				rocket_shot_location[i][j] = 0;
				bomb_location[i][j] = 0;
				score += 3 * fps;
			}
			if(rocket_location[i][j] && bomb_location[i][j]){
				bomb_location[i][j] = 0;
				health_of_rocket -= 30;
				if(health_of_rocket > 0)
					score += 3 * fps;
			}

			if(rocket_location[i][j] && health_package_location[i][j]){
				health_package_location[i][j] = 0;
				health_of_rocket += 30;
				if(health_of_rocket > 100)
					health_of_rocket = 100;
			}

			if(rocket_location[i][j] && safine_bari_location[i][j]){
				health_of_rocket -= 20;
				int x = x_center_of_safine_bari[i][j];
				int y = y_center_of_safine_bari[i][j];
				erase_safine_bari(x, y);
				health_package_location[x][y] = 1;
			}
			if(rocket_shot_location[i][j] && safine_bari_location[i][j]){
				rocket_shot_location[i][j] = 0;
				int x = x_center_of_safine_bari[i][j];
				int y = y_center_of_safine_bari[i][j];
				health_of_safine_bari[x][y] -= 20 * health_of_rocket_impact;
				if(health_of_safine_bari[x][y] <= 0){
					erase_safine_bari(x, y);
					health_package_location[x][y] = 1;
				}
			}

			if(rocket_shot_location[i][j] && bomber_location[i][j]){
				rocket_shot_location[i][j] = 0;
				int x = x_center_of_bomber[i][j];
				int y = y_center_of_bomber[i][j];
				health_of_bomber[x][y] -= 34 * health_of_rocket_impact;
				if(health_of_bomber[x][y] <= 0){
					erase_bomber(x, y);
					score += fps * 15;
				}
			}
			if(rocket_location[i][j] && bomber_location[i][j]){
				int x = x_center_of_bomber[i][j];
				int y = y_center_of_bomber[i][j];
				erase_bomber(x, y);
				health_of_rocket -= 20;
				if(health_of_rocket > 0)
					score += fps * 15;
			}

			if(rocket_shot_location[i][j] && fighter_shot_location[i][j]){
				rocket_shot_location[i][j] = 0;
				fighter_shot_location[i][j] =0;
			}
			if(rocket_shot_location[i][j] && fighter_location[i][j]){
				rocket_shot_location[i][j] = 0;
				int x = x_center_of_fighter[i][j];
				int y = y_center_of_fighter[i][j];
				health_of_fighter[x][y] -= 25 * health_of_rocket_impact;
				if(health_of_fighter[x][y] <= 0){
					score += fps * 30;
					erase_fighter(x,y);
				}
			}
			if(fighter_shot_location[i][j] && rocket_location[i][j]){
				fighter_shot_location[i][j] = 0;
				health_of_rocket -= 10;
			}
			if(fighter_location[i][j] && rocket_location[i][j]){
				int x = x_center_of_fighter[i][j];
				int y = y_center_of_fighter[i][j];
				erase_fighter(x, y);
				health_of_rocket -= 35;
				if(health_of_rocket > 0)
					score += fps * 30;
			}

			if(droid_location[i][j] && rocket_shot_location[i][j]){
				int x = x_center_of_droid[i][j];
				int y = y_center_of_droid[i][j];
				erase_droid(x, y);
				rocket_shot_location[i][j] = 0;
				score += 20 * fps;
			}
			if(droid_location[i][j] && rocket_location[i][j]){
				int x = x_center_of_droid[i][j];
				int y = y_center_of_droid[i][j];
				erase_droid(x, y);
				health_of_rocket -= 35;
			}
			if(droid_location[i][j] && bomb_location[i][j]){
				int x = x_center_of_droid[i][j];
				int y = y_center_of_droid[i][j];
				erase_droid(x, y);
				bomb_location[i][j] = 0;
			}
			if(health_package_location[i][j] && droid_location[i][j]){
				int x = x_center_of_droid[i][j];
				int y = y_center_of_droid[i][j];
				erase_droid(x, y);
				droid_location[i][j] = 0;
			}
			if(safine_bari_location[i][j] && droid_location[i][j]){
				int x = x_center_of_droid[i][j];
				int y = y_center_of_droid[i][j];
				erase_droid(x, y);
				int xx = x_center_of_safine_bari[i][j];
				int yy = y_center_of_safine_bari[i][j];
				erase_safine_bari(xx, yy);
				health_package_location[x][y] = 1;
			}
			if(bomber_location[i][j] && droid_location[i][j]){
				int x = x_center_of_droid[i][j];
				int y = y_center_of_droid[i][j];
				erase_droid(x, y);
				int xx = x_center_of_bomber[i][j];
				int yy = y_center_of_bomber[i][j];
				erase_bomber(xx, yy);
			}
			if(fighter_shot_location[i][j] && droid_location[i][j]){
				int x = x_center_of_droid[i][j];
				int y = y_center_of_droid[i][j];
				erase_droid(x, y);
				fighter_shot_location[i][j] = 0;
			}
			if(fighter_location[i][j] && droid_location[i][j]){
				int x = x_center_of_droid[i][j];
				int y = y_center_of_droid[i][j];
				erase_droid(x, y);
				int xx = x_center_of_fighter[i][j];
				int yy = y_center_of_fighter[i][j];
				erase_fighter(xx, yy);
			}
		}
}

void init(){
	for(int i = 0; i < 100; i++)
		for(int j = 0; j < 300; j++){
			rocket_shot_location[i][j] =0;
			rocket_location[i][j] = 0;
			bomb_location[i][j] = 0;
			safine_bari_location[i][j] = 0;
			health_package_location[i][j] = 0;
			bomber_location[i][j] = 0;
			fighter_shot_location[i][j] = 0;
			fighter_location[i][j] = 0;
			droid_location[i][j] = 0;
		}
	tme = 0;
	score = 0;
	health_of_rocket = 100;
	backslash = 92;
	for(int i = 0; i < 5; i++)
		for(int j = 0; j < 10; j++)
			rocket[i][j] = ' ';
	rocket[0][3] = '/';
	rocket[0][4] = '-';
	rocket[0][5] = backslash;
	rocket[1][2] = '/';
	rocket[1][3] = '|';
	rocket[1][4] = '@';
	rocket[1][5] = '|';
	rocket[1][6] = backslash;
	rocket[2][1] = '/';
	rocket[2][2] = '/';
	rocket[2][3] = '|';
	rocket[2][4] = '@';
	rocket[2][5] = '|';
	rocket[2][6] = backslash;
	rocket[2][7] = backslash;
	rocket[3][0] = '/';
	rocket[3][1] = '/';
	rocket[3][2] = '/';
	rocket[3][3] = backslash;
	rocket[3][4] = '-';
	rocket[3][5] = '/';
	rocket[3][6] = backslash;
	rocket[3][7] = backslash;
	rocket[3][8] = backslash;
	rocket[4][0] = backslash;
	rocket[4][1] = '_';
	rocket[4][2] = '/';
	rocket[4][3] = '-';
	rocket[4][4] = '-';
	rocket[4][5] = '-';
	rocket[4][6] = backslash;
	rocket[4][7] = '_';
	rocket[4][8] = '/';
	bomber[0][0] = '/';
	bomber[0][1] = '^';
	bomber[0][2] = backslash;
	bomber[1][0] = '|';
	bomber[1][1] = 'o';
	bomber[1][2] = '|';
	bomber[2][0] = backslash;
	bomber[2][1] = '_';
	bomber[2][2] = '/';
	fighter[0][0] = '/';
	fighter[0][1] = '-';
	fighter[0][2] = '-';
	fighter[0][3] = '-';
	fighter[0][4] = backslash;
	fighter[1][0] = '|';
	fighter[1][1] = '/';
	fighter[1][2] = '|';
	fighter[1][3] = backslash;
	fighter[1][4] = '|';
	fighter[2][0] = '|';
	fighter[2][1] = '|';
	fighter[2][2] = '|';
	fighter[2][3] = '|';
	fighter[2][4] = '|';
	fighter[3][0] = '|';
	fighter[3][1] = backslash;
	fighter[3][2] = '|';
	fighter[3][3] = '/';
	fighter[3][4] = '|';
	fighter[4][0] = backslash;
	fighter[4][1] = '_';
	fighter[4][2] = '-';
	fighter[4][3] = '_';
	fighter[4][4] = '/';
}