#ifndef __CONNECT_POINT_H__
#define __CONNECT_POINT_H__

#include <string>
using namespace std;


class ConnectPoint
{
public:
	ConnectPoint( string ip = "", int port = 0 );
	~ConnectPoint(void);

public:
public:
	int Connect( string ip, int port );
	int SendBuffer( string &buffer );
	int RecvBuffer( string &buffer );

private:
	unsigned int  m_socket;

};

#endif