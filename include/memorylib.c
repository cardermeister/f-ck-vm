#include <stdio.h>
/*"P" - переполнение при выполнении операции,
"O" - ошибка деления на 0
"M" - ошибка выхода за границы памяти
"T" - игнорирование тактовых импульсов
"E" - указана неверная команда*/
int sc_memory[100]; // массив памяти
short int flags; // флаги будут храниться в битах переменной

int sc_memoryInit() { // функция для инициализации оперативной памяти Simple Computer
    int i;
    for(i=0;i<100;i++) // заполняем всю оперативную память нулями 
    	sc_memory[i]=0;
    return 0;
}

int sc_memoryPrint(){ // фукнция для вывода значений из оперативной памяти на экран
    int i;
    for(i = 0; i<100; i++)
	printf("%x ",sc_memory[i]);
    return 0;
}

int sc_memorySet(int address, int value) { // функция для изменения значения в оперативной памяти по определенному адресу
    if(address>=101 || address<0) { // если выход за границы оперативной памяти
	sc_regSet(2,1);
    	return -1;
    }
    sc_memory[address]=value;
    return 0;
}

int sc_memoryGet(int address,int *value) { // возвращает хранимое значение ячейки address
    if(address>101 || address<0) { // если выход за границы оперативной памяти
	sc_regSet(2,1);	
	return -1;
    }
    *value=sc_memory[address];
    return 0;
}

int sc_memorySave(char *filename) { // функция для сохранения оперативной памяти в файл
    FILE *tf;
    tf=fopen(filename,"wb"); // открываем файл на чтение в бинарном режиме
    if(tf){  // если файл удачно открыт
        fwrite((int*)sc_memory,100,sizeof(int),tf); // записываем все 100 значений в файл
        fclose(tf); // закрываем файл 
    } else {
	sc_regSet(4,1);
        return -1;
    }
    return 0;
}

int sc_memoryLoad(char *filename) { // функция для выгрузки оперативной памяти из файла
    FILE *tf;
    tf=fopen(filename,"rb"); // открываем файл на бинарное чтение
    if(tf){ // если файл удачно открыт
        fread((int*)sc_memory,100,sizeof(int),tf);// считываем 100 значений в массив
        fclose(tf); // закрываем файл
    } else {
	sc_regSet(4,1);
        return -1;
    }
    return 0;
}

int sc_regInit(void){ // функция для инициализации флагов
    flags=0; // обнуляем переменную, это равносильно занулению всех флагов
    return 0;
}

int sc_regSet (int reg, int value) { // функция для устанвки флага
    if((reg>4) || (reg<0)){ // если вышли за пределы возможных флагов
	sc_regSet(4,1);
        return -1;
    }
    if(value!=1 && value!=0){ // если пытаемся присвоить не 0 или не 1
	sc_regSet(4,1);
        return -1;
    }
    if(value){
        flags=flags|(1<<(reg-1));  // установка флага reg в единицу
    } else {
        flags=flags&(~(1<<(reg-1)));  // установка флага reg в ноль
    }
    return 0;
}

int sc_regGet(int reg,int *value) {
    if((reg>4) || (reg<0)) { // если вышли за границы флагов
        printf("Error! Invalid register number\n");
	sc_regSet(4,1);
        return -1;
    }
    *value=(flags>>(reg-1))&0x1;  // получение значение определенного флага, flags - переменная, хранящая флаги, reg - номер разряда
    return 0;
}

int sc_commandEncode(int command, int operand, int *value){ // функция для кодирования команды с операндов в 16ую систему счисления
	*value = command * 128 + operand;
	return 0;
}

int sc_commandDecode(int value,int *command,int *operand){ // функция для декодирования из 16ой системы счисления в десятичную
	*command = value/128;
	*operand = value%128;
	return 0;
}

