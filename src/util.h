/*
 * util.h
 *
 *  Created on: Aug 12, 2013
 *      Author: jt
 */

#ifndef UTIL_H_
#define UTIL_H_

struct vector3
{
	int x,y,z;
};

void shortToBuffer(unsigned short int value, int offset, unsigned char * buf);
unsigned short bufferToShort(int offset, unsigned char * buf);
unsigned char getBytesForInt(int value);

#endif /* UTIL_H_ */
