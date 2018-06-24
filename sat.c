#include "sat.h"

int READ,ACC,iC=0;


int ALU(int command, int addr, int *operand)
{
	int val;
	switch(command)
	{
		case 0x10://READ
			
			sc_memoryGet(iC,&READ);

		break;

		case 0x11://WRITE

			sc_memoryGet(operand[addr],&val);
			sc_memorySet(iC,val);
			iC++;

		break;

		case 0x20://LOAD
			sc_memoryGet(operand[addr],&ACC);
		
		break;

		case 0x21://STORE
			sc_memorySet(operand[addr],ACC);
			iC++;
		break;

		case 0x30://ADD
			sc_memoryGet(operand[addr],&val);
			ACC+=val;
			
		break;

		case 0x31://SUB
			sc_memoryGet(operand[addr],&val);
			ACC-=val;
		break;

		case 0x32://DIVIDE
			sc_memoryGet(operand[addr],&val);
			ACC/=val;
		break;

		case 0x33://MUL
			sc_memoryGet(operand[addr],&val);
			ACC*=val;
		break;

		case 0x57://JNC
			if (ACC<=0x9999)iC=operand[addr];
		break;

		break;
	}
	return 0;
}


int str2command(char *str)
{
	int ret;

	if (strcmp(str, "READ") == 0)
		ret = 0x10;
	else if (strcmp(str, "WRITE") == 0)
		ret = 0x11;
	else if (strcmp(str, "LOAD") == 0)
		ret = 0x20;
	else if (strcmp(str, "STORE") == 0)
		ret = 0x21;
	else if (strcmp(str, "ADD") == 0)
		ret = 0x30;
	else if (strcmp(str, "SUB") == 0)
		ret = 0x31;
	else if (strcmp(str, "DIVIDE") == 0)
		ret = 0x32;
	else if (strcmp(str, "MUL") == 0)
		ret = 0x33;
	else if (strcmp(str, "HALT") == 0)
		ret = 0x43;
	else if (strcmp(str, "JNC") == 0)
		ret = 0x57;
	else
		ret = -1;
	
	return ret;
}

int pars_line(char *str, int *addr, int *value,int *add_mem,int *cmd_mem)
{
	char *ptr;
	int operand, command;
	int flag_assign = 0;
	
	ptr = strchr(str, ';');
	if (ptr != NULL)  *ptr = '\0';
	ptr = strchr(str, '\n');
	if (ptr != NULL) *ptr = '\0';
	ptr = strtok(str, " \t");
	if (ptr == NULL) return EMPTY_STR;
	if (sscanf(ptr, "%d", addr) != 1)
	{
		return ERR_ARG1;
	}

	if ((*addr < 0) || (*addr >= 100)) return ERR_ARG1;

	ptr = strtok(NULL, " \t");
	//printf("%s\n",ptr);  //CMD

	if (ptr == NULL) return ERR_FEW;
	else
		if (strcmp(ptr, "=") == 0)
		{
			//printf("f");
			cmd_mem[*addr]=0x0;
			flag_assign = 1;
		}
		else
		{
			command = str2command(ptr);
			cmd_mem[*addr]=command;//cmd
			if (command == -1)
				return ERR_ARG2;
		}


	ptr = strtok(NULL, " \t");
	if (!flag_assign)
	{
		sscanf(ptr, "%d", &add_mem[*addr]);
	}
	else
	{
		sscanf(ptr+1, "%x", &add_mem[*addr]);
	}

	if (ptr == NULL)return ERR_FEW;
	ptr = strtok(NULL, " \t");

	if (ptr != NULL)return ERR_MANY;
	
	return 0;
}

void print_error(char *line, int line_cnt, int err)
{
	switch (err) {
		case ERR_ARG1:
			fprintf(stderr, "%d: first argument isn't a number\n", line_cnt);
			break;
		
		case ERR_ARG2:
			fprintf(stderr, "%d: unknown command\n", line_cnt);
			break;
		
		case ERR_FEW:
			fprintf(stderr, "%d: should be three aguments\n", line_cnt);
			break;
		
		case ERR_MANY:
			fprintf(stderr, "%d: too many arguments\n", line_cnt);
			break;
	}
	fprintf(stderr, "%s", line);
}

int test_argv(char *argv[])
{
	char *ptr1, *ptr2;
	
	ptr1 = strrchr(argv[1], '.');
	ptr2 = strrchr(argv[2], '.');
	if ((strcmp(ptr1, ".o") != 0) || (strcmp(ptr2, ".sa") != 0))
		return -1;
	else
		return 0;
}

int main(int argc, char *argv[])
{

    sc_memoryLoad(argv[1]);
    
	char buf[256], line[256];
	int add_mem[100],cmd_mem[100];
	FILE *input;
	int value, command, operand, addr, line_cnt = 1;
	int err;
	int flag_err = 0;
	
	if (argc != 3) {
		perror("Incorrect arguments!\n");
		exit(1);
	}
	if (test_argv(argv) != 0) {
		perror("Incorrect arguments!\n");
		exit(1);
	}

	if ((input = fopen(argv[2], "rb")) == NULL) {
		fprintf(stderr, "Cannot open file:%s", argv[2]);
		exit(1);
	}
	
	while (fgets(line, 256, input)) {
		strcpy(buf, line);
		err = pars_line(buf, &addr, &value, &add_mem, &cmd_mem);
		if (err == 0) {

			if (cmd_mem[addr] == 0) {
				sc_memorySet(addr,add_mem[addr]);
			}

		}
		else if (err < 0) {
			print_error(line, line_cnt, err);
			flag_err = 1;
		}
		line_cnt++;
	}

	for(int i=1;i<=addr;i++)
	{
		ALU(cmd_mem[i], i, add_mem);
	}

	if (flag_err == 0)
	{
		sc_memorySave(argv[1]);
	}
	fclose(input);
	return 0;
}