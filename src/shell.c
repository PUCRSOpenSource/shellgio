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
sergio(void)
{
	char const* const fileName = "data/sergio.txt";
	FILE* file = fopen(fileName, "r");
	char line[256];

	while (fgets(line, sizeof(line), file))
		printf("%s", line); 

	fclose(file);
}

void
pai_de_familia(void)
{
	printf("Pai de Família em Japonês - https://www.youtube.com/watch?v=CKuP0c9lGBM\n");
}

static int
file_exists(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (file)
	{
		fclose(file);
		return 1;
	}
	return 0;
}

static char**
parse_command(char* str, int* depth)
{
	char** res  = NULL;
	char*  p    = strtok (str, "/");
	int n_spaces = 0;

	while (p)
	{
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

void
help(void)
{
	printf("doge                                     - prints doge\n");
	printf("sergio                                   - prints sergio's picture\n");
	printf("paidefamilia                             - prints the most amazing youtube link ever\n");
	printf("clear                                    - clears the screen\n");
	printf("init                                     - initialize file system, format disk\n");
	printf("load                                     - load file allocation table\n");
	printf("ls [/caminho/diretorio]                  - list directory\n");
	printf("mkdir [/caminho/diretorio]               - make directory\n");
	printf("rmdir [/caminho/diretorio]               - remove directory\n");
	printf("create [/caminho/arquivo]                - create file\n");
	printf("rm [/caminho/arquivo]                    - remove file\n");
	printf("write [\"STRING\"] [/caminho/arquivo]      - write data in file\n");
	printf("append [\"STRING\"]  [/caminho/arquivo]    - write data in file\n");
	printf("read [/caminho/arquivo]                  - concatenate files and print on the standard output\n");
	printf("memprint                                 - prints filesystem memory dump on the screen\n");
	printf("exit                                     - exit shell\n");
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

static char*
parse_string_command(char* command, char* string)
{
	char* p   = strtok (command, "\"");
	char* aux = strtok(NULL, "\"");

	if (aux)
		while((*string++=*aux++));

	return p;
}

int
start_shell(void)
{
	while (1)
	{
		if (!file_exists("fat.part"))
			printf("\e[1;31mFILE SYSTEM NOT FOUND, USE init\e[0m\n");
		else
			if (!g_fat_loaded)
				printf("\e[1;31mFAT NOT LOADED, USE load\e[0m\n");

		printf("Super \e[0;33mShellgio\e[0m \e[1;32m2000\e[0m $ ");
		char command[4096];
		fgets(command,4096,stdin);


		int path_depth = 0;
		char** res = parse_command(command, &path_depth);

		char* string_arg = malloc(sizeof(char)*4096);
		res[0] = parse_string_command(res[0], string_arg);

		int i;
		for (i = 0; i < path_depth; i++)
			res[i] = stripChars(res[i], "\n\\ ");

		if (strcmp(res[0], "init") == 0)
			init();

		if (strcmp(res[0], "load") == 0)
			load();

		if (strcmp(res[0], "mkdir") == 0)
			if (mkdir(res, path_depth))
				printf("\tOH NOES! Either directory already exists or no more directory entries are available!\n");

		if (strcmp(res[0], "create") == 0)
			create(res, path_depth);

		if (strcmp(res[0], "exit") == 0)
			return 0;

		if (strcmp(res[0], "help") == 0)
			help();

		if (strcmp(res[0], "ls") == 0)
			ls(res, path_depth);

		if (strcmp(res[0], "clear") == 0)
			system("clear");

		if (strcmp(res[0], "memprint") == 0)
			system("hexdump -C fat.part");

		if (strcmp(res[0], "doge") == 0)
			doge();

		if (strcmp(res[0], "paidefamilia") == 0)
			pai_de_familia();

		if (strcmp(res[0], "sergio") == 0)
			sergio();

		if (strcmp(res[0], "unlink") == 0)
			unlink(res, path_depth);

		if (strcmp(res[0], "write") == 0)
			write(res, path_depth, string_arg);
			/*write(res, path_depth, "iaeuehfeuiahfuiaehfuieahuifaheuifhaeufuaeuifhaeufhaeuihfaeuihfuiaehfuiaehaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahfiuaehuifaheuifhuiaeufhaieuhuiaefhiaeuehfeuiahfuiaehfuieahuifaheuifhaeufuaeuifhaeufhaeuihfaeuihfuiaehfuiaehfiuaehuifaheuifhuiaeufhaieuhuiaefhiaeuehfeuiahfuiaehfuieahuifaheuifhaeufuaeuifhaeufhaeuihfaeuihfuiaehfuiaehaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahfiuaehuifaheuifhuiaeufhaieuhuiaefhiaeuehfeuiahfuiaehfuieahuifaheuifhaeufuaeuifhaeufhaeuihfaeuihfuiaehfuiaehfiuaehuifaheuifhuiaeufhaieuhuiaefhiaeuehfeuiahfuiaehfuieahuifaheuifhaeufuaeuifhaeufhaeuihfaeuihfuiaehfuiaehaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahfiuaehuifaheuifhuiaeufhaieuhuiaefhiaeuehfeuiahfuiaehfuieahuifaheuifhaeufuaeuifhaeufhaeuihfaeuihfuiaehfuiaehfiuaehuifaheuifhuiaeufhaieuhuiaefhiaeuehfeuiahfuiaehfuieahuifaheuifhaeufuaeuifhaeufhaeuihfaeuihfuiaehfuiaehaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahfiuaehuifaheuifhuiaeufhaieuhuiaefhiaeuehfeuiahfuiaehfuieahuifaheuifhaeufuaeuifhaeufhaeuihfaeuihfuiaehfuiaehfiuaehuifaheuifhuiaeufhaieuhuiaefhiaeuehfeuiahfuiaehfuieahuifaheuifhaeufuaeuifhaeufhaeuihfaeuihfuiaehfuiaehaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahaoueghuoaehguoaehguoaehguoaehguaehoughaeogueahfiuaehuifaheuifhuiaeufhaieuhuiaefhiaeuehfeuiahfuiaehfuieahuifaheuifhaeufuaeuifhaeufhaeuihfaeuihfuiaehfuiaehfiuaehuifaheuifhuiaeufhaieuhuiaefh AQUI COM CERTEZA EH O SEGUNDO CLUSTER");*/

		if (strcmp(res[0], "read") == 0)
			read(res, path_depth);

		if (strcmp(res[0], "append") == 0)
			append(res, path_depth, string_arg);

		free(string_arg);
		free_command_array(res, path_depth);
	}

	return 0;
}
