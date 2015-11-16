#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fat.h>
#include <shell.h>

static char**
parse_command(char* str)
{
	/*char    str[]= "/home/matthias/repos/t2sisop";*/
	char ** res  = NULL;
	char *  p    = strtok (str, "/");
	printf("Size of %s: %d\n", p, strlen(p));
	int n_spaces = 0;


	/* split string and append tokens to 'res' */

	while (p) {
		res = realloc (res, sizeof (char*) * ++n_spaces);

		if (res == NULL)
			exit (-1); /* memory allocation failed */

		char* aux = malloc(sizeof(char*) * strlen(p) + 1);
		strcpy(aux, p);

		res[n_spaces-1] = aux;

		p = strtok (NULL, "/");
	}

	/* realloc one extra element for the last NULL */

	res = realloc (res, sizeof (char*) * (n_spaces+1));
	res[n_spaces] = 0;

	/* print the result */

	/*int i;*/
	/*for (i = 0; i < (n_spaces+1); ++i)*/
		/*printf ("res[%d] = %s\n", i, res[i]);*/

	/* free the memory allocated */

	/*free (res);*/

	return res;
}

/*static char**/
/*ltrim(char* s)*/
/*{*/
	/*if(s==NULL)*/
	/*{*/
		/*return s;*/
	/*}*/
	/*while(isspace(*s)) s++;*/
	/*return s;*/
/*}*/

/*static char**/
/*rtrim(char* s)*/
/*{*/
	/*if(s==NULL)*/
	/*{*/
		/*return s;*/
	/*}*/
	/*char* back = s + strlen(s);*/
	/*while(isspace(*--back));*/
	/**(back+1) = '\0';*/
	/*return s;*/
/*}*/

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
	/*help();*/
	char str[] = "/home/matthias/repos/t2sisop";
	char** res = parse_command(str);

	printf("%s\n", *res);
	printf("%s\n", *(res + 1));
	printf("%s\n", *(res + 2));
	printf("%s\n", *(res + 3));

	free(res);

	/*while (1)*/
	/*{*/
		/*char command[4096];*/
		/*fgets(command,96,stdin);*/
		/*const char* delimiter = "/";*/
		/*char* cm;*/
		/*cm = rtrim(ltrim(strtok(command, delimiter)));*/

		/*if (strcmp(cm, "init") == 0)*/
		/*{*/
			/*init();*/
		/*}*/

		/*if (strcmp(cm, "load") == 0)*/
		/*{*/
			/*load();*/
		/*}*/

		/*if (strcmp(cm, "mkdir") == 0)*/
		/*{*/
			/*mkdir();*/
		/*}*/

		/*if (strcmp(cm, "exit") == 0)*/
		/*{*/
			/*return 0;*/
		/*}*/

		/*if (strcmp(cm, "help") == 0)*/
		/*{*/
			/*help();*/
		/*}*/
	/*}*/

	/*int i = mkdir();*/
	/*printf("%d\n", i);*/

	return 0;
}
