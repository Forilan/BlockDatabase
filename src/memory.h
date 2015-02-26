/*
 * memory.h
 *
 *  Created on: Aug 12, 2013
 *      Author: jt
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "settings.h"

unsigned char world[WORLD_WIDTH][WORLD_LENGTH][256][512]; // PENTA POINTERS!!!!

unsigned short getBlock(unsigned char cx, unsigned char cy, unsigned char x, unsigned char y, unsigned short z);
void setBlock (unsigned char cx, unsigned char cy, unsigned char x, unsigned char y, unsigned short z, unsigned short type);
unsigned char *** getChunk (char cx, char cy);
char setChunk(unsigned char *** chunk);
void clearAllMem();


#endif /* MEMORY_H_ */
