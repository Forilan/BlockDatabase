#include <stdio.h>
#include <uv.h>
#include "stdlib.h"
#include <time.h>
#include "settings.h"
#include "memory.h"
#include "storage.h"
#include "network.h"

void testSetBlock();
void testGetBlock();

int main()
{
	block_s * block = malloc(sizeof(block_s));
	block->cx = 0;
	block->cy = 0;
	block->x = 0;
	block->y = 0;
	block->z = 0;
	block->type = 15;

	saveBlock(block);

	 // Load files and put them into memory
     initWorld();
	 loadWorld();

	 printf("Starting Storage Processor\n");
	 runStorageProcessor();

	 printf("Starting Server\n");
	 if (!startServer(SERVER_NAME, SERVER_PORT))
	 {
		 printf("Error binding to server ip/port\n");
	 }

	 return 0;
}

void testSetBlock()
{
	//setBlock(0,0,0,0,0,3000);
	//setBlock(0,0,0,0,1,3000);
	 printf("Setting \n");
	 for (char i = 0; i < WORLD_LENGTH; i++)
	 {
		 for (char j = 0; j < WORLD_WIDTH; j++)
		 {
			for (unsigned char k = 0; k < 16; k++)
			{
				for (unsigned char l = 0; l < 16; l++)
				{
					for (unsigned short m = 0; m < 256; m++)
					{
						setBlock(i,j,k,l,m,m+l+k+j+i);
					}
				}
			}
		 }
	 }

	 printf("Done Setting\n");
}

void testGetBlock() {
	 printf("Getting \n");


	 for (char i = 0; i < WORLD_LENGTH; i++)
	 	 {
	 		 for (char j = 0; j < WORLD_WIDTH; j++)
	 		 {
	 			for (unsigned char k = 0; k < 16; k++)
	 			{
	 				for (unsigned char l = 0; l < 16; l++)
	 				{
	 					for (unsigned short m = 0; m < 256; m++)
	 					{
	 						getBlock(i,j,k,l,m);
	 					}
	 				}
	 			}
	 		 }
	 	 }
	 printf("Done Getting\n");
}
