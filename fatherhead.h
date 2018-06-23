#pragma once
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

//flags
#define P 0
#define O 1
#define M 2
#define T 3
#define E 4

struct termios term;
enum colors{BLACK,RED,GREEN,YELLOW,BLUE,VIOLET,BIRUZ,WHITE};
enum keys {undefined=0, l=1, s=2, r=3, t=4, i=5, f5=6, f6=7, down=8, up=9, k_left=10, k_right=11, enter=12, esc=13,p=14 };

int big[17][2] =
{
	{0x3C428181,0x8181423C},
	{0x070F0707,0x07070707},
	{0x7CC60E1C,0x1870E0FF},
	{0x3C67C303,0x1EC7633E},
	{0x071B3363,0x7F030303},
	{0xFFC0C0FF,0xFF0303FF},
	{0x0C183060,0xFFC3C3FF},
	{0xFF060C18,0xFC303030},
	{0xFFC3C3FF,0xFFC3C3FF},
	{0xFFC3C3FF,0x03060C38},
	{0x7EC3C3FF,0xFFC3C3C3},
	{0x7EC3C3FE,0xFEC3C3FE},
	{0xFFC0C0C0,0xC0C0C0FF},
	{0xFEC3C3C3,0xC3C3C3FE},
	{0xFFC0C0F8,0xF8C0C0FF},
	{0xFFC0C0F8,0xF8C0C0C0},
	{0x181818FF,0xFF181818}
};

int bc_printA(char str);
int bc_box(int x1, int y1, int x2, int y2);
int bc_printbigchar(int big[2], int x, int y, enum colors bg_color, enum colors fg_color);
int bc_setbigcharpos(int *big, int x, int y, int value, int p);
int bc_getbigcharpos(int* big, int x, int y, int* value, int p);
int bc_bigcharwrite(char temp[], int *big, int count);
int bc_bigcharread(char temp[], int *big, int need_count, int *count);
int rk_readkey(enum keys *k);
int rk_mytermsave(void);
int rk_mytermregime(int regime,int vtime,int vmin,int echo,int sigint);
int sc_memoryInit();
int sc_memoryPrint();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address,int *value);
int sc_memorySave(char *filename);
int sc_memoryLoad(char *filename);
int sc_regInit(void);
int sc_regSet (int reg, int value);
int sc_regGet(int reg,int *value);
int sc_commandEncode(int command, int operand, int *value);
int sc_commandDecode(int value,int *command,int *operand);
int mt_clrscr(void);
int mt_gotoXY(int x, int y);
int mt_getscreensize(int *rows, int *cols);
int mt_setfgcolor(enum colors color);
int mt_setbgcolor(enum colors color);