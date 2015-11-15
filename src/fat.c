#include <stdio.h>
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

static void
print_fat(void)
{
	printf("[ ");
	int i;
	for (int i = 0; i < FAT_SIZE; ++i)
	{
			printf("%d ", fat[i]);
	}
	printf("]\n");
}

int
get_free_address(void)
{
	int i;
	for (i = 10; i < FAT_SIZE; i++)
	{
		if (fat[i] == 0)
		{
			fat[i] = 1;
			update_fat();
			return i;
		}
	}
	return -1;
}

int 
load_cluster(int address)
{
	 ptr_myfat = fopen("fat.part", "r+b");

	 printf("%d\n", address * BLOCK_SIZE);
	 fseek(ptr_myfat, address * BLOCK_SIZE, SEEK_SET);
	 union data_cluster teste = { .data[0] = 0xaaaa };
	 int* ptr = &teste;
	 fwrite(ptr, sizeof(teste), 1, ptr_myfat);
	 fclose(ptr_myfat);
	 return 0;
}

int
mkdir(void)
{
	//parse path
	//find directory
	//write to directory
	//find free address
	//write data to free cluster

	int address = get_free_address();

	load_cluster(address);

	return 0;
}
