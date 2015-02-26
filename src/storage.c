/*
 * storage.c
 *
 *  Created on: Aug 12, 2013
 *      Author: jt
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "settings.h"
#include "util.h"
#include "storage.h"
#include "memory.h"
#include "uv.h"
#include "threadqueue.h"

/* BEGIN INTERNAL HELPER FUNCTIONS */
int setRegionPath(char ** buffer, int rx, int ry)
{
	unsigned short regionPathLength = savePathLength + getBytesForInt(rx) + getBytesForInt(ry) + 3;
	*buffer = (char *)malloc(regionPathLength); // format world/r_0_0
	sprintf(*buffer, "%sr_%d_%d", WORLD_DB_PATH, rx, ry);

	return regionPathLength;
}

int setChunkPath(char ** buffer, char * prevPath, unsigned short prevPathLength, int cx, int cy)
{
	unsigned short chunkPathLength = prevPathLength + getBytesForInt(cx) + getBytesForInt(cy) + 4;
	*buffer = (char *)malloc(chunkPathLength); // format world/r_0_0/c_0_0
	sprintf(*buffer, "%s/c_%d_%d", prevPath, cx, cy);

	return chunkPathLength;
}

void makeDir(char * dir)
{
	uv_fs_t mkdir_req;
	uv_fs_mkdir(uv_default_loop(), &mkdir_req, dir, 0700, NULL);
	uv_fs_req_cleanup(&mkdir_req);
}
/* END INTERNAL HELPER FUNCTIONS */


void initWorld() {
	// number of bytes needed to hold the root world path
	savePathLength = strlen(WORLD_DB_PATH);

	// Create the root world directory
	makeDir(WORLD_DB_PATH);
}

void loadWorld()
{
	// Load in the world
	for (int rx = 0; rx < REGION_LENGTH; rx++)
	{
		for (int ry = 0; ry < REGION_WIDTH; ry++)
		{
			for (int cx = 0; cx < WORLD_LENGTH; cx++)
			{
				for (int cy = 0; cy < WORLD_WIDTH; cy++)
				{
					if (!loadChunk(rx, ry, cx, cy)){
						printf("WORLD HAS NOT BEEN GENERATED!!!\n");
						return;
					}
				}
			}
		}
	}
}
char loadChunk(int rx, int ry, unsigned char cx, unsigned char cy)
{
	char bSuccess = 1;

	// Get region path
	char * regionPath; // format world/r_0_0
	int regionPathLength = setRegionPath(&regionPath, rx, ry);

	char * chunkPath;
	setChunkPath(&chunkPath,regionPath, regionPathLength, cx, cy);

	// Read in blocks
	uv_fs_t open_req;
	uv_fs_t read_req;
	uv_fs_t close_req;

	uv_fs_open(uv_default_loop(), &open_req, chunkPath,  O_RDONLY, 0700, NULL);

	if (open_req.result >= 0){
		for (int z = 0; z < 256; z++)
		{
			uv_fs_read(uv_default_loop(), &read_req, open_req.result,world[cx][cy][z], 512, z * 512, NULL);
			uv_fs_req_cleanup(&read_req);
		}
	}
	else {
		bSuccess = 0;
	}

	uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
	uv_fs_req_cleanup(&close_req);
	uv_fs_req_cleanup(&open_req);

	free(regionPath);
	free(chunkPath);

	return bSuccess;
}

void storageProcessor(void *arg)
{
	struct threadmsg * msg = malloc(sizeof(struct threadmsg));

	while(!thread_queue_get(saveQueue, NULL, msg))
	{
		block_s * block = (block_s *)msg->data;

		saveBlock(block);

		free(block);
	}

	printf("EXITING READ FILE QUEUE\n");
}

void runStorageProcessor()
{
	uv_thread_t thread;

	saveQueue = malloc(sizeof(struct threadqueue));
	thread_queue_init(saveQueue);

	uv_thread_create(&thread, storageProcessor, NULL);
}

void clearAllSto() {
	// Get region path
	char * regionPath;
	int regionPathLength = setRegionPath(&regionPath, 0, 0);

	// remove directory
	uv_fs_t rmdir_req;
	uv_fs_rmdir(uv_default_loop(), &rmdir_req, regionPath, NULL);
	uv_fs_req_cleanup(&rmdir_req);
	free(regionPath);
}

void addBlockQue(unsigned char cx, unsigned char cy, unsigned char x, unsigned char y, unsigned short z, unsigned short type)
{
	block_s * block = malloc(sizeof(block_s));
	block->cx = cx;
	block->cy = cy;
	block->x = x;
	block->y = y;
	block->z = z;
	block->type = type;

	thread_queue_add(saveQueue, block, 0);
}


void saveBlock(block_s * block)
{
	// Get region path
	char * regionPath;
	int regionPathLength = setRegionPath(&regionPath, 0, 0);

	// Make the path
	makeDir(regionPath);

	// Get chunk path
	char * chunkPath;
	setChunkPath(&chunkPath, regionPath, regionPathLength, block->cx, block->cy);

	uv_fs_t open_req;
	uv_fs_t write_req;
	uv_fs_t close_req;

	// store type in buffer
	unsigned char typeBuffer[2];
	shortToBuffer(4, 0, typeBuffer);

	// Open
	uv_fs_open(uv_default_loop(), &open_req, chunkPath,  O_WRONLY | O_CREAT, 0700, NULL);
	uv_fs_req_cleanup(&open_req);

	uv_fs_write(uv_default_loop(), &write_req, open_req.result, typeBuffer, 2,(block->z * 512 + ((block->x  * 2) + block->y * 16 * 2)), NULL);
	uv_fs_req_cleanup(&write_req);

	// Close
	uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
	uv_fs_req_cleanup(&close_req);

	free(regionPath);
	free(chunkPath);
}

