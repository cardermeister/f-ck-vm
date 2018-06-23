#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
struct termios term;
enum keys {undefined=0, l=1, s=2, r=3, t=4, i=5, f5=6, f6=7, down=8, up=9, k_left=10, k_right=11, enter=12, esc=13,p=140 };
int rk_readkey(enum keys *k){
	char c[2];
	*k=undefined;
	int j=0,f=2;
	while(1) {
		read(1,&c,2);
		switch(j) {
			case 0: switch(c[0]) {
				case 'l': *k=l; break;
				case 's': *k=s; break;
				case 'r': *k=r; break;
				case 't': *k=t; break;
				case 'i': *k=i; break;
                                case 'p': *k=p; break;
				case '\n': *k=enter; break;
				}
				if(c[1]=='[') {j++; continue;}
				break;
			case 1: switch(c[0]) {
					case 'A': *k=up; break;
					case 'B': *k=down; break;
					case 'C': *k=k_right; break;
					case 'D': *k=k_left; break;
					case '1': if(c[1]=='5') *k=f5; 
						  if(c[1]=='7') *k=f6; 
						  read(0,&c,1); break;
					}
					break;
			}
		break;
		}
		return 0;
}


int rk_mytermsave(void){
	if((tcgetattr(1, &term))==-1) //в term записывает структуру, описывающую режимы работы терминала
	{
		perror("Error: tcgetattr");
		return -1;
	}
	return 0;
}

int rk_mytermload(void){
	if((tcsetattr(1, TCSANOW, &term)) < 0) //устанавливает новые параметры, сохраненные в структуре term
	{
		perror("Error: tcsetattr");
		return -1;
	}
	return 0;
}

int rk_mytermregime(int regime,int vtime,int vmin,int echo,int sigint){
 	struct termios myterm;
 	
	tcgetattr(1,&myterm); //получает текущие настройки терминала
 	
	if(regime) myterm.c_lflag |= ICANON; //канонический режим
 	else myterm.c_lflag &= ~ICANON; 
 	
	if(echo) myterm.c_lflag |= ECHO; //будут сразу отображаться на экране терминала вводимые с клавиатуры символы
 	else myterm.c_lflag &= ~ECHO; 
 
	if(sigint) myterm.c_lflag |= ISIG; //разрешена обработка управляющих символов
 	else myterm.c_lflag &= ~ISIG;
 
	myterm.c_cc[VMIN] = vmin; //какое количество символов должно быть в очереди, чтобы вызов read завершился
 	myterm.c_cc[VTIME] = vtime; //сколько времени ждать появления хотя бы одного символа в очереди
 
	return tcsetattr(1,TCSANOW,&myterm); //устанавливает новые параметры
}
