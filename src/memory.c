/*
 * memory.c
 *
 *  Created on: Aug 12, 2013
 *      Author: jt
 */

#include "util.h"
#include "memory.h"
#include "storage.h"


unsigned short getBlock(unsigned char cx, unsigned char cy, unsigned char x, unsigned char y, unsigned short z)
{
	return bufferToShort(((x*16)+y)*2,world[cx][cy][z]);
}

void setBlock (unsigned char cx, unsigned char cy, unsigned char x, unsigned char y, unsigned short z, unsigned short type)
{
	shortToBuffer(type,((x*16)+y)*2,world[cx][cy][z]);
}

unsigned char *** getChunk (char cx, char cy)
{
	return world[cx][cy];
}

char setChunk(unsigned char *** chunk)
{
	return 0;
}

void clearAllMem()
{
	memset(world, 0,WORLD_LENGTH * WORLD_WIDTH * 256 * 512 );
}
