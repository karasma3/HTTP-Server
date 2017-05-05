
#include "server.h"


const string CServer::SOCKERROR = "Error server socket";

CServer::CServer()
{

}

CServer::~CServer()
{

}

int CServer::start()
{
	try
	{
		m_config.getConfig();
	}
	catch ( const CException& exception )
	{
		cout << exception;
		return -1;
	}
	int listenSock;
	try
	{
		listenSock = prepareSrvSocket ( m_config.getIP().c_str(), m_config.getPort().c_str() );
	}
	catch ( const CException& exception )
	{
		cout << exception;
		return -1;
	}

	return listenSock;
	
}

int CServer::prepareSrvSocket ( const char * listenAddr, const char* listenPort )
{
	struct addrinfo * ai;

	if ( getaddrinfo ( listenAddr, listenPort, NULL, &ai ) != 0 )
	{
		throw CException( SOCKERROR );
	}

	int sock = socket ( ai -> ai_family, SOCK_STREAM, 0 );

	if ( sock == -1 )
	{
		freeaddrinfo ( ai );
		throw CException( SOCKERROR );
	}

	if ( bind ( sock, ai -> ai_addr, ai -> ai_addrlen ) != 0 )
	{
		close ( sock );
		freeaddrinfo ( ai );
		throw CException( SOCKERROR );
	}

	if ( listen ( sock, 10 ) != 0 )
	{
		close ( sock );
		freeaddrinfo ( ai );
		throw CException( SOCKERROR );
	}

	freeaddrinfo ( ai );

	return sock;

}

void CServer::serveClient ( int clientSock, const string& clientIP ) const
{
	stringstream stream;

	while ( 1 )
	{
		char buffer[BUFFERSIZE] = {0};

		int len = recv ( clientSock, buffer, BUFFERSIZE, 0 );

	 	for( int i = 0; i < len; i++ )
		{
			stream << buffer[i];
		}

		if ( len <= 0 )
		{
			break;
		}
		if( len>= 2 )
		{
			if( buffer[0]=='\r' && buffer[1]=='\n' )
			{
				break;
			}
		}		
		if( len >=4 )
		{
			if( buffer[len-4] == '\r' && buffer[len-3] == '\n' && buffer[len-2] == '\r' && buffer[len-1]=='\n' )
			{
				break;
			}
		}

	}

	time_t rawtime;
	time( &rawtime );
	CTime b( gmtime( &rawtime ));

	CRequest* a;
	string head,file,end;
	getline( stream, head, '\r' );

	stream.clear();
	stream.str( head );
	string request;
	getline( stream, request, ' ' );
	getline( stream, file, ' ' );
	getline( stream, end );

	if( file.find(".") != string::npos )
	{
		a = new CPage( head, clientIP );
	}
	else
	{
		a = new CContent( head, clientIP );
	}

	try
	{
		a -> Response( clientSock, m_config, b );   
	}
	catch ( const CException& exception )
	{
		cout << exception;
	}
  
	delete a;

	close ( clientSock );

}

