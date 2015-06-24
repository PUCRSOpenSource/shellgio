#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define FAT_SIZE 4096
#define BLOCK_SIZE 1024
#define DATA_CLUSTERS 4086
#define FILE_NAME_SIZE 8
#define RESERVED_SIZE 7

typedef enum{false, true} bool;

typedef struct {
        uint8_t  filename[FILE_NAME_SIZE];
        uint8_t  attributes;
        uint8_t  reserved[RESERVED_SIZE];
        uint16_t first_block;
        uint32_t size;
} dir_entry_t;

uint16_t fat[FAT_SIZE];
dir_entry_t dir[32];
uint8_t data_block[BLOCK_SIZE];
FILE* ptr_myfat;
uint8_t root_block[BLOCK_SIZE];
uint8_t root_dir[BLOCK_SIZE];
uint8_t cluster[BLOCK_SIZE];

char*
ltrim(char* s)
{
        if(s==NULL)
        {
                return s;
        }
        while(isspace(*s)) s++;
        return s;
}

char*
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

int
init(void)
{
        int i;

        ptr_myfat = fopen("fat.part","wb");

        if (!ptr_myfat)
                return 1;


        //Initialize root_block
        for (i = 0; i < BLOCK_SIZE; i++)
        {
                root_block[i] = 0xbb;
        }

        //Write root_block
        fwrite(&root_block, sizeof(root_block), 1, ptr_myfat);


        //Initialize fat
        fat[0] = 0xfffd;
        for (i = 1; i < 9; i++)
        {
                fat[i] = 0xfffe;
        }
        fat[9] = 0xffff;
        for (i = 10; i < FAT_SIZE; i++)
        {
                fat[i] = 0x0000;
        }

        //Write fat
        fwrite(&fat, sizeof(fat), 1, ptr_myfat);


        //Initialize root_dir
        for (i = 0; i < BLOCK_SIZE; i++)
        {
                root_dir[i] = 0x00;
        }

        //Write root_dir
        fwrite(&root_dir, sizeof(root_dir), 1, ptr_myfat);


        //Data clusters
        for (i = 0; i < DATA_CLUSTERS; i++)
        {
                fwrite(&cluster, sizeof(cluster), 1, ptr_myfat);
        }


        fclose(ptr_myfat);

        return 0;
}

int
load(void)
{ 
        ptr_myfat = fopen("fat.part", "rb"); 

        if (!ptr_myfat)
        {
                return 1;
        }

        fseek(ptr_myfat, 1024, SEEK_SET);
        fread(&fat, sizeof(fat), 1, ptr_myfat);

        fclose(ptr_myfat);
}

void
updateFat(void)
{
        ptr_myfat = fopen("fat.part", "r+b"); 

        if (!ptr_myfat)
        {
                return 1;
        }

        fseek(ptr_myfat, 1024, SEEK_SET);
        fwrite(&fat, sizeof(fat), 1, ptr_myfat);
        
        fclose(ptr_myfat);
}

uint16_t*
freeFat(void)
{
        int i;
        for (i = 10; i < FAT_SIZE; i++)
        {
                if (fat[i] == 0)
                {
                        fat[i] = i;
                        updateFat();
                        return fat[i];
                }
        }
        return -1;
}

int
mkdir(void)
{
        uint16_t* fFat = freeFat();
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
main(int argc, const char *argv[])
{
        help();

        while (true)
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
