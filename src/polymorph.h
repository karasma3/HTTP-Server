#ifndef POLYMORPH_H
#define POLYMORPH_H

#include <netdb.h>

#include <set>
#include <vector>
#include <dirent.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "auxiliary.h"

using namespace std;

/// Class for communication
/**
* This abstract class responds to request from a client.
* It stores first line of a clien's request, client's IP adress.
* There are also constants whether its OK or there was an ERROR loading some data
*/
class CRequest
{
    public:
	/// Implicit constructor
        CRequest();
	/// Destructor
        virtual ~CRequest();
	/// Responds to a client's request according to the request
	/**
	* Either it is a some Html page, image or text or it is content of directory
	*/
        virtual void Response( int clientSock, const CConfig& config, const CTime& b ) const = 0;

    protected:
        string m_head;
	string m_clientIP;
	static const int OK = 200;
	static const int ERROR = 404;
	static const string NOTFOUND;
	static const string BADREQUEST;

};

/// Page
/**
* Class inherited from CRequest that responds to request by sending a header with a content of a file
*/
class CPage:public CRequest
{
    public:
	/// Constructor
	/**
	* \param head Head \param clientIP Client's IP address
	*/
        CPage( const string& head, const string& clientIP );
	/// Destructor
        virtual ~CPage();
	/// Responds to a client's request by sending him a header and a file
	/**
	* Header depends on whether there are files to send or client requested non-existing files
	* \param clientSock socket on which client and server communicates
	* \param config configuration data \param b time
	*/
        virtual void Response( int clientSock, const CConfig& config, const CTime& b ) const;

    protected:

};

/// Content
/**
* Class inherited from CReuquest that responds to request by sending a content of a directory
*/
class CContent:public CRequest
{
    public:
	/// Constructor
	/**
	* \param head Head \param clientIP Client's IP address
	*/
        CContent( const string& head, const string& clientIP );
	/// Destructor
        virtual ~CContent();
	/// Responds to a client's request by sending him header and list of all stuff which is in the directory
	/**
	* Header depends on whether the directory which is being listed exists or not
	* \param clientSock socket on which client and server communicates
	* \param config configuration data \param b time
	*/
        virtual void Response( int clientSock, const CConfig& config, const CTime& b ) const;

    protected:

};


#endif /* POLYMORPH_H */

