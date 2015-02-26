/*
 * util.c
 *
 *  Created on: Aug 12, 2013
 *      Author: jt
 */

#include "util.h"


void shortToBuffer(unsigned short int value, int offset, unsigned char * buf)
{
	//printf("offset: %d\n", offset);

	buf[offset] = value & 0xff;
	buf[offset + 1] = (value >> 8) & 0xff;
}

unsigned short bufferToShort(int offset, unsigned char * buf)
{
	return (buf[offset +1] << 8) |  buf[offset];
}

unsigned char getBytesForInt(int value) {
	unsigned char digits = 0;
	while (value) {
		value /= 10;
		digits++;
	}
	return digits + 1;
}
