/*******************************************************************************
* This program and the accompanying materials
* are made available under the terms of the Common Public License v1.0
* which accompanies this distribution, and is available at 
* http://www.eclipse.org/legal/cpl-v10.html
* 
* Contributors:
*     Peter Smith
*******************************************************************************/

#include "Protocol.h"

#include <stdio.h>

int BinaryProtocol::connect()
{
	if(conn != NULL) return 0;

	WSADATA wsData;
	int wsaret = WSAStartup(0x101, &wsData);
	if(wsaret)
		return 1;

	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(conn == INVALID_SOCKET) 
		return 1;

	hostent* hp;
	if(inet_addr(hostname) == INADDR_NONE) {
		hp = gethostbyname(hostname);
	}
	else {
		unsigned int addr = inet_addr(hostname);
		hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	}

	if(hp == NULL) {
		closesocket(conn);
		conn = NULL;
		return 1;
	}

	struct sockaddr_in server;
	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	// Set timeout
	struct timeval tv;
	tv.tv_sec = 500;
	tv.tv_usec = 0;
	if(setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)))
		return 1;

	if(::connect(conn, (struct sockaddr*) &server, sizeof(server))) {
		closesocket(conn);
		conn = NULL;
		return 1;	
	}

	return 0;
}

void BinaryProtocol::disconnect()
{
	if(conn) {
		closesocket(conn);
		conn = NULL;
	}

	WSACleanup();
}

int BinaryProtocol::send(const char* name, LPXLOPER v0, LPXLOPER v1, LPXLOPER v2, LPXLOPER v3, 
		LPXLOPER v4, LPXLOPER v5, LPXLOPER v6, LPXLOPER v7, LPXLOPER v8, LPXLOPER v9)
{
	XLCodec::encode(name, os);
	XLCodec::encode(10, os);
	XLCodec::encode(v0, os);
	XLCodec::encode(v1, os);
	XLCodec::encode(v2, os);
	XLCodec::encode(v3, os);
	XLCodec::encode(v4, os);
	XLCodec::encode(v5, os);
	XLCodec::encode(v6, os);
	XLCodec::encode(v7, os);
	XLCodec::encode(v8, os);
	XLCodec::encode(v9, os);
	os.flush();
	return conn == NULL ? 1 : 0;
}

int BinaryProtocol::send(const char* name)
{
	XLCodec::encode(name, os);
	XLCodec::encode(0, os);
	os.flush();
	return conn == NULL ? 1 : 0;
}

LPXLOPER BinaryProtocol::receive(const char* name)
{
	LPXLOPER xl = new XLOPER;
	XLCodec::decode(name, is, xl);
	return xl;
}