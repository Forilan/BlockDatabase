/*
 * network.h
 *
 *  Created on: Aug 12, 2013
 *      Author: jt
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "uv.h"
void writePacket(int size, unsigned char *buffer);
void readPacket(int size,unsigned char * buffer);
int startServer(char * serverName, int port);
void on_new_connection(uv_stream_t *server, int status);
uv_buf_t alloc_buffer(uv_handle_t *handle, size_t suggested_size);
void on_client_read(uv_stream_t *client, ssize_t nread, uv_buf_t buf);
void on_client_write(uv_write_t *req, int status);

#endif /* NETWORK_H_ */


/*
 *  MoondosNetwork1 MoondosNetwork2 MoondosNetwork3 MoondosNetwork4 MoondosNetwork5
 *  MoondosMain1 -> MoondosDb1 (which handles only 1 region)
 *  MoondosMain2 -> MoondosDb2 (which handles only 1 region)
 */
