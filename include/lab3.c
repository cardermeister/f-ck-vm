#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>

enum colors{BLACK,RED,GREEN,YELLOW,BLUE,VIOLET,BIRUZ,WHITE};

int mt_clrscr(void){ // очистка экрана
	write(1,"\E[H\E[2J",7);
	return 0;
}

int mt_gotoXY(int x, int y){ // перемещение курсора на координаты (строчка, символ)
	char buf[255];
	int len = sprintf(buf,"\E[%d;%dH",x,y);
	write(1,buf,len);
	return 0;
}

int mt_getscreensize(int *rows, int *cols){ // возвращает размеры экрана (в строках и столбцах)
	struct winsize ws;
	if(!ioctl(1,TIOCGWINSZ,&ws)){
		*rows = ws.ws_row;
		*cols = ws.ws_col;
	} else {
		write(1,"Error",5);
		return -1;
	}
	return 0;
}

int mt_setfgcolor(enum colors color){ // установка цвета выводимых символов
	char buf[255];
	int len = sprintf(buf,"\E[3%dm",color);
	write(1,buf,len);
	return 0;
}

int mt_setbgcolor(enum colors color){ // установка фона консоли
	char buf[255];
	int len = sprintf(buf,"\E[4%dm",color);
	write(1,buf,len);
	return 0;
}
