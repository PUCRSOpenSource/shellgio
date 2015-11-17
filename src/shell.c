#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fat.h>
#include <shell.h>

static char**
parse_command(char* str, int* depth)
{
	char** res  = NULL;
	char*  p    = strtok (str, "/");
	int n_spaces = 0;

	while (p) {
		res = realloc (res, sizeof (char*) * ++n_spaces);

		if (res == NULL)
			exit (-1);

		char* aux = malloc(sizeof(char*) * strlen(p) + 1);
		strcpy(aux, p);

		res[n_spaces-1] = aux;

		p = strtok (NULL, "/");
		(*depth)++;
	}

	res = realloc (res, sizeof (char*) * (n_spaces+1));
	res[n_spaces] = 0;

	return res;
}

static void
free_command_array(char** command, int size)
{
	int i;
	for (i = 0; i < size; i++)
		free(command[i]);
	free(command);
}

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

		int directory_depth = 0;
		char** res = parse_command(command, &directory_depth);

		int i;
		for (i = 0; i < directory_depth; i++)
			printf("res[%d]: %s\n", i, res[i]);
		printf("Size: %d\n", directory_depth);

		res[0] = rtrim(ltrim(res[0]));

		if (strcmp(res[0], "init") == 0)
		{
			init();
		}

		if (strcmp(res[0], "load") == 0)
		{
			load();
		}

		if (strcmp(res[0], "mkdir") == 0)
		{
			mkdir();
		}

		if (strcmp(res[0], "exit") == 0)
		{
			return 0;
		}

		if (strcmp(res[0], "help") == 0)
		{
			help();
		}

		if (strcmp(res[0], "ls") == 0)
		{
			ls();
		}

		free_command_array(res, directory_depth);
	}

	return 0;
}
