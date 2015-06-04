#pragma once

#include <string>
#include <vector>
#include "Thread.h"

using namespace std;

class AccessPoint
{
public:
	AccessPoint(void);
	~AccessPoint(void);
public:
	int Start( int port, string ip = ""  );

public:
	static void *OnAccept( void *bind );
	static void *OnReceive( void *arg  );
private:
	unsigned int m_socket_access;


private:
	pthread_t m_access_thread;
	vector< pthread_t * > m_client_thread;
};
