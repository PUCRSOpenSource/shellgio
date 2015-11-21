#ifndef FAT_H
#define FAT_H

#include <stdint.h>

#define FAT_SIZE 4096
#define BLOCK_SIZE 1024
#define DATA_CLUSTERS 4086
#define FILE_NAME_SIZE 18
#define RESERVED_SIZE 7
#define ROOT_ADDRESS 9

typedef struct {
	uint8_t  filename[FILE_NAME_SIZE];
	uint8_t  attributes; // 1 if dir, 2 if file
	uint8_t  reserved[RESERVED_SIZE];
	uint16_t first_block;
	uint32_t size;
} dir_entry_t;

 union data_cluster{
	dir_entry_t dir[BLOCK_SIZE / sizeof(dir_entry_t)];
	uint8_t data[BLOCK_SIZE];
};

uint16_t fat[FAT_SIZE];
FILE* ptr_myfat;
uint8_t boot_block[BLOCK_SIZE];
uint8_t root_dir[BLOCK_SIZE];
uint8_t cluster[BLOCK_SIZE];

int get_free_address(void);
void set_fat_address(int address, int value);
int init(void);
int load(void);
int save_data(int address, union data_cluster cluster);
void update_fat(void);
int mkdir(char** path, int size);
int ls(char** path, int size);

#endif /* FAT_H */
