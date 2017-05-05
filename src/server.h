#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <ctime>
#include <string>
#include <sstream>
#include <iostream>
 
#include "polymorph.h"

using namespace std;

/// Server
/**
* This class represents HTTP server
* It stores configuration data such IP, port, path to root and path to log
* Also it has constant for when there is some error with socket
*/
class CServer
{
    public:
	/// Implicit constructor
	CServer();
	/// Destructor
	~CServer();
	/// Starts the server
	/**
	* \return Socket for communication between client and server or -1 when there is some error
	*/
	int start();
	/// Serving client
	/**
	* \param clientSock Socket on which communication will be done \param clientIP Client's IP address
	*/
	void serveClient ( int clientSock, const string& clientIP ) const;

    private:
	/// Preparing server socket on which client's can connect
	/**
	* \param listenAddr IP address of server \param listenPort Port of server
	* \return Socket
	*/
	int prepareSrvSocket ( const char * listenAddr, const char* listenPort );

	CConfig m_config;
	static const int BUFFERSIZE = 500;
	static const string SOCKERROR;

};


#endif /* SERVER_H */

