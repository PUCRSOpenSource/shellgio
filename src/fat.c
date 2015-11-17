#include <stdio.h>
#include <string.h>
#include <fat.h>

int
init(void)
{
	int i;

	ptr_myfat = fopen("fat.part","wb");

	if (!ptr_myfat)
		return 1;


	//Initialize boot_block
	for (i = 0; i < BLOCK_SIZE; i++)
	{
		boot_block[i] = 0xbb;
	}

	//Write boot_block
	fwrite(&boot_block, sizeof(boot_block), 1, ptr_myfat);


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

	return 0;
}

void
update_fat(void)
{
	ptr_myfat = fopen("fat.part", "r+b");

	if (!ptr_myfat)
	{
		printf("Couldn't open fat!\n");
	}
	else
	{
		fseek(ptr_myfat, 1024, SEEK_SET);
		fwrite(&fat, sizeof(fat), 1, ptr_myfat);
	}

	fclose(ptr_myfat);
}

int
get_free_address(void)
{
	int i;
	for (i = 10; i < FAT_SIZE; i++)
	{
		if (fat[i] == 0)
		{
			fat[i] = -1;
			update_fat();
			return i;
		}
	}
	return -1;
}

int 
save_data(int address, union data_cluster file)
{
	 ptr_myfat = fopen("fat.part", "r+b");
	 fseek(ptr_myfat, address * BLOCK_SIZE, SEEK_SET);
	 fwrite(&file, sizeof(file), 1, ptr_myfat);
	 fclose(ptr_myfat);
	 return 0;
}

union data_cluster
load_cluster(int address) 
{
	ptr_myfat = fopen("fat.part", "rb");
	fseek(ptr_myfat, address * BLOCK_SIZE, SEEK_SET);
	union data_cluster cluster;
	fread(&cluster, BLOCK_SIZE, 1, ptr_myfat);
	fclose(ptr_myfat);
	return cluster;
}

int
check_directory_entry(char* name, int address)
{
	union data_cluster root_cluster = load_cluster(address);

	int i;
	for (i = 0; i < BLOCK_SIZE / sizeof(dir_entry_t); i++)
		if (root_cluster.dir[i].attributes == 1 && strcmp((const char*)root_cluster.dir[i].filename, name) == 0)
			return 0;

	for (i = 0; i < BLOCK_SIZE / sizeof(dir_entry_t); i++)
		if (root_cluster.dir[i].attributes != 1 && root_cluster.dir[i].attributes != 2)
			return 1;

	return 0;
}

int
mkdir(char** path, int size)
{
	/*char dir_name[] = "dir";*/

	if (check_directory_entry(path[1], 9))
	{
		// Load addresses
		int address = get_free_address();
		int root_address = 9;

		// Load root cluster
		union data_cluster root_cluster = load_cluster(root_address);

		// Write filename on root dir
		int i;
		for (i = 0; i < BLOCK_SIZE / sizeof(dir_entry_t); i++)
		{
			if (root_cluster.dir[i].attributes != 1 && root_cluster.dir[i].attributes != 2)
			{
				int j;
				/*for(j = 0; j < strlen(dir_name); j++) */
				for(j = 0; j < strlen(path[1]); j++) 
				{
					/*root_cluster.dir[i].filename[j] = dir_name[j];*/
					root_cluster.dir[i].filename[j] = path[1][j];
				}
				root_cluster.dir[i].attributes = 1;
				root_cluster.dir[i].first_block = address;
				break;
			}
		}
		save_data(root_address,root_cluster);

		// Create File
		union data_cluster new_file;
		save_data(address, new_file);

		return 0;
	}

	return 1;
}

int
ls(void)
{
	// Loads root cluster
	int root_address = 9;
	union data_cluster root_cluster = load_cluster(root_address);

	// Prints All Filenames
	int i;
	for (i = 0; i < BLOCK_SIZE / sizeof(dir_entry_t); i++) 
	{
		if (root_cluster.dir[i].attributes == 1 || root_cluster.dir[i].attributes == 2) 
		{
			printf("\t%s", root_cluster.dir[i].filename);
		}
	}

	return 0;

}
