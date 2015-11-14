#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <fat.h>
#include <shell.h>

static char*
ltrim(char* s)
{
	if(s==NULL)
	{
		return s;
	}
	while(isspace(*s)) s++;
	return s;
}

static char*
rtrim(char* s)
{
	if(s==NULL)
	{
		return s;
	}
	char* back = s + strlen(s);
	while(isspace(*--back));
	*(back+1) = '\0';
	return s;
}

void
help(void)
{
	printf("init                       - initialize file system, format disk\n");
	printf("load                       - load file allocation table\n");
	printf("ls [/caminho/diretorio]    - list directory\n");
	printf("mkdir [/caminho/diretorio] - make directory\n");
	printf("rmdir [/caminho/diretorio] - remove directory\n");
	printf("create [/caminho/arquivo]  - create file\n");
	printf("rm [/caminho/arquivo]      - remove file\n");
	printf("write  [/caminho/arquivo]  - write data in file\n");
	printf("cat [/caminho/arquivo]     - concatenate files and print on the standard output\n");
	printf("exit                       - exit shell\n");
}

int
start_shell(void)
{
	help();

	while (1)
	{
		char command[4096];
		fgets(command,96,stdin);
		const char* delimiter = "/";
		char* cm;
		cm = rtrim(ltrim(strtok(command, delimiter)));

		if (strcmp(cm, "init") == 0)
		{
			init();
		}

		if (strcmp(cm, "load") == 0)
		{
			load();
		}

		if (strcmp(cm, "mkdir") == 0)
		{
			mkdir();
		}

		if (strcmp(cm, "exit") == 0)
		{
			return 0;
		}

		if (strcmp(cm, "help") == 0)
		{
			help();
		}
	}

	int i = mkdir();
	printf("%d\n", i);

	return 0;
}
