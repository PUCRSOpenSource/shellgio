#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fat.h>
#include <shell.h>

void
doge(void)
{
	printf("                Y.                      _             \n");
	printf("                YiL                   .```.           \n");
	printf("  much files    Yii;                .; .;;`.          \n");
	printf("                YY;ii._           .;`.;;;; :          \n");
	printf(" such shell     iiYYYYYYiiiii;;;;i` ;;::;;;;          \n");
	printf("            _.;YYYYYYiiiiiiYYYii  .;;.   ;;;          \n");
	printf("wow      .YYYYYYYYYYiiYYYYYYYYYYYYii;`  ;;;;          \n");
	printf("       .YYYYYYY$$YYiiYY$$$$iiiYYYYYY;.ii;`..          \n");
	printf("      :YYY$!.  TYiiYY$$$$$YYYYYYYiiYYYYiYYii.         \n");
	printf("      Y$MM$:   :YYYYYY$! `` 4YYYYYiiiYYYYiiYY.        \n");
	printf("   `. :MM$$b.,dYY$$Yii  :'   :YYYYllYiiYYYiYY         \n");
	printf("_.._ :`4MM$!YYYYYYYYYii,.__.diii$$YYYYYYYYYYY         \n");
	printf(".,._ $b`P`      4$$$$$iiiiiiii$$$$YY$$$$$$YiY;        \n");
	printf("   `,.`$:       :$$$$$$$$$YYYYY$$$$$$$$$YYiiYYL       \n");
	printf("     `;$$.    .;PPb$`.,.``T$$YY$$$$YYYYYYiiiYYU:      \n");
	printf("    ;$P$;;: ;;;;i$y$ !Y$$$b;$$$Y$YY$$YYYiiiYYiYY      \n");
	printf("    $Fi$$ .. ``:iii.`- :YYYYY$$YY$$$$$YYYiiYiYYY      \n");
	printf("    :Y$$rb ````  `_..;;i;YYY$YY$$$$$$$YYYYYYYiYY:     \n");
	printf("     :$$$$$i;;iiiiidYYYYYYYYYY$$$$$$YYYYYYYiiYYYY.    \n");
	printf("      `$$$$$$$YYYYYYYYYYYYY$$$$$$YYYYYYYYiiiYYYYYY    \n");
	printf("      .i!$$$$$$YYYYYYYYY$$$$$$YYY$$YYiiiiiiYYYYYYY    \n");
	printf("     :YYiii$$$$$$$YYYYYYY$$$$YY$$$$YYiiiiiYYYYYYi'    \n");
}

void
pai_de_familia(void)
{
	printf("Pai de Família em Japonês - https://www.youtube.com/watch?v=CKuP0c9lGBM\n");
}

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

static char*
stripChars(const char *string, const char *chars)
{
	char* newstr = malloc(strlen(string) + 1);

	int counter = 0;
	while(*string)
	{
		if (!strchr(chars, *string))
		{
			newstr[counter] = *string;
			counter++;
		}
		string++;
	}

	newstr[counter] = 0;
	return newstr;
}

int
start_shell(void)
{
	while (1)
	{
		if (!g_fat_loaded)
			printf("\e[1;31mFAT NOT LOADED\e[0m\n");
		printf("Super Shell 2000$ ");
		char command[4096];
		fgets(command,96,stdin);


		int path_depth = 0;
		char** res = parse_command(command, &path_depth);

		int i;
		for (i = 0; i < path_depth; i++)
		{
			/*printf("res[%d]: %s\n", i, res[i]);*/
			res[i] = stripChars(res[i], "\n\\ ");
		}
		/*printf("Size: %d\n\n\n", path_depth);*/

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
			if (mkdir(res, path_depth))
				printf("\tOH NOES! Either directory already exists or no more directory entries are available!\n");
		}

		if (strcmp(res[0], "create") == 0)
		{
			create(res, path_depth);
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
			ls(res, path_depth);
		}

		if (strcmp(res[0], "clear") == 0)
		{
			system("clear");
		}

		if (strcmp(res[0], "memprint") == 0)
		{
			system("hexdump fat.part");
		}

		if (strcmp(res[0], "doge") == 0)
		{
			doge();
		}

		if (strcmp(res[0], "paidefamilia") == 0)
		{
			pai_de_familia();
		}

		free_command_array(res, path_depth);
	}

	return 0;
}
