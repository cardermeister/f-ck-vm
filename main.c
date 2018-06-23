#include <sys/time.h>
#include <signal.h>
#include <unistd.h>

#include "fatherhead.h"

int iC = 1;
int status = 1,t2 = 0; // для считывания
int statusAc = 1, t1 = 0; // статус фиксации аккумулятора, врем.хранение
int statusIc = 1; // статус фиксации аккумулятора, врем.хранение
char temp[4], temp1[4]; // для считывания значения

int verifMemory(int t)
{
	int op, com;
	sc_commandDecode(t,&com,&op);
	int mass[40] = {0x10,0x11,0x20,0x21,0x30,0x31,0x32,0x33,0x40,0x41,0x42,0x43,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x60,0x61,0x62, 0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x70,0x71,0x72,0x73,0x74,0x75,0x76};
	int status = -1;
	int i;
	for(i = 0; i < 40; i++){
		if(2*mass[i] == com){
			status = 0;
			break;
		}
	}
	return status;
}

void draw()
{
	// очищаем экран
	mt_clrscr();
	bc_box(0,0,13,72); // для памяти
	bc_box(0,73,4,75); // для accumulator
	bc_box(4,73,3,75); // для instruction
	bc_box(7,73,3,75); // для operation
	bc_box(10,73,3,75); // для flags
	bc_box(13,52,10,75); // для keys
	bc_box(13,0,10,51); // для 
	// 
	mt_gotoXY(1,4*7+5);
	write(1,"Memory",6);
	int i,j = 0,value,k=2;
	// memory
	for(i = 1; i <= 100; i++,j++)
    {
		sc_memoryGet(i,&value);
		if(i == iC && statusIc == 1){
			mt_setbgcolor(RED);
		}
	    	mt_gotoXY(k,j*7+2);
		char buf1[8];
		int l1 = sprintf(buf1,"+%.4x",value);
		write(1,buf1,5);
		if(i%10==0){
			k++;
			j = -1;
		}
		if(i == iC && statusIc == 1){
			mt_setbgcolor(BLACK);
		}
	}
	// accumulator
	mt_gotoXY(1,11*7-3);
	write(1,"accumulator",11);
	mt_gotoXY(2,11*7);
	if(statusAc == 1)
    {
		sc_memoryGet(iC,&value);
		char buf3[8];
		int l3 = sprintf(buf3,"+%.4x",value);
		write(1,buf3,l3);
	}
    else if(statusAc == 0)
    {
		char buf3[8];
		int l3 = sprintf(buf3,"+%.4x",t1);
		write(1,buf3,l3);
	}
	// instuctionCounter
	mt_gotoXY(4,11*7-3);
	write(1,"instuctionCounter",17);
	mt_gotoXY(5,11*7);
	sc_memoryGet(iC,&value);
	char buf2[8];
	int l2 = sprintf(buf2,"+%.4x",iC-1);
	
	write(1,buf2,l2);
	// Operation
	mt_gotoXY(7,11*7-1);
	write(1,"Operation",9);
	mt_gotoXY(8,11*7);
	write(1,"+00:00",6);
	// Flags
	mt_gotoXY(10,11*7);
	write(1,"Flags",5);
	int reg;
	sc_regGet(P,&reg);
	if (reg)
    {
		mt_gotoXY(11,11*7);
		write(1,"P",1);
	}
	sc_regGet(O,&reg);
	if (reg)
    {
		mt_gotoXY(11,11*7-2);
		write(1,"O",1);
	}
	sc_regGet(M,&reg);
	if (reg)
    {
		mt_gotoXY(11,11*7+2);
		write(1,"M",1);
	}
	sc_regGet(T,&reg);
	if (reg)
    {
		mt_gotoXY(11,11*7+4);
		write(1,"T",1);
	}
	sc_regGet(E,&reg);
	if (reg)
    {
		mt_gotoXY(11,11*7+6);
		write(1,"E",1);
	}
	// Keys
	mt_gotoXY(13,8*7);
	write(1,"Keys",4);
	mt_gotoXY(14,8*7-2);
	write(1,"l - load",8);
	mt_gotoXY(15,8*7-2);
	write(1,"s - save",8);
	mt_gotoXY(16,8*7-2);
	write(1,"r - run",7);
	mt_gotoXY(17,8*7-2);
	write(1,"t - step",8);
	mt_gotoXY(18,8*7-2);
	write(1,"i - reset",9);
	mt_gotoXY(19,8*7-2);
	write(1,"F5 - accumulator",17);
	mt_gotoXY(20,8*7-2);
	write(1,"F6 - instructionCounter",23);
	// +9999
	char buf1[8];
	int l1 = sprintf(buf1,"+%.4x",value);
	int mass[8];
	for(i = 0; i < 5; i++)
    {
		switch(buf1[i]){
			case 'a': mass[i] = 10; break;
			case 'b': mass[i] = 11; break;
			case 'c': mass[i] = 12; break;
			case 'd': mass[i] = 13; break;
			case 'e': mass[i] = 14; break;
			case 'f': mass[i] = 15; break;
			case '+': mass[i] = 16; break;
			default: mass[i] = buf1[i] - '0'; break;
		}
	}
	bc_printbigchar(big[mass[0]],14,2,BLACK,WHITE);
	bc_printbigchar(big[mass[1]],14,11,BLACK,WHITE);
	bc_printbigchar(big[mass[2]],14,20,BLACK,WHITE);
	bc_printbigchar(big[mass[3]],14,29,BLACK,WHITE);
	bc_printbigchar(big[mass[4]],14,38,BLACK,WHITE);
	mt_gotoXY(24,1);
} 


