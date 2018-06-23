#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
enum colors{BLACK,RED,GREEN,YELLOW,BLUE,VIOLET,BIRUZ,WHITE};

int bc_printA(char str){  // вывод символа из дополнительной кодировочной таблицы
	int len;
	char buf[255];
	len = sprintf(buf,"\E(0%c\E(B",str);
	write(1,buf,len);
	return 0;
}

int bc_box(int x1, int y1, int x2, int y2){ // рисование box-а
	int i;
	if((x1<0)||(x2<0)||(y1<0)||(y2<0))
		return -1;
	for(i = x1+1; i < x1+x2-1; i++){
		mt_gotoXY(i, y1);
		bc_printA('x');
		mt_gotoXY(i, y1+y2-1);
		bc_printA('x');
	}
	for(i = y1+1; i < y1+y2-1; i++){
		mt_gotoXY(x1, i);
		bc_printA('q');
		mt_gotoXY(x1+x2-1, i);
		bc_printA('q');
	}
	mt_gotoXY(x1, y1);
	bc_printA('l');
	mt_gotoXY(x1+x2-1, y1);
	bc_printA('m');
	mt_gotoXY(x1, y1+y2-1);
	bc_printA('k');
	mt_gotoXY(x1+x2-1, y1+y2-1);
	bc_printA('j');
	return 0;
}

int bc_printbigchar(int big[2], int x, int y, enum colors bg_color, enum colors fg_color){ // вывод "большого" символа
	int i,k=0;
   	write(1,"\E(0",3);
	mt_setbgcolor(bg_color);	
	mt_setfgcolor(fg_color);
	mt_gotoXY(x++,y);
	int z = big[0];
	for(i = 31; i >= 0; i--){
		int temp = (z>>i)&0x1;
		if(temp==1) bc_printA('a');
        	else bc_printA(' ');
        	k++;
        	if(k>=8){
            		mt_gotoXY(x++, y);
            		k=0;
        	}        	
	}
	z = big[1];
	for(i = 31; i >= 0; i--){
		int temp = (z>>i)&0x1;
		if(temp==1) bc_printA('a');
        	else bc_printA(' ');
        	k++;
        	if(k>=8){
            		mt_gotoXY(x++, y);
            		k=0;
        	}        	
	}
	write(1,"\E(B",3);
}

int bc_setbigcharpos(int *big, int x, int y, int value, int p){ // установка значения в большом символе
	if((value != 0) && (value != 1))
		return -1;
	if((x > 8) || (x < 1) || (y > 8) || (y < 1))
		return -1;
	value <<= ((x - 1) * 8) + y - 1;
	if(value == 1)
		*(big+p) |= value;
	else
		*(big+p) &= (~value);
	return 0;
}

int bc_getbigcharpos(int* big, int x, int y, int* value, int p){ // возврат значения в числе
	if((value == NULL) || (x > 8) || (x < 1) || (y > 8) || (y < 1))
		return -1;
	(*value) = ((*(big+p)) >> (((x - 1) * 8) + y - 1)) & 0x1;
	return 0;
}

int bc_bigcharwrite(char temp[], int *big, int count){ // запись значений букв в 
   FILE *f = fopen(temp,"w");
   if(!f) return -1;
   int i;
   for(i = 0; i < count; i++){
	char buf[19];
	int len = sprintf(buf,"%x %x\n",*(big+0),*(big+1));
	fprintf(f,"%s",buf);
   }
   fclose(f);
   return 0;
}

int bc_bigcharread(char temp[], int *big, int need_count, int *count){
   if (need_count==0) return -1;
   *count=0;
   int i;
   FILE* f = fopen(temp,"r");
   for(i = 0; i < need_count; i++){
	int buf[2];
	fscanf(f,"%x",&buf[0]);
	*count += 1;
   }
   return 0;
}
