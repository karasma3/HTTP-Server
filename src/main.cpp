
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include "server.h" 

#define START "Server is running without any complication."
#define END "Server is shuting down."

using namespace std;


//------------------------------------------------------
// Functions for catching signals for interupting server 

static int s_interrupted = 0;

static void s_signal_handler ( int signal_value )
{
    s_interrupted = 1;
}

static void s_catch_signals ( void )
{
    struct sigaction action;
    action.sa_handler = s_signal_handler;
    action.sa_flags = 0;
    sigemptyset ( &action.sa_mask );
    sigaction ( SIGINT, &action, NULL );
    sigaction ( SIGTERM, &action, NULL );

}
//------------------------------------------------------

int main ( int argc, char * argv [] )
 {

	CServer server;

	int listenSock = server.start();

	if ( listenSock == -1 )
	{
		return 1;
	}

	cout << START <<endl;

	s_catch_signals ();

	while ( 1 )
	{
		struct sockaddr addr;
		socklen_t addrLen = sizeof ( addr );

		int clientSock = accept ( listenSock, &addr, &addrLen );

		string clientIP;

		if( clientSock>=0 )
		{
			clientIP = to_string( int ( addr.sa_data[2] & 0xff ) );

			for( int i = 3; i < 6; i++ )
			{
				clientIP += "." + to_string( int ( addr.sa_data[i] & 0xff ) );
			}

		}

		if( clientSock != -1 ) 
		{
			server.serveClient ( clientSock, clientIP );
		}

		if ( s_interrupted )
		{
			cout << endl << END << endl;
			break;
        	}

	}

	close ( listenSock );

   return 0;
 }


