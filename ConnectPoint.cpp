
#include <winsock2.h>

#include "ConnectPoint.h"
#include "LogManage.h"

ConnectPoint::ConnectPoint( string ip, int port )
{
	if( ip.empty() == false && port != 0 )
		Connect( ip, port );
}

ConnectPoint::~ConnectPoint(void)
{
	if( m_socket != INVALID_SOCKET )
	{
		closesocket( m_socket );
	}
}
int ConnectPoint::Connect( string ip, int port )
{
	if( m_socket != INVALID_SOCKET )
	{
		closesocket( m_socket );
	}
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == m_socket )
	{	
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		m_socket = socket(AF_INET, SOCK_STREAM, 0); 
		if( SOCKET_ERROR == m_socket )
		{
			CLog( "%s","Error at Connect socket()" );
			return -1;
		}
	}

	sockaddr_in addrServer; 
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.s_addr = inet_addr( ip.c_str());
	addrServer.sin_port = htons( port );
	if ( SOCKET_ERROR == connect( m_socket, (struct sockaddr*) &addrServer, sizeof(addrServer) ))
	{
		closesocket( m_socket );
		CLog( "连[%s:%d]失败",ip.c_str(), port);
		return -1;
	}

	CLog( "与[%s:%d]的连接成功", ip.c_str(), port);

	return 0;
}

int ConnectPoint::SendBuffer( string &buffer )
{
	if( m_socket == INVALID_SOCKET )
	{
		return 0;
	}
	
	return send( m_socket, buffer.c_str(),buffer.length(),0);

}

int ConnectPoint::RecvBuffer( string &buffer )
{
	char temp[100];
	int length;
	do 
	{
		memset( temp, 0, 100 );
		length = recv( m_socket, temp, 100, 0 );
		buffer.append( temp );
		if( length < 100 )
			break;
	} 
	while (true);
	return 1;
}