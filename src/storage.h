/*
 * storage.h
 *
 *  Created on: Aug 12, 2013
 *      Author: jt
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include "uv.h"
unsigned short savePathLength;

typedef struct
{
	unsigned char cx;
	unsigned char cy;
	unsigned char x;
	unsigned char y;
	unsigned short z;
	unsigned short type;
}  block_s;

struct threadqueue *saveQueue;

void initWorld();
void loadWorld();
void runStorageProcessor();
void addBlockQue(unsigned char cx, unsigned char cy, unsigned char x, unsigned char y, unsigned short z, unsigned short type);
char loadChunk(int rx, int ry, unsigned char cx, unsigned char cy);
void saveBlock(block_s * block);
void clearAllSto();

#endif /* STORAGE_H_ */
