


#include "ConnectPoint.h"
#include "Config.h"
#include "AccessPoint.h"
#include "ConnectPoint.h"



int main()
{
	//Config config( "config.ini" );
	//string ip;
	//string port;
	//config.GetKeyValue( "local","ip",   ip   );
	//config.GetKeyValue( "local","port", port );

	AccessPoint access_point;
	ConnectPoint connect_point;
	string sendBuffer =  "hello world" ;
	string recvBuffer;

	access_point.Start( 10812 );
	connect_point.Connect( "127.0.0.1", 10812 );
	connect_point.SendBuffer( sendBuffer );
	connect_point.RecvBuffer( recvBuffer );

	cout << "RecvBuffer:" << recvBuffer.c_str() << endl;

	return 1;
}