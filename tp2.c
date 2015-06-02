#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FAT_SIZE 4096
#define BLOCK_SIZE 1024

uint16_t fat[FAT_SIZE];

typedef struct {
        uint8_t  filename[8];
        uint8_t  attributes;
        uint8_t  reserved[7];
        uint16_t first_block;
        uint32_t size;
} dir_entry_t;

dir_entry_t dir[32];

uint8_t data_block[BLOCK_SIZE];

int main(int argc, const char *argv[])
{
        int counter;
        FILE *ptr_myfat;

        ptr_myfat = fopen("fat.part","wb");


        //Root block
        uint8_t root_block[BLOCK_SIZE];

        for (counter = 0; counter < BLOCK_SIZE; counter++) {
                root_block[counter] = 0xbb;
        }

        //Write root_block
        fwrite(&root_block, sizeof(root_block), 1, ptr_myfat);


        //Initialize fat
        fat[0] = 0xfffd;
        for (counter = 1; counter < 9; counter++)
        {
                fat[counter] = 0xfffe;
        }
        fat[9] = 0xffff;
        for (counter = 10; counter < FAT_SIZE; counter++)
        {
                fat[counter] = 0x0000;
        }

        //Write fat
        fwrite(&fat, sizeof(fat), 1, ptr_myfat);


        //Root dir
        uint8_t root_dir[BLOCK_SIZE];

        //Initialize root_dir
        for (counter = 0; counter < BLOCK_SIZE; counter++)
        {
                root_dir[counter] = 0x00;
        }

        //Write root_dir
        fwrite(&root_dir, sizeof(root_dir), 1, ptr_myfat);


        //Data clusters
        for (counter = 0; counter < 4086; counter++)
        {
                uint8_t cluster[BLOCK_SIZE];
                fwrite(&cluster, sizeof(cluster), 1, ptr_myfat);
        }


        fclose(ptr_myfat);

        return 0;
}