void timer_on(int signo)
{ // функция для таймера
	int t;
	sc_regGet(T,&t); // ставим флаг T
	if(t == 0){ // проверяем, если флаг установлен
		if(iC < 100){ // если позиция меньше 100
 			iC++; // увеличиваем позицию
			draw(); // перерисовываем
			alarm(1); // продолжаем таймер
		} else { // если дошли до максимальной позиции
			sc_regSet(T,1); // обнуляем флаг 
			draw(); // перерисовываем 
			alarm(0); // останавливаем таймер
		}
	} else { // если флаг не установлен
		sc_regSet(T,1); 
		draw(); // перерисовываем 
		alarm(0); // останавливаем таймер
	}
}

void timer_off(int signo)
{ // при выключении таймера
	sc_regSet(T,1); // устанавливаем флаг в 0
	draw(); // перерисовываем
	alarm(0); // останавливаем таймер
}

void move()
{ // функция для перемещения по таймеру
	signal(SIGALRM, timer_on); // по сигналу таймера вызываем функцию timer_on
	signal(SIGUSR1, timer_off); // по любому сигналу от пользователя вызываем timer_off
	draw(); // перерисовываем
}


int ALU(int command, int operand)
{
	switch(command){
		case 2*0x40: iC = operand; draw(); break;
		case 2*0x41: if(t1 < 0){ iC = operand; draw(); } break;
		case 2*0x42: if(t1 == 0){ iC = operand; draw(); } break;
		case 2*0x43: raise(SIGUSR1); break;
	}
}

int main()
{ 
	struct itimerval nval, oval; //структура, описывающая таймер 
	nval.it_interval.tv_sec=2; //секунды
	nval.it_interval.tv_usec=500; //микросекунды
	nval.it_value.tv_sec=1; /*начальное */
	nval.it_value.tv_usec=0; /*значение*/
	// инициализируем флагов
	sc_regInit(); 
	sc_regSet(T,1);
	//mt_setbgcolor(BLACK);
	mt_setfgcolor(WHITE);
	// получим размеры экрана
	int r, c;
	mt_getscreensize(&r,&c);
	sc_memoryInit();
	rk_mytermregime(0,50,0,0,1);
	// Рамки
	draw();
	enum keys key;
	while(1){
            rk_readkey(&key);  
	    int temp10;
		if(key == 14){
	        	raise(SIGUSR1);
			sc_memoryInit();
			iC = iC+0;
			draw();
		}
	    sc_regGet(T,&temp10);
	    if(temp10 == 0){
		if(key == 5){
	        	raise(SIGUSR1);
			sc_memoryInit();
			iC = 1;
			draw();
		}
	    } else {
	    switch(key){
		    case 2: sc_memorySave("save.o"); break;
		    case 3: { 
			int temp;
			sc_regGet(T,&temp);
			if(temp){
				sc_regSet(T,0); 
				setitimer(ITIMER_REAL, &nval, &oval); 
				move(); 
			} else {
				sc_regSet(T,1);
				alarm(0);
				draw();
			}
		    } break;
		    case 1: sc_memoryLoad("save.o"); draw(); break;
		    case 9: if(iC > 10) iC -= 10; draw(); break;
		    case 8: if(iC <= 90) iC += 10; draw(); break;
		    case 10: if(iC > 1) iC--; draw(); break;
		    case 11: if(iC < 100) iC++; draw(); break;
		    case 12: {
			if(status == 1){
				write(1,"Input: ",7);
				rk_mytermregime(1,0,50,1,1);
				read(1,&temp,4);
				sscanf(temp,"%x",&t2);
				status = 0;
				mt_clrscr();
				draw();
			} else if(status == 0){
				if(verifMemory(t2) == 0)
					sc_memorySet(iC,t2);
				rk_mytermregime(0,50,0,0,1);
				status = 1;
				mt_clrscr();
				draw();
			}
		    } break;
		    case 6: { 
			if(statusAc == 1){
				sc_memoryGet(iC,&t1);
				statusAc = 0;
			} else if(statusAc == 0){
				sc_memorySet(iC,t1);
				statusAc = 1;
				draw();
			}
	  	    } break;
		    case 7: {
			if(statusIc == 1){
				write(1,"Input: ",7);
				rk_mytermregime(1,0,50,1,1);
				read(1,&temp1,4);
				sscanf(temp1,"%x",&t2);
				iC = t2;
				mt_clrscr();
				rk_mytermregime(0,50,0,0,1);
				draw();
			}
			
		    } break;
		    case 5: {
	 		sc_memoryInit();
			sc_regInit();
			draw();
		    } break;
		    case 4: {
			iC++; 
			draw();
		    } break;
	    }
	}
	}
	rk_mytermregime(1,0,50,1,1);
   	return 0;
}
