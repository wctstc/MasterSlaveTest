
#include <winsock2.h>

#include "AccessPoint.h"
#include "LogManage.h"

AccessPoint::AccessPoint(  ):m_socket_access( SOCKET_ERROR )
{
}

AccessPoint::~AccessPoint(void)
{
	if( m_socket_access != SOCKET_ERROR )
	{
		closesocket( m_socket_access );
	}
}

int AccessPoint::Start( int port, string ip  )
{
	if( m_socket_access != SOCKET_ERROR )
	{
		closesocket( m_socket_access );
	}

	//����socket
	m_socket_access = socket(AF_INET, SOCK_STREAM, 0); 
	if( SOCKET_ERROR == m_socket_access )
	{
		//��������
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		m_socket_access = socket(AF_INET, SOCK_STREAM, 0); 
		if( SOCKET_ERROR == m_socket_access )
		{
			return -1;
		}
	}

	//��
	struct sockaddr_in addr;
	memset(&addr, 0x00,sizeof(addr));
	addr.sin_family = AF_INET;
	if ( ip.empty() )
	{
		addr.sin_addr.s_addr = INADDR_ANY;
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
	}

	addr.sin_port = htons( port );
	if( SOCKET_ERROR == bind( m_socket_access, (struct sockaddr *)&addr, sizeof(addr)) )
	{
		closesocket( m_socket_access );
		return -1;
	}

	//����
	if( SOCKET_ERROR == listen( m_socket_access, 10 ))
	{
		closesocket( m_socket_access );
		return -1;
	}

	CLog( "��[%d]�˿�", port );
	Thread::CreateThread( &m_access_thread, OnAccept, this );

	return 0;
}
void *AccessPoint::OnAccept( void *bind )
{

	struct sockaddr_in stPeerAddr;
	int nAddrLen = sizeof(sockaddr);


	AccessPoint *access_point = ( AccessPoint * ) bind;

	while( true )
	{
		//�������ӵ�
		unsigned int sAccept = accept( access_point->m_socket_access,reinterpret_cast<sockaddr*>(&stPeerAddr),&nAddrLen);
		if (INVALID_SOCKET == sAccept)
			return NULL;
		
		//�������̴߳������ӵ�
		pthread_t *new_pthread = new pthread_t;
		unsigned int arg[2] = {sAccept,(unsigned int)bind};//����

		Thread::CreateThread( new_pthread, OnReceive, (void*)arg );
		access_point->m_client_thread.push_back( new_pthread );

		//��ȡ���ӵ�ĵ�ַ�Ͷ˿�
		struct sockaddr_in peer_address;
		int len = sizeof( peer_address );
		if( !getpeername( sAccept,(struct sockaddr *)&peer_address,&len ) )
		{
			CLog( "�յ�����%s:%d����", inet_ntoa(peer_address.sin_addr), ntohs(peer_address.sin_port) );
		}
	}
	return NULL;
}


void *AccessPoint::OnReceive( void* arg )
{
	//��һ������Ϊsocket���ڶ�������ΪAccessPoint�����ַ
	unsigned int socket = (( unsigned int *)arg)[0];
	AccessPoint *parent = (AccessPoint *)(((unsigned int *)arg)[1]);


	char temp[100];
	int temp_length;
	string recvBuffer = "";

	do
	{
		temp_length = recv( socket, temp, 100, 0 );
		recvBuffer.append( temp );
		if( temp_length < 100 )
		{
			break;
		}
	}
	while( true );

	send( socket, "I recv you messge", 17, 0 );

	closesocket( socket );
	return NULL;
}