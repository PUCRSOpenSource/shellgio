#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FAT_SIZE 4096
#define BLOCK_SIZE 1024
#define DATA_CLUSTERS 4086
#define FILE_NAME_SIZE 8
#define RESERVED_SIZE 7

uint16_t fat[FAT_SIZE];

typedef struct {
        uint8_t  filename[FILE_NAME_SIZE];
        uint8_t  attributes;
        uint8_t  reserved[RESERVED_SIZE];
        uint16_t first_block;
        uint32_t size;
} dir_entry_t;

dir_entry_t dir[32];

uint8_t data_block[BLOCK_SIZE];

FILE *ptr_myfat;

//Root block
uint8_t root_block[BLOCK_SIZE];

//Root dir
uint8_t root_dir[BLOCK_SIZE];

//Data cluster
uint8_t cluster[BLOCK_SIZE];

int
main(int argc, const char *argv[])
{
        int i;

        ptr_myfat = fopen("fat.part","wb");


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
