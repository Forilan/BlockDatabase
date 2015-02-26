/*
 * network.c
 *
 *  Created on: Aug 12, 2013
 *
 */

#include "network.h"
#include "memory.h"
#include "storage.h"
#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "util.h"

void *contextPub;
void *contextSub;
void *pub;
void *sub;
int blockCount = 0;

int startServer(char * serverName, int port)
{
	int maxS = 10000000;

	// Sub
	contextSub = zmq_ctx_new();
	sub = zmq_socket (contextSub, ZMQ_SUB);
	zmq_setsockopt(sub,ZMQ_RCVHWM,&maxS, sizeof(maxS));
	zmq_setsockopt (sub, ZMQ_SUBSCRIBE,NULL,0);
	zmq_connect (sub, "tcp://127.0.0.1:6969");

	// Pub
	contextPub = zmq_ctx_new();
	pub = zmq_socket (contextPub, ZMQ_PUB);
	zmq_setsockopt(pub,ZMQ_SNDHWM,&maxS, sizeof(maxS));
	zmq_bind (pub, "tcp://127.0.0.1:6868");

	unsigned char buffer[9];

	while(1)
	{
		int size = zmq_recv(sub, buffer, 9, 0);



		readPacket(size, buffer);
	}

	return 0;
}

void readPacket(int size, unsigned char * buffer)
{
	unsigned char type = buffer[0];

	// GET BLOCK
	if (type == 0)
	{
		// Load block type into buffer
		unsigned char cx = buffer[1];
		unsigned char cy = buffer[2];
		unsigned char x = buffer[3];
		unsigned char y = buffer[4];
		unsigned short z = bufferToShort(5, buffer);

		unsigned char typeBuffer[2];

		short blockType = getBlock(buffer[1], buffer[2], buffer[3], buffer[4], bufferToShort(5, buffer));

		setvbuf (stdout, NULL, _IONBF, 0);
		printf("Getting Block: %d\n", 	blockCount++);

		shortToBuffer(blockType,0, typeBuffer);
		setvbuf (stdout, NULL, _IONBF, 0);
		printf("cx: %d, cy: %d, x: %d, y: %d, z: %u \n", cx, cy, x, y, z);

		writePacket(2,typeBuffer);
	}
	// SET BLOCK
	else if(type == 1)
	{
		unsigned char cx = buffer[1];
		unsigned char cy = buffer[2];
		unsigned char x = buffer[3];
		unsigned char y = buffer[4];
		unsigned short z = bufferToShort(5, buffer);
		unsigned short type = bufferToShort(7, buffer);
		setvbuf (stdout, NULL, _IONBF, 0);
		printf("cx: %d, cy: %d, x: %d, y: %d, z: %u, type: %u \n", cx, cy, x, y, z,type);
		setBlock(cx, cy, x, y, z, type);
		addBlockQue(cx, cy, x, y, z, type);
	}
	// CLEAR DB
	else if(type == 2)
	{
		clearAllMem();
		//clearAllSto();
	}
}

void writePacket(int size, unsigned char *buffer)
{
	zmq_send(pub, buffer, size, 0);
}
