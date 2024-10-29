#include "menu.h"
#include "game.h"
#include "logs.h"
#include "exit_page.h"

int main(){
	int res;
	while((res = menu()) != 3){
		if(res == 1)
			PLAY();
		else if(res == 2)
			LOGS();
	}
	exit_page();
	return 0;
}